#include "clay_renderer_SFML2.hpp"

// This implimentation is kind of nasty- we create an sf::Text with the desired properties and call
// SFML Text's own getLocalBounds function to measure it. Clay caches this, so it doesn't need to be
// the fastest thing in the world
Clay_Dimensions SFML_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData)
{
    SFML_Renderer *renderer = (SFML_Renderer*)userData;

    sf::Font& font = renderer->fonts[config->fontId];
    char *chars = (char *)calloc(text.length + 1, 1);
    memcpy(chars, text.chars, text.length);
    int width = 0;
    int height = 0;
    
    sf::Text t;
    t.setFont(font);
    t.setString(chars);
    auto floatDimensions = t.getLocalBounds();
    width = floatDimensions.width;
    height = floatDimensions.height;

    free(chars);
    Clay_Dimensions dimensions;
    dimensions.width = width;
    dimensions.height = height;
    return dimensions;
}

static void SFML_RenderFillRoundedRect(SFML_Renderer* renderer, const sf::FloatRect rect, const float cornerRadius, const Clay_Color _color) {
    const sf::Color colour(_color.r, _color.g, _color.b, _color.a);


    sf::CircleShape circ;
    circ.setFillColor(colour);
    circ.setRadius(cornerRadius);
    circ.setOrigin(cornerRadius, cornerRadius);
    
    circ.setPosition(sf::Vector2f(rect.left, rect.top) + sf::Vector2f{cornerRadius, cornerRadius});
    renderer->target->draw(circ);
    circ.setPosition(sf::Vector2f(rect.left + rect.width, rect.top) + sf::Vector2f{cornerRadius * -1.0f, cornerRadius});
    renderer->target->draw(circ);
    circ.setPosition(sf::Vector2f(rect.left, rect.top + rect.height) + sf::Vector2f{cornerRadius, cornerRadius * -1.0f});
    renderer->target->draw(circ);
    circ.setPosition(sf::Vector2f(rect.left + rect.width, rect.top + rect.height) - sf::Vector2f{cornerRadius, cornerRadius});
    renderer->target->draw(circ);

    sf::RectangleShape r;
    r.setFillColor(colour);
    r.setSize(sf::Vector2f(rect.width, rect.height - cornerRadius * 2.0f));
    
    r.setPosition(sf::Vector2f(rect.left, rect.top + cornerRadius));
    renderer->target->draw(r);
    r.setSize(sf::Vector2f(rect.width - cornerRadius * 2.0f, rect.height));
    r.setPosition(rect.left + cornerRadius, rect.top);
    renderer->target->draw(r);
}

static void SFML_RenderFillRect(SFML_Renderer* renderer, sf::FloatRect boundingBox, const Clay_Color color) {
    sf::RectangleShape shape;
    shape.setSize(sf::Vector2f(boundingBox.width, boundingBox.height));
    shape.setPosition(sf::Vector2f(boundingBox.left, boundingBox.top));
    shape.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
    renderer->target->draw(shape);
}

#define M_PI 3.141592

static void SFML_RenderCornerBorder(sf::RenderTarget& target, sf::FloatRect boundingBox, Clay_CornerRadius cornerRadii,
    Clay_BorderWidth borderWidths, Clay_Color color, int cornerIndex) {
    // Unfinished. I can't quite grok it
}

void Clay_SFML_Render(SFML_Renderer *renderer, Clay_RenderCommandArray renderCommands)
{
    // If we deleted them last frame it could corrupt the displayed image
    for(auto* tex: renderer->oldRenderTextures)
        delete tex;
    renderer->oldRenderTextures.clear();

    for (uint32_t i = 0; i < renderCommands.length; i++)
    {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;
        switch (renderCommand->commandType)
        {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleRenderData *config = &renderCommand->renderData.rectangle;
                Clay_Color color = config->backgroundColor;
                sf::FloatRect rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
                if (config->cornerRadius.topLeft > 0 && false) {
                    SFML_RenderFillRoundedRect(renderer, rect, config->cornerRadius.topLeft, color);
                }
                else {
                    SFML_RenderFillRect(renderer, rect, color);
                }
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                Clay_TextRenderData *textData = &renderCommand->renderData.text;
                char *cloned = (char *)malloc(textData->stringContents.length + 1);
                memcpy(cloned, textData->stringContents.chars, textData->stringContents.length);
                cloned[textData->stringContents.length] = '\0';
                sf::Font& font = renderer->fonts[textData->fontId];
                sf::Color textColour(textData->textColor.r, textData->textColor.g, textData->textColor.b, textData->textColor.a);
                sf::Text text(cloned, font, textData->fontSize);
                text.setPosition(boundingBox.x, boundingBox.y);
                text.setLetterSpacing(textData->letterSpacing);
                text.setFillColor(textColour);
                text.setStyle(sf::Text::Bold);
                
                renderer->target->draw(text);
                free(cloned);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                // SFML 2 doesn't support scissor mode, so we will make a new render target.
                // If a fragment is outside of the bounds of this target, it will be discarded, so
                // we are simulating the effect

                renderer->clippedRects.emplace(boundingBox);
                renderer->pushedTargets.emplace(renderer->target);
                sf::RenderTexture* clipTex = new sf::RenderTexture;
                clipTex->create(boundingBox.width, boundingBox.height);
                clipTex->setView(sf::View(sf::FloatRect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)));
                clipTex->clear(sf::Color(0, 0, 0, 0));
                renderer->target = clipTex;

                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                // See above- we will have created a new render texture to be our little
                // scissor layer. We now need to render it, and then draw that texture to the
                // correct position on the target beneath it.

                sf::RenderTexture* renderTex = (sf::RenderTexture*)renderer->target;
                renderTex->display();

                Clay_BoundingBox clip = renderer->clippedRects.front();
                renderer->clippedRects.pop();
                renderer->target = renderer->pushedTargets.front();
                renderer->pushedTargets.pop();

                sf::RectangleShape shape(sf::Vector2f(clip.width, clip.height));
                shape.setPosition(clip.x, clip.y);
                shape.setTexture(&renderTex->getTexture());
                shape.setTextureRect(sf::IntRect(0, 0, clip.width, clip.height));

                renderer->target->draw(shape);
                renderer->oldRenderTextures.emplace_back(renderTex);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                // TODO, images currently don't scale nicely
                Clay_ImageRenderData *config = &renderCommand->renderData.image;
                
                sf::Texture* texture = (sf::Texture*)config->imageData;
                sf::RectangleShape rect(sf::Vector2f(boundingBox.width, boundingBox.height));
                rect.setPosition(boundingBox.x, boundingBox.y);
                rect.setTexture(texture);
                rect.setTextureRect(sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y));

                renderer->target->draw(rect);

                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderRenderData *config = &renderCommand->renderData.border;
                // SDL_SetRenderDrawColor(renderer, CLAY_COLOR_TO_SDL_COLOR_ARGS(config->color));

                if(boundingBox.width > 0 & boundingBox.height > 0){
                     const float maxRadius = std::min(boundingBox.width, boundingBox.height) / 2.0f;

                     if (config->width.left > 0) {
                         const float clampedRadiusTop = std::min((float)config->cornerRadius.topLeft, maxRadius);
                         const float clampedRadiusBottom = std::min((float)config->cornerRadius.bottomLeft, maxRadius);
                         sf::FloatRect rect = { 
                             boundingBox.x, 
                             boundingBox.y + clampedRadiusTop, 
                             (float)config->width.left, 
                             boundingBox.height - clampedRadiusTop - clampedRadiusBottom
                         };
                         SFML_RenderFillRect(renderer, rect, config->color);
                     }
    
                     if (config->width.right > 0) {
                         const float clampedRadiusTop = std::min((float)config->cornerRadius.topRight, maxRadius);
                         const float clampedRadiusBottom = std::min((float)config->cornerRadius.bottomRight, maxRadius);
                         sf::FloatRect rect = {
                             boundingBox.x + boundingBox.width - config->width.right,
                             boundingBox.y + clampedRadiusTop,
                             (float)config->width.right,
                             boundingBox.height - clampedRadiusTop - clampedRadiusBottom
                         };
                         SFML_RenderFillRect(renderer, rect, config->color);
                     }
    
                     if (config->width.top > 0) {
                         const float clampedRadiusLeft = std::min((float)config->cornerRadius.topLeft, maxRadius);
                         const float clampedRadiusRight = std::min((float)config->cornerRadius.topRight, maxRadius);
                         sf::FloatRect rect = {
                             boundingBox.x + clampedRadiusLeft, 
                             boundingBox.y, 
                             boundingBox.width - clampedRadiusLeft - clampedRadiusRight, 
                             (float)config->width.top };
                         SFML_RenderFillRect(renderer, rect, config->color);
                     }
    
                     if (config->width.bottom > 0) {
                         const float clampedRadiusLeft = std::min((float)config->cornerRadius.bottomLeft, maxRadius);
                         const float clampedRadiusRight = std::min((float)config->cornerRadius.bottomRight, maxRadius);
                         sf::FloatRect rect = {
                             boundingBox.x + clampedRadiusLeft, 
                             boundingBox.y + boundingBox.height - config->width.bottom, 
                             boundingBox.width - clampedRadiusLeft - clampedRadiusRight, 
                             (float)config->width.bottom
                         };
                         SFML_RenderFillRect(renderer, rect, config->color);
                     }
    
                     //corner index: 0->3 topLeft -> CW -> bottonLeft
                     sf::FloatRect floatBox;
                     if (config->width.top > 0 && config->cornerRadius.topLeft > 0) {
                         SFML_RenderCornerBorder(*renderer->target, floatBox, config->cornerRadius ,config->width , config->color, 1);
                     }

                     if (config->width.top > 0 && config->cornerRadius.topRight> 0) {
                         SFML_RenderCornerBorder(*renderer->target, floatBox, config->cornerRadius, config->width, config->color, 2);
                     }

                     if (config->width.bottom > 0 && config->cornerRadius.bottomLeft > 0) {
                         SFML_RenderCornerBorder(*renderer->target, floatBox, config->cornerRadius, config->width, config->color, 3);
                     }

                     if (config->width.bottom > 0 && config->cornerRadius.bottomLeft > 0) {
                         SFML_RenderCornerBorder(*renderer->target, floatBox, config->cornerRadius, config->width, config->color, 4);
                     }
                 }

                break;
            }
            default: {
                fprintf(stderr, "Error: unhandled render command: %d\n", renderCommand->commandType);
                exit(1);
            }
        }
    }
}

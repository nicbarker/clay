#pragma once
#include "../../clay.h"
#include <SFML/Graphics.hpp>
#include <queue>

struct SFML_Renderer;

Clay_Dimensions SFML_MeasureText(Clay_StringSlice text, Clay_TextElementConfig* config, void* userData);

void Clay_SFML_Render(SFML_Renderer* renderer, Clay_RenderCommandArray renderCommands);


struct SFML_Renderer{
    std::vector<sf::Font> fonts;
    sf::RenderTarget* target;

    friend void Clay_SFML_Render(SFML_Renderer* renderer, Clay_RenderCommandArray renderCommands);
private:
    std::queue<sf::RenderTarget*> pushedTargets;
    std::queue<Clay_BoundingBox> clippedRects;
    std::vector<sf::RenderTexture*> oldRenderTextures;
};

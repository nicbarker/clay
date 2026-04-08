#ifndef CLAY_RENDERER_QRHI_H
#define CLAY_RENDERER_QRHI_H

#ifdef __cplusplus
#include <QFile>
#include <QGuiApplication>
#include <QPalette>
#include <QQuickWindow>
#include <QSGRenderNode>
#include <QSGTextNode>
#include <QSGTextureProvider>
#include <QSGTransformNode>
#include <QTextLayout>
#include <QtMinMax>

#include <rhi/qrhi.h>
#endif

#include <clay.h>

#define CLAY_RENDERER_QRHI_TEX_COUNT 16
#define CLAY_RENDERER_MAX_ELEMENTS 16384

/*
 * Instanced rendering for Rects/Images/Borders
 * will use this data
 * Note, it needs to be padded to 4 floats
 * Draws:
 * - One rectangular with possibly rounded corner
 * - And possibly with a hole inside (with rounded edges too, if corners are rounded)
 * - It could also draw a picture with also a rounded corner
 */
typedef struct RectInstance {
    float x, y, w, h;         // 4 Draw where on screen
    float u0, v0, u1, v1;     // 4 Atlas region
    float r, g, b, a;         // 4 Color
    float radiusTL, radiusTR; // 2 Corner rounding
    float radiusBL, radiusBR; // 2
    float borderL, borderR;   // 2 Border widths
    float borderT, borderB;   // 2
    float texToUse;           // 1 Texture atlas to take an image from (1-CLAY_RENDERER_QRHI_TEX_COUNT)
    float pad[3];             // 3
} RectInstance;

typedef struct QuadInstanceArray {
    RectInstance *instData; // packed per-instance floats
    int           capacity; // how many instances it can hold
    int           count;    // how many instances does it actually hold
} QuadInstanceArray;

typedef struct ImageConfig {
    int   textureToUse;
    float u0, v0;
    float u1, v1;
} ImageConfig;

#ifdef __cplusplus
class ClayFastQSGRenderNode : public QSGRenderNode {
  public:
    ClayFastQSGRenderNode(QQuickWindow *window);

    void setCmds(Clay_RenderCommandArray cmds, const QList<QFont> &fonts);
    void setTextures(QRhiTexture *textures[CLAY_RENDERER_QRHI_TEX_COUNT]);

    void                      prepare() override;
    void                      render(const RenderState *state) override;
    void                      releaseResources() override;
    RenderingFlags            flags() const override;
    QSGRenderNode::StateFlags changedStates() const override;

  private:
    std::unique_ptr<QRhiGraphicsPipeline> configurePipeline();

    QQuickWindow *m_window;

    std::unique_ptr<QRhiBuffer>                 m_vertexBuffer;
    std::unique_ptr<QRhiBuffer>                 m_quadInstanceBuffer;
    std::unique_ptr<QRhiBuffer>                 m_uniformBuffer;
    std::vector<std::unique_ptr<QRhiSampler>>   m_samplers;
    std::unique_ptr<QRhiShaderResourceBindings> m_pipelineResourceBindings;
    std::unique_ptr<QRhiShaderResourceBindings> m_resourceBindings;
    std::unique_ptr<QRhiGraphicsPipeline>       m_pipeline;
    QList<QRhiShaderStage>                      m_shaders;

    QRhiTexture **m_textures;
    bool          m_texturesStale;

    QSGTextNode *m_textNode;

    Clay_RenderCommandArray m_cmds;

    QuadInstanceArray m_quads; // Each instance is one quad

    int m_quadInstancesCount = 0;
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

Clay_Dimensions clayRendererQRhiMeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);

Clay_Color clayRendererQRhiPaletteBaseColor();
Clay_Color clayRendererQRhiPaletteWindowColor();
Clay_Color clayRendererQRhiPaletteHighlightColor();
Clay_Color clayRendererQRhiPaletteTextColor();
Clay_Color clayRendererQRhiPaletteHiglightedTextColor();

#ifdef __cplusplus
}
#endif

#ifdef CLAY_RENDERER_QRHI_IMPLEMENTATION

#ifdef __cplusplus
extern "C" {
#endif

Clay_Dimensions clayRendererQRhiMeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    QList<QFont> *fonts = *(QList<QFont> **)userData;
    QTextLayout   textLayout(QString::fromUtf8(text.chars, text.length));
    textLayout.setFont((*fonts)[config->fontId]);
    textLayout.beginLayout();
    textLayout.createLine();
    textLayout.endLayout();

    QRectF boundingRect = textLayout.boundingRect();

    return (Clay_Dimensions){
        .width  = (float)boundingRect.width(),
        .height = (float)boundingRect.height(),
    };
}

Clay_Color clayRendererQRhiPaletteBaseColor() {
    QColor color = QGuiApplication::palette().base().color();
    return Clay_Color{
        (float)color.red(),
        (float)color.green(),
        (float)color.blue(),
        (float)color.alpha(),
    };
}

Clay_Color clayRendererQRhiPaletteWindowColor() {
    QColor color = QGuiApplication::palette().window().color();
    return Clay_Color{
        (float)color.red(),
        (float)color.green(),
        (float)color.blue(),
        (float)color.alpha(),
    };
}

Clay_Color clayRendererQRhiPaletteHighlightColor() {
    QColor color = QGuiApplication::palette().highlight().color();
    return Clay_Color{
        (float)color.red(),
        (float)color.green(),
        (float)color.blue(),
        (float)color.alpha(),
    };
}

Clay_Color clayRendererQRhiPaletteTextColor() {
    QColor color = QGuiApplication::palette().text().color();
    return Clay_Color{
        (float)color.red(),
        (float)color.green(),
        (float)color.blue(),
        (float)color.alpha(),
    };
}

Clay_Color clayRendererQRhiPaletteHiglightedTextColor() {
    QColor color = QGuiApplication::palette().highlightedText().color();
    return Clay_Color{
        (float)color.red(),
        (float)color.green(),
        (float)color.blue(),
        (float)color.alpha(),
    };
}

#ifdef __cplusplus
}
#endif

ClayFastQSGRenderNode::ClayFastQSGRenderNode(QQuickWindow *window) : m_window(window), m_textures(), m_texturesStale() {
    QFile file;
    file.setFileName(":/qt/qml/com/clay/renderers/QRhi/clayquad.vert.qsb");
    if (!file.open(QFile::ReadOnly)) {
        qFatal("Failed to load vertex shader");
    }
    m_shaders.append(QRhiShaderStage(QRhiShaderStage::Vertex, QShader::fromSerialized(file.readAll())));

    file.close();
    file.setFileName(":/qt/qml/com/clay/renderers/QRhi/clayquad.frag.qsb");
    if (!file.open(QFile::ReadOnly)) {
        qFatal("Failed to load fragment shader");
    }
    m_shaders.append(QRhiShaderStage(QRhiShaderStage::Fragment, QShader::fromSerialized(file.readAll())));

    m_textNode = window->createTextNode();
    appendChildNode(m_textNode);

    int maxInstances = CLAY_RENDERER_MAX_ELEMENTS;

    // create instance buffer big enough
    m_quads.capacity = maxInstances;
    m_quads.instData = (RectInstance *)malloc(sizeof(RectInstance) * m_quads.capacity);
    m_quads.count    = 0;
}

void ClayFastQSGRenderNode::setCmds(Clay_RenderCommandArray cmds, const QList<QFont> &fonts) {
    m_cmds = cmds;

    m_textNode->clear();
    for (int i = 0; i < m_cmds.length; i++) {
        Clay_RenderCommand *cmd = Clay_RenderCommandArray_Get(&m_cmds, i);
        if (cmd->commandType == CLAY_RENDER_COMMAND_TYPE_TEXT) {
            Clay_StringSlice contents = cmd->renderData.text.stringContents;
            QTextLayout      textLayout(QString::fromUtf8(contents.chars, contents.length));
            textLayout.setFont(fonts[cmd->renderData.text.fontId]);
            textLayout.beginLayout();
            textLayout.createLine();
            textLayout.endLayout();

            Clay_Color textColor = cmd->renderData.text.textColor;
            m_textNode->setColor(QColor(textColor.r, textColor.g, textColor.b, textColor.a));

            m_textNode->addTextLayout(QPointF(cmd->boundingBox.x, cmd->boundingBox.y), &textLayout);
        }
    }

    markDirty(QSGNode::DirtyGeometry);
}

void ClayFastQSGRenderNode::setTextures(QRhiTexture *textures[CLAY_RENDERER_QRHI_TEX_COUNT]) {
    m_textures      = textures;
    m_texturesStale = true;
}

void ClayFastQSGRenderNode::releaseResources() {
    m_vertexBuffer.reset();
    m_quadInstanceBuffer.reset();
    m_uniformBuffer.reset();
    m_samplers.clear();
    m_pipeline.reset();
    m_pipelineResourceBindings.reset();
    m_resourceBindings.reset();
}

QSGRenderNode::RenderingFlags ClayFastQSGRenderNode::flags() const {
    // We are rendering 2D content directly into the scene graph using QRhi, no
    // direct usage of a 3D API. Hence NoExternalRendering. This is a minor
    // optimization.

    // Additionally, the node takes the item transform into account by relying
    // on projectionMatrix() and matrix() (see prepare()) and never rendering at
    // other Z coordinates. Hence DepthAwareRendering. This is a potentially
    // bigger optimization.

    return QSGRenderNode::NoExternalRendering | QSGRenderNode::DepthAwareRendering;
}

QSGRenderNode::StateFlags ClayFastQSGRenderNode::changedStates() const {
    // In Qt 6 only ViewportState and ScissorState matter, the rest is ignored.
    return QSGRenderNode::StateFlag::ViewportState | QSGRenderNode::StateFlag::CullState;
}

std::unique_ptr<QRhiGraphicsPipeline> ClayFastQSGRenderNode::configurePipeline() {
    QRhi *rhi = m_window->rhi();

    auto pipeline = std::unique_ptr<QRhiGraphicsPipeline>(rhi->newGraphicsPipeline());

    // If layer.enabled == true on our QQuickItem and layer.textureMirroring is the
    // default MirrorVertically, the winding order needs to be inverted when
    // isYUpInFrameBuffer == true (OpenGL). This does not happen with other backends,
    // unless textureMirroring is changed so that the situation is inverted, meaning
    // GL needs no adjustments while others need the opposite winding order. Here we
    // choose the replicate, to a degree, what the scenegraph renderer does, but note that
    // this is only correct as long as textureMirroring is not changed from the default.
    pipeline->setFrontFace(
        renderTarget()->resourceType() == QRhiResource::TextureRenderTarget && rhi->isYUpInFramebuffer()
            ? QRhiGraphicsPipeline::CW
            : QRhiGraphicsPipeline::CCW);
    pipeline->setCullMode(QRhiGraphicsPipeline::Back);
    pipeline->setTopology(QRhiGraphicsPipeline::TriangleStrip);
    QRhiGraphicsPipeline::TargetBlend blend;
    blend.enable = true;
    pipeline->setTargetBlends({blend});
    pipeline->setShaderResourceBindings(m_pipelineResourceBindings.get());
    pipeline->setShaderStages(m_shaders.cbegin(), m_shaders.cend());
    pipeline->setDepthTest(true);

    QRhiVertexInputLayout inputLayout;

    inputLayout.setBindings(
        {{2 * sizeof(float)}, // unit quad
         {sizeof(RectInstance), QRhiVertexInputBinding::PerInstance}});

    inputLayout.setAttributes({
        // unit quad
        {0, 0, QRhiVertexInputAttribute::Float2, 0},

        // instance data
        {1, 1, QRhiVertexInputAttribute::Float4, offsetof(RectInstance, x)},
        {1, 2, QRhiVertexInputAttribute::Float4, offsetof(RectInstance, u0)},
        {1, 3, QRhiVertexInputAttribute::Float4, offsetof(RectInstance, r)},
        {1, 4, QRhiVertexInputAttribute::Float4, offsetof(RectInstance, radiusTL)},
        {1, 5, QRhiVertexInputAttribute::Float4, offsetof(RectInstance, borderL)},
        {1, 6, QRhiVertexInputAttribute::Float, offsetof(RectInstance, texToUse)},
    });

    pipeline->setVertexInputLayout(inputLayout);
    pipeline->setRenderPassDescriptor(renderTarget()->renderPassDescriptor());

    return pipeline;
}

void ClayFastQSGRenderNode::prepare() {
    QRhi                    *rhi              = m_window->rhi();
    QSize                    renderTargetSize = renderTarget()->pixelSize();
    QRhiResourceUpdateBatch *resourceUpdates  = rhi->nextResourceUpdateBatch();

    // Turning Clay cmds into buffers to upload to the GPU through the QRhi API.

    for (int i = 0; i < m_cmds.length; i++) {
        Clay_RenderCommand *cmd         = Clay_RenderCommandArray_Get(&m_cmds, i);
        Clay_BoundingBox    boundingBox = (Clay_BoundingBox){
            .x      = roundf(cmd->boundingBox.x),
            .y      = roundf(cmd->boundingBox.y),
            .width  = roundf(cmd->boundingBox.width),
            .height = roundf(cmd->boundingBox.height),
        };

        switch (cmd->commandType) {
        case CLAY_RENDER_COMMAND_TYPE_TEXT: {
            // Did in the setCmds().
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
        case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
            Clay_RectangleRenderData *config = &cmd->renderData.rectangle;
            Clay_Color                c      = config->backgroundColor;

            // Convert to float 0..1
            float rf = c.r / 255.0f;
            float gf = c.g / 255.0f;
            float bf = c.b / 255.0f;
            float af = c.a / 255.0f;

            bool isImage = cmd->commandType == CLAY_RENDER_COMMAND_TYPE_IMAGE;

            // Ensure we don't overflow the capacity
            if (m_quads.count >= m_quads.capacity) {
                printf("Clay renderer: instance overflow!\n");
                break;
            }

            int           idx = m_quads.count;
            RectInstance *dst = &m_quads.instData[idx];
            dst->x            = boundingBox.x;
            dst->y            = boundingBox.y;
            dst->w            = boundingBox.width;
            dst->h            = boundingBox.height;

            if (isImage) {
                ImageConfig *imgConf = (ImageConfig *)cmd->renderData.image.imageData;
                dst->u0              = imgConf->u0;
                dst->v0              = imgConf->v0;
                dst->u1              = imgConf->u1;
                dst->v1              = imgConf->v1;
                dst->texToUse        = (float)imgConf->textureToUse;
            } else {
                dst->u0 = dst->v0 = 0.0f;
                dst->u1 = dst->v1 = 1.0f;
                dst->texToUse     = -1.0f; // This means no image, use albedo color
            }

            // colour
            dst->r = rf * af;
            dst->g = gf * af;
            dst->b = bf * af;
            dst->a = af;

            // corner radii
            Clay_CornerRadius r = config->cornerRadius;
            dst->radiusTL       = r.topLeft;
            dst->radiusTR       = r.topRight;
            dst->radiusBL       = r.bottomLeft;
            dst->radiusBR       = r.bottomRight;

            dst->borderT = 0.0f;
            dst->borderR = 0.0f;
            dst->borderB = 0.0f;
            dst->borderL = 0.0f;

            m_quads.count++;
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
            // Ignore.
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
            // Ignore.
            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_BORDER: {
            Clay_BorderRenderData *br = &cmd->renderData.border;

            float rf = br->color.r / 255.0f;
            float gf = br->color.g / 255.0f;
            float bf = br->color.b / 255.0f;
            float af = br->color.a / 255.0f;

            float x = boundingBox.x;
            float y = boundingBox.y;
            float w = boundingBox.width;
            float h = boundingBox.height;

            float top    = br->width.top;
            float bottom = br->width.bottom;
            float left   = br->width.left;
            float right  = br->width.right;

            int           idx = m_quads.count;
            RectInstance *dst = &m_quads.instData[idx];

            dst->x = x - left;
            dst->y = y - top;
            dst->w = w + right;
            dst->h = h + bottom;

            dst->borderB = bottom;
            dst->borderL = left;
            dst->borderT = top;
            dst->borderR = right;

            // Clay borders are inset, but adding support to outset borders
            // Is as easy as this + some minor changes in shader too
            bool CLAY_BORDERS_ARE_INSET = true;
            if (CLAY_BORDERS_ARE_INSET) {
                // Normal behaviour
                dst->x = x;
                dst->y = y;
                dst->w = w;
                dst->h = h;
            } else {
                // Hypotethical behaviour, if the borders were outside
                dst->x = x - left;
                dst->y = y - top;
                dst->w = w + left + right;
                dst->h = h + top + bottom;
            }

            dst->u0 = 0.0f;
            dst->v0 = 0.0f;
            dst->u1 = 1.0f;
            dst->v1 = 1.0f;

            dst->r = rf;
            dst->g = gf;
            dst->b = bf;
            dst->a = af;

            dst->radiusTL = br->cornerRadius.topLeft;
            dst->radiusTR = br->cornerRadius.topRight;
            dst->radiusBR = br->cornerRadius.bottomRight;
            dst->radiusBL = br->cornerRadius.bottomLeft;

            dst->texToUse = -1.0f;

            m_quads.count++;
            break;
        }

        case CLAY_RENDER_COMMAND_TYPE_CUSTOM: {
            // printf("Unhandled clay cmd: custom\n");
            break;
        }
        default: {
            printf("Error: unhandled render command\n");
            exit(1);
        }
        }
    }

    if (!m_quadInstanceBuffer || m_quadInstanceBuffer->size() < sizeof(RectInstance) * m_quads.capacity) {
        m_quadInstanceBuffer.reset(
            rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::VertexBuffer, sizeof(RectInstance) * m_quads.capacity));
        m_quadInstanceBuffer->create();
    }

    if (m_quads.count > 0) {
        resourceUpdates->updateDynamicBuffer(
            m_quadInstanceBuffer.get(), 0, m_quads.count * sizeof(RectInstance), m_quads.instData);
    }

    m_quadInstancesCount = m_quads.count;

    // Clear instance arrays, as they were flushed to their render buffers.
    m_quads.count = 0;

    if (!m_vertexBuffer) {
        // create unit quad VBO (0..1)
        const float quadVerts[8] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f};
        m_vertexBuffer.reset(rhi->newBuffer(QRhiBuffer::Immutable, QRhiBuffer::VertexBuffer, sizeof(quadVerts)));
        m_vertexBuffer->create();
        resourceUpdates->uploadStaticBuffer(m_vertexBuffer.get(), quadVerts);
    }

    if (!m_uniformBuffer) {
        m_uniformBuffer.reset(rhi->newBuffer(QRhiBuffer::Dynamic, QRhiBuffer::UniformBuffer, 68));
        m_uniformBuffer->create();
    }

    if (m_samplers.empty()) {
        for (size_t i = 0; i < CLAY_RENDERER_QRHI_TEX_COUNT; ++i) {
            m_samplers.emplace_back(rhi->newSampler(
                QRhiSampler::Linear,
                QRhiSampler::Linear,
                QRhiSampler::None,
                QRhiSampler::Repeat,
                QRhiSampler::Repeat,
                QRhiSampler::Repeat));
            if (!m_samplers[i]->create()) {
                qFatal("Failed to create a sampler");
            }
        }
    }

    if (!m_resourceBindings || m_texturesStale) {
        m_texturesStale = false;
        QList<QRhiShaderResourceBinding> bindings;
        bindings.push_back(
            QRhiShaderResourceBinding::uniformBuffer(
                0,
                QRhiShaderResourceBinding::VertexStage | QRhiShaderResourceBinding::FragmentStage,
                m_uniformBuffer.get()));

        if (m_textures) {
            for (size_t i = 0; i < CLAY_RENDERER_QRHI_TEX_COUNT; ++i) {
                if (m_textures[i]) {
                    bindings.push_back(
                        QRhiShaderResourceBinding::sampledTexture(
                            1 + i, QRhiShaderResourceBinding::FragmentStage, m_textures[i], m_samplers[i].get()));
                }
            }
        }

        m_resourceBindings.reset(rhi->newShaderResourceBindings());
        m_resourceBindings->setBindings(bindings.begin(), bindings.end());
        m_resourceBindings->create();
    }

    if (!m_pipelineResourceBindings) {
        m_pipelineResourceBindings.reset(rhi->newShaderResourceBindings());
        m_pipelineResourceBindings->setBindings({
            QRhiShaderResourceBinding::uniformBuffer(
                0, QRhiShaderResourceBinding::VertexStage | QRhiShaderResourceBinding::FragmentStage, nullptr),
            QRhiShaderResourceBinding::sampledTexture(
                1, QRhiShaderResourceBinding::FragmentStage, nullptr, m_samplers[0].get()),
        });
        m_pipelineResourceBindings->create();
    }

    if (!m_pipeline) {
        m_pipeline = configurePipeline();
        m_pipeline->create();
    }

    const QMatrix4x4 mvp     = *projectionMatrix() * *matrix();
    const float      opacity = inheritedOpacity();

    resourceUpdates->updateDynamicBuffer(m_uniformBuffer.get(), 0, 64, mvp.constData());
    resourceUpdates->updateDynamicBuffer(m_uniformBuffer.get(), 64, 4, &opacity);
    commandBuffer()->resourceUpdate(resourceUpdates);
}

void ClayFastQSGRenderNode::render(const RenderState *) {
    QSize renderTargetSize = renderTarget()->pixelSize();

    QRhiCommandBuffer *cb = commandBuffer();

    cb->setViewport(QRhiViewport(0, 0, renderTargetSize.width(), renderTargetSize.height()));

    cb->setGraphicsPipeline(m_pipeline.get());
    cb->setShaderResources(m_resourceBindings.get());

    QRhiCommandBuffer::VertexInput bindings[] = {
        {m_vertexBuffer.get(), 0},      // unit quad (0..1)
        {m_quadInstanceBuffer.get(), 0} // instances
    };

    cb->setVertexInput(0, 2, bindings);
    cb->draw(4, m_quadInstancesCount);

    m_textNode->setViewport(QRect(0, 0, renderTargetSize.width(), renderTargetSize.height()));
}

#endif

#endif

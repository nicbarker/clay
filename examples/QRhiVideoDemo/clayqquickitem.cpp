#include "clayqquickitem.h"

static inline void clayErrorHandler(Clay_ErrorData errorData) {
    qCritical() << "Clay error:" << QString::fromUtf8(errorData.errorText.chars, errorData.errorText.length);
}

ClayQQuickItem::ClayQQuickItem(QQuickItem *parent) : QQuickItem(parent), m_textures(), m_clayImageConfigs() {
    setFlag(ItemHasContents, true);

    size_t clayRequiredMemory = Clay_MinMemorySize();
    m_clayMemory              = (Clay_Arena){
        .capacity = clayRequiredMemory,
        .memory   = (char *)malloc(clayRequiredMemory),
    };
    m_clayCtx = Clay_Initialize(
        m_clayMemory,
        Clay_Dimensions{},
        Clay_ErrorHandler{
            .errorHandlerFunction = clayErrorHandler,
        });
}

QSGNode *ClayQQuickItem::updatePaintNode(QSGNode *old, UpdatePaintNodeData *) {
    ClayFastQSGRenderNode *node        = static_cast<ClayFastQSGRenderNode *>(old);
    bool                   nodeCreated = false;

    if (!node) {
        node        = new ClayFastQSGRenderNode(window());
        nodeCreated = true;
    }

    // Note that MeasureText has to be set after the Context is set!
    Clay_SetCurrentContext(m_clayCtx);
    Clay_SetMeasureTextFunction(clayRendererQRhiMeasureText, &m_fonts);

    Clay_SetLayoutDimensions((Clay_Dimensions){(float)width(), (float)height()});

    bool texturesStale;

    node->setCmds(doLayout(m_textures, &texturesStale), *m_fonts);

    if (nodeCreated || texturesStale) {
        node->setTextures(m_textures);
    }

    return node;
}

void ClayQQuickItem::setFontsPtr(QList<QFont> *fonts) {
    m_fonts = fonts;
}

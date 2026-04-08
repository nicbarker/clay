#pragma once

#ifdef __cplusplus
#include <QQuickItem>
#endif

#include "clay_renderer_qrhi.h"

typedef struct ImageConfig ImageConfig;

typedef struct ImageConfigs {
    size_t       capacity;
    size_t       count;
    ImageConfig *cfgs;
} ImageConfigs;

#ifdef __cplusplus
class ClayQQuickItem : public QQuickItem {
    Q_OBJECT

  public:
    explicit ClayQQuickItem(QQuickItem *parent = nullptr);

  protected:
    QSGNode *updatePaintNode(QSGNode *old, UpdatePaintNodeData *) override;

    void setFontsPtr(QList<QFont> *fonts);

    virtual Clay_RenderCommandArray
    doLayout(QRhiTexture *textures[CLAY_RENDERER_QRHI_TEX_COUNT], bool *texturesStale) = 0;

    QRhiTexture *m_textures[CLAY_RENDERER_QRHI_TEX_COUNT];

    ImageConfigs m_clayImageConfigs;

    Clay_Arena    m_clayMemory;
    Clay_Context *m_clayCtx;
    QList<QFont> *m_fonts;
};
#endif

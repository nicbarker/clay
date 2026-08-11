#pragma once

#include <memory>

#include "clayqquickitem.h"

class VideoDemoItem : public ClayQQuickItem {
    Q_OBJECT
    QML_ELEMENT

  public:
    explicit VideoDemoItem(QQuickItem *parent = nullptr);

  protected:
    Clay_RenderCommandArray doLayout(QRhiTexture *textures[CLAY_RENDERER_QRHI_TEX_COUNT], bool *texturesStale) override;

    std::unique_ptr<void, void (*)(void*)> m_data;

    QList<QFont> m_fonts = QList<QFont>{QFont()};
};

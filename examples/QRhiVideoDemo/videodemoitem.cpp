#include "videodemoitem.h"

#include <memory.h>

extern "C" {
#include "../shared-layouts/clay-video-demo.c"
}

VideoDemoItem::VideoDemoItem(QQuickItem *parent) : ClayQQuickItem(parent), m_data(malloc(sizeof(ClayVideoDemo_Data)), free) {
    *(ClayVideoDemo_Data*)m_data.get() = ClayVideoDemo_Initialize();
    setFontsPtr(&m_fonts);
}

Clay_RenderCommandArray VideoDemoItem::doLayout(QRhiTexture *textures[CLAY_RENDERER_QRHI_TEX_COUNT], bool *texturesStale) {
    *texturesStale = false;
    return ClayVideoDemo_CreateLayout((ClayVideoDemo_Data*)m_data.get());
}

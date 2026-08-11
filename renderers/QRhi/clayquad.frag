#version 440
precision mediump float;
layout(location = 0) in vec2 vPos;
layout(location = 1) in vec4 vRect;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec4 vColor;
layout(location = 4) in vec4 vCornerRadii;
layout(location = 5) in vec4 vBorderWidths;
layout(location = 6) in float vTexSlot;
layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
};
layout(binding = 1) uniform sampler2D uTex0;
layout(binding = 2) uniform sampler2D uTex1;
layout(binding = 3) uniform sampler2D uTex2;
layout(binding = 4) uniform sampler2D uTex3;
layout(binding = 5) uniform sampler2D uTex4;
layout(binding = 6) uniform sampler2D uTex5;
layout(binding = 7) uniform sampler2D uTex6;
layout(binding = 8) uniform sampler2D uTex7;
layout(binding = 9) uniform sampler2D uTex8;
layout(binding = 10) uniform sampler2D uTex9;
layout(binding = 11) uniform sampler2D uTex10;
layout(binding = 12) uniform sampler2D uTex11;
layout(binding = 13) uniform sampler2D uTex12;
layout(binding = 14) uniform sampler2D uTex13;
layout(binding = 15) uniform sampler2D uTex14;
layout(binding = 16) uniform sampler2D uTex15;
layout(location = 0) out vec4 frag;
void main() {
    // Pixel coordinates in pixel space
    vec2 pix = vRect.xy + vPos * vRect.zw;
    float x0 = vRect.x;
    float y0 = vRect.y;
    float w  = vRect.z;
    float h  = vRect.w;
    // Local position inside the rectangle (0..w, 0..h)
    vec2 local = pix - vec2(x0, y0);
    // Original corner radii
    float tl = vCornerRadii.x;
    float tr = vCornerRadii.y;
    float bl = vCornerRadii.z;
    float br = vCornerRadii.w;
    // Border thicknesses
    float L = vBorderWidths.x;
    float R = vBorderWidths.y;
    float T = vBorderWidths.z;
    float B = vBorderWidths.w;
    bool CLAY_BORDERS_ARE_INSET = true; // it is true
    bool isBorder = (L > 0.0 || R > 0.0 || T > 0.0 || B > 0.0);
    float outerAlpha = 1.0;
    // If it is not a border but rect or image, then it only has outer border what is provided
    // Otherwise it increases the outter border, but the provided borde is the ineer border
    // I think is better not increase rounding radius when that radius is smaller than border thickness
    float outter_tl;
    float outter_tr;
    float outter_bl;
    float outter_br;
    if (CLAY_BORDERS_ARE_INSET) {
        // Actural behaviour
        outter_tl = tl;
        outter_tr = tr;
        outter_bl = bl;
        outter_br = br;
        tl = (tl > min(T, L)) ? tl - min(T, L) : tl;
        tr = (tr > min(T, R)) ? tr - min(T, R) : tr;
        bl = (bl > min(B, L)) ? bl - min(B, L) : bl;
        br = (br > min(B, R)) ? br - min(B, R) : br;
    } else {
        // Hypothetical behaviour
        outter_tl = (tl > min(T, L)) ? tl + min(T, L) : tl;
        outter_tr = (tr > min(T, R)) ? tr + min(T, R) : tr;
        outter_bl = (bl > min(B, L)) ? bl + min(B, L) : bl;
        outter_br = (br > min(B, R)) ? br + min(B, R) : br;
    }
    if (outter_tl > 0.0 && local.x < outter_tl && local.y < outter_tl)
        outerAlpha = step(length(local - vec2(outter_tl, outter_tl)), outter_tl);
    if (outter_tr > 0.0 && local.x > w - outter_tr && local.y < outter_tr)
        outerAlpha *= step(length(local - vec2(w - outter_tr, outter_tr)), outter_tr);
    if (outter_bl > 0.0 && local.x < outter_bl && local.y > h - outter_bl)
        outerAlpha *= step(length(local - vec2(outter_bl, h - outter_bl)), outter_bl);
    if (outter_br > 0.0 && local.x > w - outter_br && local.y > h - outter_br)
        outerAlpha *= step(length(local - vec2(w - outter_br, h - outter_br)), outter_br);
    if (outerAlpha < 0.5)
        discard;
    // -------- Border logic --------
    if (isBorder) {
        float iw = w - L - R;
        float ih = h - T - B;
        vec2 innerLocal = local - vec2(L, T);
        // Check if pixel is inside inner rounded rect
        bool insideInner = true;
        if (tl > 0.0 && innerLocal.x < tl && innerLocal.y < tl)
            insideInner = (length(innerLocal - vec2(tl, tl)) <= tl);
        if (tr > 0.0 && innerLocal.x > iw - tr && innerLocal.y < tr)
            insideInner = insideInner && (length(innerLocal - vec2(iw - tr, tr)) <= tr);
        // Bottom-left
        if (bl> 0.0 && innerLocal.x < bl && innerLocal.y > ih - bl)
            insideInner = insideInner && (length(innerLocal - vec2(bl, ih - bl)) <= bl);
        // Bottom-right
        if (br > 0.0 && innerLocal.x > iw - br && innerLocal.y > ih - br)
            insideInner = insideInner && (length(innerLocal - vec2(iw - br, ih - br)) <= br);
        // Discard pixels inside inner rounded rect
        if (insideInner && innerLocal.x >= 0.0 && innerLocal.x <= iw && innerLocal.y >= 0.0 && innerLocal.y < ih)
            discard;
        frag = vColor * qt_Opacity;
        return;
    }
    // -------- Non-border rectangle or image --------
    if (vTexSlot < 0.0) {
        frag = vColor * qt_Opacity;
    } else {
        int slot = int(vTexSlot + 0.5);
        if (slot == 0) frag = texture(uTex0, vUV) * qt_Opacity;
        if (slot == 1) frag = texture(uTex1, vUV) * qt_Opacity;
        if (slot == 2) frag = texture(uTex2, vUV) * qt_Opacity;
        if (slot == 3) frag = texture(uTex3, vUV) * qt_Opacity;
        if (slot == 4) frag = texture(uTex4, vUV) * qt_Opacity;
        if (slot == 5) frag = texture(uTex5, vUV) * qt_Opacity;
        if (slot == 6) frag = texture(uTex6, vUV) * qt_Opacity;
        if (slot == 7) frag = texture(uTex7, vUV) * qt_Opacity;
        if (slot == 8) frag = texture(uTex8, vUV) * qt_Opacity;
        if (slot == 9) frag = texture(uTex9, vUV) * qt_Opacity;
        if (slot == 10) frag = texture(uTex10, vUV) * qt_Opacity;
        if (slot == 11) frag = texture(uTex11, vUV) * qt_Opacity;
        if (slot == 12) frag = texture(uTex12, vUV) * qt_Opacity;
        if (slot == 13) frag = texture(uTex13, vUV) * qt_Opacity;
        if (slot == 14) frag = texture(uTex14, vUV) * qt_Opacity;
        if (slot == 15) frag = texture(uTex15, vUV) * qt_Opacity;
    }
}

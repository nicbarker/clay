#version 440
precision mediump float;
layout(location = 0) in vec2 aPos;        // unit quad (0..1)
layout(location = 1) in vec4 aRect;       // x,y,w,h (pixels)
layout(location = 2) in vec4 aUV;         // u0,v0,u1,v1
layout(location = 3) in vec4 aColor;      // rgba
layout(location = 4) in vec4 aCornerRadii;
layout(location = 5) in vec4 aBorderWidths;
layout(location = 6) in float aTexSlot;
layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
};
layout(location = 0) out vec2 vPos;
layout(location = 1) out vec4 vRect;
layout(location = 2) out vec2 vUV;
layout(location = 3) out vec4 vColor;
layout(location = 4) out vec4 vCornerRadii;
layout(location = 5) out vec4 vBorderWidths;
layout(location = 6) out float vTexSlot;
void main() {
    vec2 pos = vec2(aPos.x * aRect.z + aRect.x, aPos.y * aRect.w + aRect.y);
    gl_Position = qt_Matrix * vec4(pos, 0.0, 1.0);
    vPos = aPos;
    vRect = aRect;
    vColor = aColor;
    vUV = mix(aUV.xy, aUV.zw, aPos);
    vCornerRadii = aCornerRadii;
    vBorderWidths = aBorderWidths;
    vTexSlot = aTexSlot;
}

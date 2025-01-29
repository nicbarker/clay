module raylib @if($feature(NO_STDLIB));

distinct ZString = inline char*;    

module clay::renderer;

import raylib;

// TODO: this entire file was very rushed so it can probably be cleaned up a LOT
fn double calculate_sine(double x) {
    double term = x; 
    double sum = term;
    double n = 1;

    for (int i = 1; i <= 10; i++) { 
        term *= (-1) * x * x / ((2 * n) * (2 * n + 1));
        sum += term;
        n++;
    }

    return sum;
}

fn double calculate_cosine(double x) {
    double term = 1;
    double sum = term;
    double n = 1;

    for (int i = 1; i <= 10; i++) {
        term *= (-1) * x * x / ((2 * n - 1) * (2 * n));
        sum += term;
        n++;
    }

    return sum;
}

macro double calculate_tangent(double x) {
    double sine = calculate_sine(x);
    double cosine = calculate_cosine(x);
    // Check for cases where cosine is zero (tangent is undefined)
    if (cosine == 0.0) {
        return 0.0;
    }
    return sine / cosine;
}


fn Color clayToRaylibColor(ClayColor color) @inline
{
    return { (char)$$round(color.r), (char)$$round(color.g), (char)$$round(color.b), (char)$$round(color.a)};
}

struct RaylibFont
{
    int fontId;
    raylib::Font font;
}

RaylibFont[10] raylibFonts;
raylib::Camera raylibCamera;

const Matrix MATRIX_IDENTITY = { 
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1,
};

enum CustomLayoutElementType
{
    MODEL_3D
}

struct Model3DLayoutElement
{
    raylib::Model model;
    float scale;
    raylib::Vector3 position;
    raylib::Matrix rotation;
}

struct CustomLayoutElement
{
    CustomLayoutElementType type;
    union element
    {
        Model3DLayoutElement model;
    }
}

fn Matrix matrixPerspective(double fovY, double aspect, double nearPlane, double farPlane)
{
    Matrix result = {   0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0 };

    double top = nearPlane * calculate_tangent(fovY*0.5*raylib::RAD2DEG);
    double bottom = -top;
    double right = top*aspect;
    double left = -right;

    // MatrixFrustum(-right, right, -top, top, near, far);
    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float far_near = (float)(farPlane - nearPlane);

    result.m0 = ((float)nearPlane*2.0f)/rl;
    result.m5 = ((float)nearPlane*2.0f)/tb;
    result.m8 = ((float)right + (float)left)/rl;
    result.m9 = ((float)top + (float)bottom)/tb;
    result.m10 = -((float)farPlane + (float)nearPlane)/far_near;
    result.m11 = -1.0f;
    result.m14 = -((float)farPlane*(float)nearPlane*2.0f)/far_near;

    return result;
}

fn Vector3 vector3Unproject(Vector3 source, Matrix projection, Matrix view)
{
    Vector3 result = { 0, 0, 0 };

    // Calculate unprojected matrix (multiply view matrix by projection matrix) and invert it
    Matrix matViewProj = {      // MatrixMultiply(view, projection);
        view.m0*projection.m0 + view.m1*projection.m4 + view.m2*projection.m8 + view.m3*projection.m12,
        view.m0*projection.m1 + view.m1*projection.m5 + view.m2*projection.m9 + view.m3*projection.m13,
        view.m0*projection.m2 + view.m1*projection.m6 + view.m2*projection.m10 + view.m3*projection.m14,
        view.m0*projection.m3 + view.m1*projection.m7 + view.m2*projection.m11 + view.m3*projection.m15,
        view.m4*projection.m0 + view.m5*projection.m4 + view.m6*projection.m8 + view.m7*projection.m12,
        view.m4*projection.m1 + view.m5*projection.m5 + view.m6*projection.m9 + view.m7*projection.m13,
        view.m4*projection.m2 + view.m5*projection.m6 + view.m6*projection.m10 + view.m7*projection.m14,
        view.m4*projection.m3 + view.m5*projection.m7 + view.m6*projection.m11 + view.m7*projection.m15,
        view.m8*projection.m0 + view.m9*projection.m4 + view.m10*projection.m8 + view.m11*projection.m12,
        view.m8*projection.m1 + view.m9*projection.m5 + view.m10*projection.m9 + view.m11*projection.m13,
        view.m8*projection.m2 + view.m9*projection.m6 + view.m10*projection.m10 + view.m11*projection.m14,
        view.m8*projection.m3 + view.m9*projection.m7 + view.m10*projection.m11 + view.m11*projection.m15,
        view.m12*projection.m0 + view.m13*projection.m4 + view.m14*projection.m8 + view.m15*projection.m12,
        view.m12*projection.m1 + view.m13*projection.m5 + view.m14*projection.m9 + view.m15*projection.m13,
        view.m12*projection.m2 + view.m13*projection.m6 + view.m14*projection.m10 + view.m15*projection.m14,
        view.m12*projection.m3 + view.m13*projection.m7 + view.m14*projection.m11 + view.m15*projection.m15 };

    // Calculate inverted matrix . MatrixInvert(matViewProj);
    // Cache the matrix values (speed optimization)
    float a00 = matViewProj.m0;
    float a01 = matViewProj.m1;
    float a02 = matViewProj.m2;
    float a03 = matViewProj.m3;
    float a10 = matViewProj.m4;
    float a11 = matViewProj.m5;
    float a12 = matViewProj.m6;
    float a13 = matViewProj.m7;
    float a20 = matViewProj.m8;
    float a21 = matViewProj.m9;
    float a22 = matViewProj.m10;
    float a23 = matViewProj.m11;
    float a30 = matViewProj.m12;
    float a31 = matViewProj.m13;
    float a32 = matViewProj.m14;
    float a33 = matViewProj.m15;

    float b00 = a00*a11 - a01*a10;
    float b01 = a00*a12 - a02*a10;
    float b02 = a00*a13 - a03*a10;
    float b03 = a01*a12 - a02*a11;
    float b04 = a01*a13 - a03*a11;
    float b05 = a02*a13 - a03*a12;
    float b06 = a20*a31 - a21*a30;
    float b07 = a20*a32 - a22*a30;
    float b08 = a20*a33 - a23*a30;
    float b09 = a21*a32 - a22*a31;
    float b10 = a21*a33 - a23*a31;
    float b11 = a22*a33 - a23*a32;

    // Calculate the invert determinant (inlined to avoid double-caching)
    float invDet = 1.0f/(b00*b11 - b01*b10 + b02*b09 + b03*b08 - b04*b07 + b05*b06);

    Matrix matViewProjInv = {
        (a11*b11 - a12*b10 + a13*b09)*invDet,
        (-a01*b11 + a02*b10 - a03*b09)*invDet,
        (a31*b05 - a32*b04 + a33*b03)*invDet,
        (-a21*b05 + a22*b04 - a23*b03)*invDet,
        (-a10*b11 + a12*b08 - a13*b07)*invDet,
        (a00*b11 - a02*b08 + a03*b07)*invDet,
        (-a30*b05 + a32*b02 - a33*b01)*invDet,
        (a20*b05 - a22*b02 + a23*b01)*invDet,
        (a10*b10 - a11*b08 + a13*b06)*invDet,
        (-a00*b10 + a01*b08 - a03*b06)*invDet,
        (a30*b04 - a31*b02 + a33*b00)*invDet,
        (-a20*b04 + a21*b02 - a23*b00)*invDet,
        (-a10*b09 + a11*b07 - a12*b06)*invDet,
        (a00*b09 - a01*b07 + a02*b06)*invDet,
        (-a30*b03 + a31*b01 - a32*b00)*invDet,
        (a20*b03 - a21*b01 + a22*b00)*invDet };

    // Create quaternion from source point
    raylib::Quaternion quat = { source.x, source.y, source.z, 1.0f };

    // Multiply quat point by unprojecte matrix
    raylib::Quaternion qtransformed = {     // QuaternionTransform(quat, matViewProjInv)
        matViewProjInv.m0*quat.x + matViewProjInv.m4*quat.y + matViewProjInv.m8*quat.z + matViewProjInv.m12*quat.w,
        matViewProjInv.m1*quat.x + matViewProjInv.m5*quat.y + matViewProjInv.m9*quat.z + matViewProjInv.m13*quat.w,
        matViewProjInv.m2*quat.x + matViewProjInv.m6*quat.y + matViewProjInv.m10*quat.z + matViewProjInv.m14*quat.w,
        matViewProjInv.m3*quat.x + matViewProjInv.m7*quat.y + matViewProjInv.m11*quat.z + matViewProjInv.m15*quat.w };

    // Normalized world points in vectors
    result.x = qtransformed.x/qtransformed.w;
    result.y = qtransformed.y/qtransformed.w;
    result.z = qtransformed.z/qtransformed.w;

    return result;
}

fn Matrix matrixOrtho(double left, double right, double bottom, double top, double nearPlane, double farPlane)
{
    Matrix result = {   0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0 };

    float rl = (float)(right - left);
    float tb = (float)(top - bottom);
    float far_near = (float)(farPlane - nearPlane);

    result.m0 = 2.0f/rl;
    result.m1 = 0.0f;
    result.m2 = 0.0f;
    result.m3 = 0.0f;
    result.m4 = 0.0f;
    result.m5 = 2.0f/tb;
    result.m6 = 0.0f;
    result.m7 = 0.0f;
    result.m8 = 0.0f;
    result.m9 = 0.0f;
    result.m10 = -2.0f/far_near;
    result.m11 = 0.0f;
    result.m12 = -((float)left + (float)right)/rl;
    result.m13 = -((float)top + (float)bottom)/tb;
    result.m14 = -((float)farPlane + (float)nearPlane)/far_near;
    result.m15 = 1.0f;

    return result;
}

fn Vector3 vector3Normalize(Vector3 v)
{
    Vector3 result = v;

    float length = $$sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    if (length != 0.0f)
    {
        float ilength = 1.0f/length;

        result.x *= ilength;
        result.y *= ilength;
        result.z *= ilength;
    }

    return result;
}

fn Vector3 vector3Subtract(Vector3 v1, Vector3 v2)
{
    Vector3 result = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };

    return result;
}


fn Ray getScreenToWorldPointWithZDistance(Vector2 position, Camera camera, int screenWidth, int screenHeight, float zDistance)
{
    Ray ray = { {0,0,0}, {0,0,0} };
    float x = (2.0f*position.x)/(float)screenWidth - 1.0f;
    float y = 1.0f - (2.0f*position.y)/(float)screenHeight;
    float z = 1.0f;

    // Store values in a vector
    Vector3 deviceCoords = { x, y, z };

    // Calculate view matrix from camera look at
    Matrix matView = raylib::getCameraMatrix(camera);

    Matrix matProj = MATRIX_IDENTITY;

    if (camera.projection == CameraProjection.PERSPECTIVE)
    {
        // Calculate projection matrix from perspective
        matProj = matrixPerspective((double)(camera.fovy*raylib::DEG2RAD), ((double)screenWidth/(double)screenHeight), 0.01f, zDistance);
    }
    else if (camera.projection == CameraProjection.ORTHOGRAPHIC)
    {
        double aspect = (double)screenWidth/(double)screenHeight;
        double top = camera.fovy/2.0;
        double right = top*aspect;

        // Calculate projection matrix from orthographic
        matProj = matrixOrtho(-right, right, -top, top, 0.01, 1000.0);
    }

    // Unproject far/near points
    Vector3 nearPoint = vector3Unproject({ deviceCoords.x, deviceCoords.y, 0.0f }, matProj, matView);
    Vector3 farPoint = vector3Unproject({ deviceCoords.x, deviceCoords.y, 1.0f }, matProj, matView);

    // Calculate normalized direction vector
    Vector3 direction = vector3Normalize(vector3Subtract(farPoint, nearPoint));

    ray.position = farPoint;

    // Apply calculated vectors to ray
    ray.direction = direction;

    return ray;
}

fn Dimensions raylibMeasureText(ClayString *text, TextElementConfig *config)
{
    // Measure string size for Font
    Dimensions textSize = { 0, 0 };

    float maxTextWidth = 0.0f;
    float lineTextWidth = 0;

    float textHeight = config.fontSize;
    Font fontToUse = raylibFonts[config.fontId].font;
    float scaleFactor = config.fontSize/(float)fontToUse.baseSize;

    for (int i = 0; i < text.length; ++i)
    {
        if (text.chars[i] == '\n') {
            maxTextWidth = $$max(maxTextWidth, lineTextWidth);
            lineTextWidth = 0;
            continue;
        }
        int index = text.chars[i] - 32;
        if (fontToUse.glyphs[index].advanceX != 0) {lineTextWidth += fontToUse.glyphs[index].advanceX;}
        else {lineTextWidth += (fontToUse.recs[index].width + fontToUse.glyphs[index].offsetX);}
    }

    maxTextWidth = $$max(maxTextWidth, lineTextWidth);

    textSize.width = maxTextWidth * scaleFactor;
    textSize.height = textHeight;

    return textSize;
}

fn void raylibRender(RenderCommandArray renderCommands)
{
    for (int j = 0; j < renderCommands.length; j++)
    {
        RenderCommand *renderCommand = renderCommands.get(j);
        ClayBoundingBox boundingBox = renderCommand.boundingBox;
        switch (renderCommand.commandType)
        {
            case RenderCommandType.TEXT: {
                // Raylib uses standard C strings so isn't compatible with cheap slices, we need to clone the string to append null terminator
                ClayString text = renderCommand.text;
                ZString cloned = (ZString)malloc((ulong)text.length + 1);
                $$memcpy(cloned, text.chars, (isz)text.length, false, (isz)0, (isz)0);
                cloned[text.length] = '\0';
                Font fontToUse = raylibFonts[renderCommand.config.textElementConfig.fontId].font;
                raylib::drawTextEx(fontToUse, cloned, {boundingBox.x, boundingBox.y}, (float)renderCommand.config.textElementConfig.fontSize, (float)renderCommand.config.textElementConfig.letterSpacing, clayToRaylibColor(renderCommand.config.textElementConfig.textColor));
                free(cloned);
                break;
            }
            case RenderCommandType.IMAGE: {
                Texture2D imageTexture = *(Texture2D *)renderCommand.config.imageElementConfig.imageData;
                raylib::drawTextureEx(
                imageTexture,
                {boundingBox.x, boundingBox.y},
                0,
                boundingBox.width / (float)imageTexture.width,
                raylib::WHITE);
                break;
            }
            case RenderCommandType.SCISSOR_START: {
                raylib::beginScissorMode((int)$$round(boundingBox.x), (int)$$round(boundingBox.y), (int)$$round(boundingBox.width), (int)$$round(boundingBox.height));
                break;
            }
            case RenderCommandType.SCISSOR_END: {
                raylib::endScissorMode();
                break;
            }
            case RenderCommandType.RECTANGLE: {
                RectangleElementConfig *config = renderCommand.config.rectangleElementConfig;
                if (config.cornerRadius.topLeft > 0) {
                    float radius = (config.cornerRadius.topLeft * 2) / (float)(boundingBox.width > boundingBox.height ? boundingBox.height : boundingBox.width);
                    raylib::drawRectangleRounded({ boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height }, radius, 8, clayToRaylibColor(config.color));
                } else {
                    raylib::drawRectangle((int)$$round(boundingBox.x), (int)$$round(boundingBox.y), (int)$$round(boundingBox.width), (int)$$round(boundingBox.height), clayToRaylibColor(config.color));
                }
                break;
            }
            case RenderCommandType.BORDER: {
                BorderElementConfig *config = renderCommand.config.borderElementConfig;
                // Left border
                if (config.left.width > 0) {
                    raylib::drawRectangle((int)$$round(boundingBox.x), (int)$$round(boundingBox.y + config.cornerRadius.topLeft), (int)config.left.width, (int)$$round(boundingBox.height - config.cornerRadius.topLeft - config.cornerRadius.bottomLeft), clayToRaylibColor(config.left.color));
                }
                // Right border
                if (config.right.width > 0) {
                    raylib::drawRectangle((int)$$round(boundingBox.x + boundingBox.width - config.right.width), (int)$$round(boundingBox.y + config.cornerRadius.topRight), (int)config.right.width, (int)$$round(boundingBox.height - config.cornerRadius.topRight - config.cornerRadius.bottomRight), clayToRaylibColor(config.right.color));
                }
                // Top border
                if (config.top.width > 0) {
                    raylib::drawRectangle((int)$$round(boundingBox.x + config.cornerRadius.topLeft), (int)$$round(boundingBox.y), (int)$$round(boundingBox.width - config.cornerRadius.topLeft - config.cornerRadius.topRight), (int)config.top.width, clayToRaylibColor(config.top.color));
                }
                // Bottom border
                if (config.bottom.width > 0) {
                    raylib::drawRectangle((int)$$round(boundingBox.x + config.cornerRadius.bottomLeft), (int)$$round(boundingBox.y + boundingBox.height - config.bottom.width), (int)$$round(boundingBox.width - config.cornerRadius.bottomLeft - config.cornerRadius.bottomRight), (int)config.bottom.width, clayToRaylibColor(config.bottom.color));
                }
                if (config.cornerRadius.topLeft > 0) {
                    raylib::drawRing({ $$round(boundingBox.x + config.cornerRadius.topLeft), $$round(boundingBox.y + config.cornerRadius.topLeft) }, $$round(config.cornerRadius.topLeft - config.top.width), config.cornerRadius.topLeft, 180, 270, 10, clayToRaylibColor(config.top.color));
                }
                if (config.cornerRadius.topRight > 0) {
                    raylib::drawRing({ $$round(boundingBox.x + boundingBox.width - config.cornerRadius.topRight), $$round(boundingBox.y + config.cornerRadius.topRight) }, $$round(config.cornerRadius.topRight - config.top.width), config.cornerRadius.topRight, 270, 360, 10, clayToRaylibColor(config.top.color));
                }
                if (config.cornerRadius.bottomLeft > 0) {
                    raylib::drawRing({ $$round(boundingBox.x + config.cornerRadius.bottomLeft), $$round(boundingBox.y + boundingBox.height - config.cornerRadius.bottomLeft) }, $$round(config.cornerRadius.bottomLeft - config.top.width), config.cornerRadius.bottomLeft, 90, 180, 10, clayToRaylibColor(config.bottom.color));
                }
                if (config.cornerRadius.bottomRight > 0) {
                    raylib::drawRing({ $$round(boundingBox.x + boundingBox.width - config.cornerRadius.bottomRight), $$round(boundingBox.y + boundingBox.height - config.cornerRadius.bottomRight) }, $$round(config.cornerRadius.bottomRight - config.bottom.width), config.cornerRadius.bottomRight, 0.1, 90, 10, clayToRaylibColor(config.bottom.color));
                }
                break;
            }
            case RenderCommandType.CUSTOM: {
                CustomLayoutElement *customElement = (CustomLayoutElement *)renderCommand.config.customElementConfig.customData;
                if (!customElement) continue;
                switch (customElement.type) {
                    case CustomLayoutElementType.MODEL_3D: {
                        ClayBoundingBox rootBox = renderCommands.data[0].boundingBox;
                        float scaleValue = $$min($$min(1f, 768f / rootBox.height) * $$max(1f, rootBox.width / 1024f), 1.5f);
                        Ray positionRay = getScreenToWorldPointWithZDistance({ renderCommand.boundingBox.x + renderCommand.boundingBox.width / 2, renderCommand.boundingBox.y + (renderCommand.boundingBox.height / 2) + 20 }, raylibCamera, (int)$$round(rootBox.width), (int)$$round(rootBox.height), 140);
                        raylib::beginMode3D(raylibCamera);
                            raylib::drawModel(customElement.element.model.model, positionRay.position, customElement.element.model.scale * scaleValue, raylib::WHITE);        // Draw 3d model with texture
                        raylib::endMode3D();
                        break;
                    }
                    default: break;
                }
                break;
            }
            default: {
                // std::io::printfn("ERROR: unhandled render command."); TODO: maybe make a workout for this
            }
        }
    }
}
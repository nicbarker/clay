#include <Windows.h>

#if !defined(CLAY_DISABLE_SIMD) && (defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64))
#include <immintrin.h>  // AVX intrinsincs for faster sqrtf
#endif

#include "../../clay.h"

HDC renderer_hdcMem = {0};
HBITMAP renderer_hbmMem = {0};
HANDLE renderer_hOld = {0};
DWORD g_dwGdiRenderFlags;

#ifndef RECTWIDTH
#define RECTWIDTH(rc)   ((rc).right - (rc).left)
#endif
#ifndef RECTHEIGHT
#define RECTHEIGHT(rc)  ((rc).bottom - (rc).top)
#endif

// Renderer options bit flags
// RF clearly stated in the name to avoid confusion with possible macro definitions for other purposes
#define CLAYGDI_RF_ALPHABLEND       0x00000001
#define CLAYGDI_RF_SMOOTHCORNERS    0x00000002
// These are bitflags, not indexes. Next would be 0x00000004

inline DWORD Clay_Win32_GetRendererFlags() { return g_dwGdiRenderFlags; }

// Replaces the rendering flags with new ones provided
inline void Clay_Win32_SetRendererFlags(DWORD dwFlags) { g_dwGdiRenderFlags = dwFlags; }

// Returns `true` if flags were modified
inline bool Clay_Win32_ModifyRendererFlags(DWORD dwRemove, DWORD dwAdd)
{
    DWORD dwSavedFlags = g_dwGdiRenderFlags;
    DWORD dwNewFlags = (dwSavedFlags & ~dwRemove) | dwAdd;

    if (dwSavedFlags == dwNewFlags)
        return false;

    Clay_Win32_SetRendererFlags(dwNewFlags);
    return true;
}


/*----------------------------------------------------------------------------+
 | Math stuff start                                                           |
 +----------------------------------------------------------------------------*/
// Intrinsincs wrappers
#if !defined(CLAY_DISABLE_SIMD) && (defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64))
inline float intrin_sqrtf(const float f)
{
    __m128 temp = _mm_set_ss(f);
    temp = _mm_sqrt_ss(temp);
    return _mm_cvtss_f32(temp);
}
#endif

// Use fast inverse square root
#if defined(USE_FAST_SQRT)
float fast_inv_sqrtf(float number)
{
    const float threehalfs = 1.5f;

    float x2 = number * 0.5f;
    float y = number;

    // Evil bit-level hacking
    uint32_t i = *(uint32_t*)&y;
    i = 0x5f3759df - (i >> 1);  // Initial guess for Newton's method
    y = *(float*)&i;

    // One iteration of Newton's method
    y = y * (threehalfs - (x2 * y * y)); // y = y * (1.5 - 0.5 * x * y^2)

    return y;
}

// Fast square root approximation using the inverse square root
float fast_sqrtf(float number)
{
    if (number < 0.0f) return 0.0f; // Handle negative input
    return number * fast_inv_sqrtf(number);
}
#endif

// sqrtf_impl implementation chooser
#if !defined(CLAY_DISABLE_SIMD) && (defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64))
#define sqrtf_impl(x) intrin_sqrtf(x)
#elif defined(USE_FAST_SQRT)
#define sqrtf_impl(x) fast_sqrtf(x)
#else
#define sqrtf_impl(x) sqrtf(x)  // Fallback to std sqrtf
#endif
/*----------------------------------------------------------------------------+
 | Math stuff end                                                             |
 +----------------------------------------------------------------------------*/

static inline Clay_Color ColorBlend(Clay_Color base, Clay_Color overlay, float factor)
{
    Clay_Color blended;

    // Normalize alpha values for multiplications
    float base_a = base.a / 255.0f;
    float overlay_a = overlay.a / 255.0f;

    overlay_a *= factor;

    float out_a = overlay_a + base_a * (1.0f - overlay_a);

    // Avoid division by zero and fully transparent cases
    if (out_a <= 0.0f)
    {
        return (Clay_Color) { .a = 0, .r = 0, .g = 0, .b = 0 };
    }

    blended.r = (overlay.r * overlay_a + base.r * base_a * (1.0f - overlay_a)) / out_a;
    blended.g = (overlay.g * overlay_a + base.g * base_a * (1.0f - overlay_a)) / out_a;
    blended.b = (overlay.b * overlay_a + base.b * base_a * (1.0f - overlay_a)) / out_a;
    blended.a = out_a * 255.0f; // Denormalize alpha back

    return blended;
}

static float RoundedRectPixelCoverage(int x, int y, const Clay_CornerRadius radius, int width, int height) {
    // Check if the pixel is in one of the four rounded corners
    if (x < radius.topLeft && y < radius.topLeft) {
        // Top-left corner
        float dx = radius.topLeft - x - 1;
        float dy = radius.topLeft - y - 1;
        float distance = sqrtf_impl(dx * dx + dy * dy);
        if (distance > radius.topLeft)
            return 0.0f;
        if (distance <= radius.topLeft - 1)
            return 1.0f;
        return radius.topLeft - distance;
    }
    else if (x >= width - radius.topRight && y < radius.topRight) {
        // Top-right corner
        float dx = x - (width - radius.topRight);
        float dy = radius.topRight - y - 1;
        float distance = sqrtf_impl(dx * dx + dy * dy);
        if (distance > radius.topRight)
            return 0.0f;
        if (distance <= radius.topRight - 1)
            return 1.0f;
        return radius.topRight - distance;
    }
    else if (x < radius.bottomLeft && y >= height - radius.bottomLeft) {
        // Bottom-left corner
        float dx = radius.bottomLeft - x - 1;
        float dy = y - (height - radius.bottomLeft);
        float distance = sqrtf_impl(dx * dx + dy * dy);
        if (distance > radius.bottomLeft)
            return 0.0f;
        if (distance <= radius.bottomLeft - 1)
            return 1.0f;
        return radius.bottomLeft - distance;
    }
    else if (x >= width - radius.bottomRight && y >= height - radius.bottomRight) {
        // Bottom-right corner
        float dx = x - (width - radius.bottomRight);
        float dy = y - (height - radius.bottomRight);
        float distance = sqrtf_impl(dx * dx + dy * dy);
        if (distance > radius.bottomRight)
            return 0.0f;
        if (distance <= radius.bottomRight - 1)
            return 1.0f;
        return radius.bottomRight - distance;
    }
    else {
        // Not in a corner, full coverage
        return 1.0f;
    }
}

typedef struct {
    HDC hdcMem;
    HBITMAP hbmMem;
    HBITMAP hbmMemPrev;
    void* pBits;
    SIZE size;
} HDCSubstitute;

static void CreateHDCSubstitute(HDCSubstitute* phdcs, HDC hdcSrc, PRECT prc)
{
    if (prc == NULL)
        return;

    phdcs->size = (SIZE){ RECTWIDTH(*prc), RECTHEIGHT(*prc) };
    if (phdcs->size.cx <= 0 || phdcs->size.cy <= 0)
        return;

    phdcs->hdcMem = CreateCompatibleDC(hdcSrc);
    if (phdcs->hdcMem == NULL)
        return;

    // Create a 32-bit DIB section for the memory DC
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = phdcs->size.cx;
    bmi.bmiHeader.biHeight = -phdcs->size.cy;   // I think it's faster? Probably
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    phdcs->pBits = NULL;

    phdcs->hbmMem = CreateDIBSection(phdcs->hdcMem, &bmi, DIB_RGB_COLORS, &phdcs->pBits, NULL, 0);
    if (phdcs->hbmMem == NULL)
    {
        DeleteDC(phdcs->hdcMem);
        return;
    }

    // Select the DIB section into the memory DC
    phdcs->hbmMemPrev = SelectObject(phdcs->hdcMem, phdcs->hbmMem);

    // Copy the content of the target DC to the memory DC
    BitBlt(phdcs->hdcMem, 0, 0, phdcs->size.cx, phdcs->size.cy, hdcSrc, prc->left, prc->top, SRCCOPY);
}

static void DestroyHDCSubstitute(HDCSubstitute* phdcs)
{
    if (phdcs == NULL)
        return;

    // Clean up
    SelectObject(phdcs->hdcMem, phdcs->hbmMemPrev);
    DeleteObject(phdcs->hbmMem);
    DeleteDC(phdcs->hdcMem);

    ZeroMemory(phdcs, sizeof(HDCSubstitute));
}

static void __Clay_Win32_FillRoundRect(HDC hdc, PRECT prc, Clay_Color color, Clay_CornerRadius radius)
{
    HDCSubstitute substitute = { 0 };
    CreateHDCSubstitute(&substitute, hdc, prc);

    bool has_corner_radius = radius.topLeft || radius.topRight || radius.bottomLeft || radius.bottomRight;

    if (has_corner_radius)
    {
        // Limit the corner radius to the minimum of half the width and half the height
        float max_radius = (float)fmin(substitute.size.cx / 2.0f, substitute.size.cy / 2.0f);
        if (radius.topLeft > max_radius)        radius.topLeft = max_radius;
        if (radius.topRight > max_radius)       radius.topRight = max_radius;
        if (radius.bottomLeft > max_radius)     radius.bottomLeft = max_radius;
        if (radius.bottomRight > max_radius)    radius.bottomRight = max_radius;
    }

    // Iterate over each pixel in the DIB section
    uint32_t* pixels = (uint32_t*)substitute.pBits;
    for (int y = 0; y < substitute.size.cy; ++y)
    {
        for (int x = 0; x < substitute.size.cx; ++x)
        {
            float coverage = 1.0f;
            if (has_corner_radius)
                coverage = RoundedRectPixelCoverage(x, y, radius, substitute.size.cx, substitute.size.cy);

            if (coverage > 0.0f)
            {
                uint32_t pixel = pixels[y * substitute.size.cx + x];
                Clay_Color dst_color = {
                    .r = (float)((pixel >> 16) & 0xFF), // Red
                    .g = (float)((pixel >> 8) & 0xFF),  // Green
                    .b = (float)(pixel & 0xFF),         // Blue
                    .a = 255.0f                         // Fully opaque
                };
                Clay_Color blended = ColorBlend(dst_color, color, coverage);

                pixels[y * substitute.size.cx + x] =
                    ((uint32_t)(blended.b) << 0) |
                    ((uint32_t)(blended.g) << 8) |
                    ((uint32_t)(blended.r) << 16);
            }
        }
    }

    // Copy the blended content back to the target DC
    BitBlt(hdc, prc->left, prc->top, substitute.size.cx, substitute.size.cy, substitute.hdcMem, 0, 0, SRCCOPY);
    DestroyHDCSubstitute(&substitute);
}

void Clay_Win32_Render(HWND hwnd, Clay_RenderCommandArray renderCommands, HFONT* fonts)
{
    bool is_clipping = false;
    HRGN clipping_region = {0};

    PAINTSTRUCT ps;
    HDC hdc;
    RECT rc; // Top left of our window

    GetWindowRect(hwnd, &rc);

    hdc = BeginPaint(hwnd, &ps);

    int win_width = rc.right - rc.left,
        win_height = rc.bottom - rc.top;

    // Create an off-screen DC for double-buffering
    renderer_hdcMem = CreateCompatibleDC(hdc);
    renderer_hbmMem = CreateCompatibleBitmap(hdc, win_width, win_height);

    renderer_hOld = SelectObject(renderer_hdcMem, renderer_hbmMem);

    // draw

    for (int j = 0; j < renderCommands.length; j++)
    {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, j);
        Clay_BoundingBox boundingBox = renderCommand->boundingBox;

        switch (renderCommand->commandType)
        {
        case CLAY_RENDER_COMMAND_TYPE_TEXT:
        {
            Clay_Color c = renderCommand->renderData.text.textColor;
            SetTextColor(renderer_hdcMem, RGB(c.r, c.g, c.b));
            SetBkMode(renderer_hdcMem, TRANSPARENT);

            RECT r = rc;
            r.left = boundingBox.x;
            r.top = boundingBox.y;
            r.right = boundingBox.x + boundingBox.width + r.right;
            r.bottom = boundingBox.y + boundingBox.height + r.bottom;

            uint16_t font_id = renderCommand->renderData.text.fontId;
            HFONT hFont = fonts[font_id];
            HFONT hPrevFont = SelectObject(renderer_hdcMem, hFont);

            // Actually draw text
            DrawTextA(renderer_hdcMem, renderCommand->renderData.text.stringContents.chars,
                      renderCommand->renderData.text.stringContents.length,
                      &r, DT_TOP | DT_LEFT);

            SelectObject(renderer_hdcMem, hPrevFont);

            break;
        }
        case CLAY_RENDER_COMMAND_TYPE_RECTANGLE:
        {
            DWORD dwFlags = Clay_Win32_GetRendererFlags();
            Clay_RectangleRenderData rrd = renderCommand->renderData.rectangle;
            RECT r = rc;

            r.left = boundingBox.x;
            r.top = boundingBox.y;
            r.right = boundingBox.x + boundingBox.width;
            r.bottom = boundingBox.y + boundingBox.height;

            bool translucid = false;
            // There is need to check that only if alphablending is enabled.
            // In other case the blending will be always opaque and we can jump to simpler FillRgn/Rect
            if (dwFlags & CLAYGDI_RF_ALPHABLEND)
                translucid = rrd.backgroundColor.a > 0.0f && rrd.backgroundColor.a < 255.0f;
            
            bool has_rounded_corners = rrd.cornerRadius.topLeft > 0.0f
                || rrd.cornerRadius.topRight > 0.0f
                || rrd.cornerRadius.bottomLeft > 0.0f
                || rrd.cornerRadius.bottomRight > 0.0f;

            // We go here if CLAYGDI_RF_SMOOTHCORNERS flag is set and one of the corners is rounded
            // Also we go here if GLAYGDI_RF_ALPHABLEND flag is set and the fill color is translucid
            if ((dwFlags & CLAYGDI_RF_ALPHABLEND) && translucid || (dwFlags & CLAYGDI_RF_SMOOTHCORNERS) && has_rounded_corners)
            {
                __Clay_Win32_FillRoundRect(renderer_hdcMem, &r, rrd.backgroundColor, rrd.cornerRadius);
            }
            else
            {
                HBRUSH recColor = CreateSolidBrush(RGB(rrd.backgroundColor.r, rrd.backgroundColor.g, rrd.backgroundColor.b));

                if (has_rounded_corners)
                {
                    HRGN roundedRectRgn = CreateRoundRectRgn(
                        r.left, r.top, r.right + 1, r.bottom + 1,
                        rrd.cornerRadius.topLeft * 2, rrd.cornerRadius.topLeft * 2);

                    FillRgn(renderer_hdcMem, roundedRectRgn, recColor);
                    DeleteObject(roundedRectRgn);
                }
                else
                {
                    FillRect(renderer_hdcMem, &r, recColor);
                }

                DeleteObject(recColor);
            }

            break;
        }

        // The renderer should begin clipping all future draw commands, only rendering content that falls within the provided boundingBox.
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
        {
            is_clipping = true;

            clipping_region = CreateRectRgn(boundingBox.x,
                                            boundingBox.y,
                                            boundingBox.x + boundingBox.width,
                                            boundingBox.y + boundingBox.height);

            SelectClipRgn(renderer_hdcMem, clipping_region);
            break;
        }

        // The renderer should finish any previously active clipping, and begin rendering elements in full again.
        case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
        {
            SelectClipRgn(renderer_hdcMem, NULL);

            if (clipping_region)
            {
                DeleteObject(clipping_region);
            }

            is_clipping = false;
            clipping_region = NULL;

            break;
        }

        // The renderer should draw a colored border inset into the bounding box.
        case CLAY_RENDER_COMMAND_TYPE_BORDER:
        {
            Clay_BorderRenderData brd = renderCommand->renderData.border;
            RECT r = rc;

            r.left = boundingBox.x;
            r.top = boundingBox.y;
            r.right = boundingBox.x + boundingBox.width;
            r.bottom = boundingBox.y + boundingBox.height;

            HPEN topPen = CreatePen(PS_SOLID, brd.width.top, RGB(brd.color.r, brd.color.g, brd.color.b));
            HPEN leftPen = CreatePen(PS_SOLID, brd.width.left, RGB(brd.color.r, brd.color.g, brd.color.b));
            HPEN bottomPen = CreatePen(PS_SOLID, brd.width.bottom, RGB(brd.color.r, brd.color.g, brd.color.b));
            HPEN rightPen = CreatePen(PS_SOLID, brd.width.right, RGB(brd.color.r, brd.color.g, brd.color.b));

            HPEN oldPen = SelectObject(renderer_hdcMem, topPen);

            if (brd.cornerRadius.topLeft == 0)
            {
                MoveToEx(renderer_hdcMem, r.left, r.top, NULL);
                LineTo(renderer_hdcMem, r.right, r.top);

                SelectObject(renderer_hdcMem, leftPen);
                MoveToEx(renderer_hdcMem, r.left, r.top, NULL);
                LineTo(renderer_hdcMem, r.left, r.bottom);

                SelectObject(renderer_hdcMem, bottomPen);
                MoveToEx(renderer_hdcMem, r.left, r.bottom, NULL);
                LineTo(renderer_hdcMem, r.right, r.bottom);

                SelectObject(renderer_hdcMem, rightPen);
                MoveToEx(renderer_hdcMem, r.right, r.top, NULL);
                LineTo(renderer_hdcMem, r.right, r.bottom);
            }
            else
            {
                // todo: i should be rounded
                MoveToEx(renderer_hdcMem, r.left, r.top, NULL);
                LineTo(renderer_hdcMem, r.right, r.top);

                SelectObject(renderer_hdcMem, leftPen);
                MoveToEx(renderer_hdcMem, r.left, r.top, NULL);
                LineTo(renderer_hdcMem, r.left, r.bottom);

                SelectObject(renderer_hdcMem, bottomPen);
                MoveToEx(renderer_hdcMem, r.left, r.bottom, NULL);
                LineTo(renderer_hdcMem, r.right, r.bottom);

                SelectObject(renderer_hdcMem, rightPen);
                MoveToEx(renderer_hdcMem, r.right, r.top, NULL);
                LineTo(renderer_hdcMem, r.right, r.bottom);
                
            }

            SelectObject(renderer_hdcMem, oldPen);
            DeleteObject(topPen);
            DeleteObject(leftPen);
            DeleteObject(bottomPen);
            DeleteObject(rightPen);

            break;
        }

            // case CLAY_RENDER_COMMAND_TYPE_IMAGE:
            // {
            //     // TODO: i couldnt get the win 32 api to load a bitmap.... So im punting on this one :(
            //     break;
            // }

        default:
            printf("Unhandled render command %d\r\n", renderCommand->commandType);
            break;
        }
    }

    BitBlt(hdc, 0, 0, win_width, win_height, renderer_hdcMem, 0, 0, SRCCOPY);

    // Free-up the off-screen DC
    SelectObject(renderer_hdcMem, renderer_hOld);
    DeleteObject(renderer_hbmMem);
    DeleteDC(renderer_hdcMem);

    EndPaint(hwnd, &ps);
}

/*
    Hacks due to the windows api not making sence to use.... may measure too large, but never too small
*/

#ifndef WIN32_FONT_HEIGHT
#define WIN32_FONT_HEIGHT (16)
#endif

#ifndef WIN32_FONT_WIDTH
#define WIN32_FONT_WIDTH (8)
#endif

static inline Clay_Dimensions Clay_Win32_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData)
{
    Clay_Dimensions textSize = {0};

    if (userData != NULL)
    {
        HFONT* fonts = (HFONT*)userData;
        HFONT hFont = fonts[config->fontId];

        if (hFont != NULL)
        {
            HDC hScreenDC = GetDC(NULL);
            HDC hTempDC = CreateCompatibleDC(hScreenDC);

            if (hTempDC != NULL)
            {
                HFONT hPrevFont = SelectObject(hTempDC, hFont);

                SIZE size;
                GetTextExtentPoint32(hTempDC, text.chars, text.length, &size);

                textSize.width = size.cx;
                textSize.height = size.cy;

                SelectObject(hScreenDC, hPrevFont);
                DeleteDC(hTempDC);

                return textSize;
            }

            ReleaseDC(HWND_DESKTOP, hScreenDC);
        }
    }

    // Fallback for system bitmap font
    float maxTextWidth = 0.0f;
    float lineTextWidth = 0;
    float textHeight = WIN32_FONT_HEIGHT;

    for (int i = 0; i < text.length; ++i)
    {
        if (text.chars[i] == '\n')
        {
            maxTextWidth = fmax(maxTextWidth, lineTextWidth);
            lineTextWidth = 0;
            continue;
        }

        lineTextWidth += WIN32_FONT_WIDTH;
    }

    maxTextWidth = fmax(maxTextWidth, lineTextWidth);

    textSize.width = maxTextWidth;
    textSize.height = textHeight;

    return textSize;
}

HFONT Clay_Win32_SimpleCreateFont(const char* filePath, const char* family, int height, int weight)
{
    // Add the font resource to the application instance
    int fontAdded = AddFontResourceEx(filePath, FR_PRIVATE, NULL);
    if (fontAdded == 0) {
        return NULL;
    }

    int fontHeight = height;

    // If negative, treat height as Pt rather than pixels
    if (height < 0) {
        // Get the screen DPI
        HDC hScreenDC = GetDC(NULL);
        int iScreenDPI = GetDeviceCaps(hScreenDC, LOGPIXELSY);
        ReleaseDC(HWND_DESKTOP, hScreenDC);

        // Convert font height from points to pixels
        fontHeight = MulDiv(height, iScreenDPI, 72);
    }

    // Create the font using the calculated height and the font name
    HFONT hFont = CreateFont(fontHeight, 0, 0, 0, weight, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH, family);

    return hFont;
}

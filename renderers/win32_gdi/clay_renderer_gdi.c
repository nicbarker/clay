#include <Windows.h>
#include "../../clay.h"

HDC renderer_hdcMem = {0};
HBITMAP renderer_hbmMem = {0};
HANDLE renderer_hOld = {0};

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
            Clay_RectangleRenderData rrd = renderCommand->renderData.rectangle;
            RECT r = rc;

            r.left = boundingBox.x;
            r.top = boundingBox.y;
            r.right = boundingBox.x + boundingBox.width;
            r.bottom = boundingBox.y + boundingBox.height;

            HBRUSH recColor = CreateSolidBrush(RGB(rrd.backgroundColor.r, rrd.backgroundColor.g, rrd.backgroundColor.b));

            if (rrd.cornerRadius.topLeft > 0)
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
            HDC hScreenDC = GetDC(HWND_DESKTOP);
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
        HDC hScreenDC = GetDC(HWND_DESKTOP);
        int iScreenDPI = GetDeviceCaps(hScreenDC, LOGPIXELSY);
        ReleaseDC(HWND_DESKTOP, hScreenDC);

        // Convert font height from points to pixels
        fontHeight = MulDiv(height, iScreenDPI, 72);
    }

    // Create the font using the calculated height and the font name
    HFONT hFont = CreateFont(
        fontHeight,             // Height 
        0,                      // Width (0 means default width)
        0,                      // Escapement angle
        0,                      // Orientation angle
        weight,                 // Font weight
        FALSE,                  // Italic
        FALSE,                  // Underline
        FALSE,                  // Strikeout
        ANSI_CHARSET,           // Character set
        OUT_DEFAULT_PRECIS,     // Output precision
        CLIP_DEFAULT_PRECIS,    // Clipping precision
        DEFAULT_QUALITY,        // Font quality
        DEFAULT_PITCH,          // Pitch and family
        family                  // Font name
    );

    return hFont;
}

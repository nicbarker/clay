#include <cstdio>
#include <cstdlib>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wingdi.h>

#define CLAY_IMPLEMENTATION
#include "../../clay.h"

// Simple layout config with prefixed naming to match the MSVC-friendly style guide.
Clay_LayoutConfig s_LayoutElement = Clay_LayoutConfig{ .padding = {5} };

// Win32-compatible font handle that can be reused by the text measuring callback.
static HFONT s_TextFont = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));

// Bridge Clay's text measuring hook to GDI so layouts get accurate extents on Windows.
Clay_Dimensions HandleMeasureText(Clay_StringSlice text, Clay_TextElementConfig* config, void* userData)
{
    (void)config; // Config is available for future font switching or styling.
    HFONT* l_FontHandle = static_cast<HFONT*>(userData);
    HFONT l_ResolvedFont = l_FontHandle != nullptr && *l_FontHandle != nullptr ? *l_FontHandle : s_TextFont;

    HDC l_DeviceContext = GetDC(nullptr);
    if (l_DeviceContext == nullptr)
    {
        return Clay_Dimensions{ 0, 0 };
    }

    HGDIOBJ l_PreviousFont = nullptr;
    if (l_ResolvedFont != nullptr)
    {
        l_PreviousFont = SelectObject(l_DeviceContext, l_ResolvedFont);
    }

    SIZE l_TextSize{ 0, 0 };
    int l_TextLength = static_cast<int>(text.length);
    GetTextExtentPoint32A(l_DeviceContext, text.chars, l_TextLength, &l_TextSize);

    if (l_PreviousFont != nullptr)
    {
        SelectObject(l_DeviceContext, l_PreviousFont);
    }
    ReleaseDC(nullptr, l_DeviceContext);

    // Future improvement: swap GDI for DirectWrite or cache glyph metrics to avoid repeated calls.
    return Clay_Dimensions{ static_cast<float>(l_TextSize.cx), static_cast<float>(l_TextSize.cy) };
}

void HandleClayErrors(Clay_ErrorData errorData)
{
    printf("%s", errorData.errorText.chars);
}

int main(void)
{
    uint64_t l_TotalMemorySize = Clay_MinMemorySize();
    Clay_Arena l_ClayMemory = Clay_CreateArenaWithCapacityAndMemory(l_TotalMemorySize, static_cast<char*>(malloc(l_TotalMemorySize)));

    // Initialize the Clay context and immediately provide a Windows-friendly text measure callback.
    Clay_Initialize(l_ClayMemory, Clay_Dimensions{ 1024, 768 }, Clay_ErrorHandler{ HandleClayErrors });
    Clay_SetMeasureTextFunction(HandleMeasureText, &s_TextFont);

    // The measure function lets Clay compute text bounds before laying out widgets.
    Clay_BeginLayout();
    CLAY_AUTO_ID({ .layout = s_LayoutElement, .backgroundColor = {255,255,255,0} })
    {
        CLAY_TEXT(CLAY_STRING(""), CLAY_TEXT_CONFIG({ .fontId = 0 }));
    }
    Clay_EndLayout();

    return 0;
}
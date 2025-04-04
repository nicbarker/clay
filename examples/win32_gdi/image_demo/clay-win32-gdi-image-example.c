#include "../../../clay.h"
#include <stdlib.h>
#include <wincodec.h> // Required for windows image decoding

typedef struct {
    HBITMAP hBitmap;
    struct HBitmapNode* next;
} HBitmapNode;

HBitmapNode* hBitmapNodeHead = NULL;

// Stores bitmap handles so we can clean them up later.
HBITMAP* pushHBitmap(HBITMAP hBitmap)
{
    HBitmapNode* newHead = (HBitmapNode *) malloc(sizeof(HBitmapNode));
    newHead->hBitmap = hBitmap;
    newHead->next = hBitmapNodeHead;
    hBitmapNodeHead = newHead;
    return &(newHead->hBitmap);
}

// Deallocates Win32 GDI bitmap handle memory, as well as memory used by the linkedlist
void cleanupHBitmaps()
{
    HBitmapNode* curr = hBitmapNodeHead;
    HBitmapNode* next = NULL;
    while(curr != NULL)
    {
        next = curr->next;
        DeleteObject(curr->hBitmap);
        free(curr);
        curr = next;
    }
    hBitmapNodeHead = NULL;
}

// This may be slow for loading huge numbers of images,
// consider allocating and initializing interfaces once
// outside of the function if that becomes an issue
//
// Handles PNGs, JPEGs, BMPs, ICOs, TIFFs, HD Photos, and GIFs (only loads the first frame)
// Images need to be destroyed with DeleteObject(myImage) after they are no longer needed
Clay_ImageElementConfig LoadClayImage(LPCWSTR filename)
{
    // Windows codec stuff
    CoInitialize(NULL);
    IWICImagingFactory *imgFactory = NULL;
    IWICBitmapDecoder *bmpDecoder = NULL;
    IWICBitmapFrameDecode *bmpFrame = NULL;
    IWICFormatConverter *formatConverter = NULL;
    
    // Storage stuff
    HBITMAP hBitmap = NULL;
    void *bits = NULL;
    UINT width = 0;
    UINT height = 0;
    BITMAPINFO bmi = { 0 };
    
    // Start decoding
    CoCreateInstance(
            &CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            &IID_IWICImagingFactory,
            (void **) &imgFactory);
    imgFactory->lpVtbl->CreateDecoderFromFilename(
            imgFactory,
            filename,
            NULL,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &bmpDecoder);
    bmpDecoder->lpVtbl->GetFrame(bmpDecoder, 0, &bmpFrame);
    imgFactory->lpVtbl->CreateFormatConverter(imgFactory, &formatConverter);
    
    // Convert to a BitBlt-friendly format
    formatConverter->lpVtbl->Initialize(
            formatConverter,
            (IWICBitmapSource *) bmpFrame,
            &GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.0,
            WICBitmapPaletteTypeCustom);
    bmpFrame->lpVtbl->GetSize(bmpFrame, &width, &height);

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -((LONG) height); // Negative makes it a top down bitmap
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    formatConverter->lpVtbl->CopyPixels(formatConverter, NULL, width * 4, width * height * 4, (BYTE*) bits);

    if (formatConverter)
        formatConverter->lpVtbl->Release(formatConverter);
    if (bmpFrame)
        bmpFrame->lpVtbl->Release(bmpFrame);
    if (bmpDecoder)
        bmpDecoder->lpVtbl->Release(bmpDecoder);
    if (imgFactory)
        imgFactory->lpVtbl->Release(imgFactory);
    CoUninitialize();
    
    // Add to dealloc list
    HBITMAP* hBitmap_ptr = pushHBitmap(hBitmap);
    Clay_ImageElementConfig clayImage = { .sourceDimensions = { .height = height, .width = width }, .imageData = hBitmap_ptr };

    return clayImage;
}

Clay_ImageElementConfig reefImg, vacImg;

const int FONT_ID_BODY_16 = 0;
Clay_Color COLOR_WHITE = { 255, 255, 255, 255};

void RenderHeaderButton(Clay_String text) {
    CLAY({
        .layout = { .padding = { 16, 16, 8, 8 }},
        .backgroundColor = { 140, 140, 140, 255 },
        .cornerRadius = CLAY_CORNER_RADIUS(5)
    }) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({
            .fontId = FONT_ID_BODY_16,
            .fontSize = 16,
            .textColor = { 255, 255, 255, 255 }
        }));
    }
}

void RenderDropdownMenuItem(Clay_String text) {
    CLAY({.layout = { .padding = CLAY_PADDING_ALL(16)}}) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({
            .fontId = FONT_ID_BODY_16,
            .fontSize = 16,
            .textColor = { 255, 255, 255, 255 }
        }));
    }
}

typedef struct {
    Clay_String title;
    Clay_ImageElementConfig image;
    Clay_String contents;
} Document;

typedef struct {
    Document *documents;
    uint32_t length;
} DocumentArray;

Document documentsRaw[5];

DocumentArray documents = {
    .length = 5,
    .documents = documentsRaw
};

typedef struct {
    intptr_t offset;
    intptr_t memory;
} ClayVideoDemo_Arena;

typedef struct {
    int32_t selectedDocumentIndex;
    float yOffset;
    ClayVideoDemo_Arena frameArena;
} ClayVideoDemo_Data;

typedef struct {
    int32_t requestedDocumentIndex;
    int32_t* selectedDocumentIndex;
} SidebarClickData;

void HandleSidebarInteraction(
    Clay_ElementId elementId,
    Clay_PointerData pointerData,
    intptr_t userData
) {
    SidebarClickData *clickData = (SidebarClickData*)userData;
    // If this button was clicked
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        if (clickData->requestedDocumentIndex >= 0 && clickData->requestedDocumentIndex < documents.length) {
            // Select the corresponding document
            *clickData->selectedDocumentIndex = clickData->requestedDocumentIndex;
        }
    }
}

ClayVideoDemo_Data ClayVideoDemo_Initialize() {
    reefImg = LoadClayImage(L"reef.jpg");
    vacImg = LoadClayImage(L"Vacuum.jpg");
    documents.documents[0] = (Document){ .image = reefImg, .title = CLAY_STRING("Squirrels"), .contents = CLAY_STRING("There is an image here. It should be of a reef. Try resizing this image.") };
    documents.documents[1] = (Document){ .image = vacImg, .title = CLAY_STRING("Images in this example"), .contents = CLAY_STRING("In order to properly render images, make sure that you don't dereference a null or uninitialized pointer. .image attributes should be pointers to HBITMAP values. This example loads images in the same folder as the executable to HBITMAPS stored in a linkedlist, and then on close, the linkedlist is deallocated, and the HBITMAPS are destroyed. You can of course load and destroy HBITMAPS however you like, just make sure .image attributes are pointers to HBITMAP!\n\nIn this example, there is a custom document struct with an image attribute. The render commands always render the document's image, so we have to make sure we give each document a valid image! Otherwise, undefined behavior could happen, most likely in the form of dereferencing an uninitialized pointer.") };
    documents.documents[2] = (Document){ .image = vacImg, .title = CLAY_STRING("How to use Images with GDI + Clay"), .contents = CLAY_STRING("Images can be loaded in a fashion similar to the LoadClayImage function in this example for external files, or they can be compiled into the executable as a resource via a .rc file, and retrieved via windows' LoadImage function. This approach lets you skip lengthy allocations, but requires a bit more compilation messing around stuff.\n\n Load HBITMAPS however you like, and remember to destroy them before exiting the program!") };
    documents.documents[3] = (Document){ .image = reefImg, .title = CLAY_STRING("Article 4"), .contents = CLAY_STRING("Article 4") };
    documents.documents[4] = (Document){ .image = vacImg, .title = CLAY_STRING("Article 5"), .contents = CLAY_STRING("Article 5") };

    ClayVideoDemo_Data data = {
        .frameArena = { .memory = (intptr_t)malloc(1024) }
    };
    return data;
}

ClayVideoDemo_Data ClayVideoDemo_Uninitialize() {
    cleanupHBitmaps();
}

Clay_RenderCommandArray ClayVideoDemo_CreateLayout(ClayVideoDemo_Data *data) {
    data->frameArena.offset = 0;

    Clay_BeginLayout();

    Clay_Sizing layoutExpand = {
        .width = CLAY_SIZING_GROW(0),
        .height = CLAY_SIZING_GROW(0)
    };

    Clay_Color contentBackgroundColor = { 90, 90, 90, 255 };

    // Build UI here
    CLAY({ .id = CLAY_ID("OuterContainer"),
        .backgroundColor = {43, 41, 51, 255 },
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = layoutExpand,
            .padding = CLAY_PADDING_ALL(16),
            .childGap = 16
        }
    }) {
        // Child elements go inside braces
        CLAY({ .id = CLAY_ID("HeaderBar"),
            .layout = {
                .sizing = {
                    .height = CLAY_SIZING_FIXED(60),
                    .width = CLAY_SIZING_GROW(0)
                },
                .padding = { 16, 16, 0, 0 },
                .childGap = 16,
                .childAlignment = {
                    .y = CLAY_ALIGN_Y_CENTER
                }
            },
            .backgroundColor = contentBackgroundColor,
            .cornerRadius = CLAY_CORNER_RADIUS(8)
        }) {
            // Header buttons go here
            CLAY({ .id = CLAY_ID("FileButton"),
                .layout = { .padding = { 16, 16, 8, 8 }},
                .backgroundColor = {140, 140, 140, 255 },
                .cornerRadius = CLAY_CORNER_RADIUS(5)
            }) {
                CLAY_TEXT(CLAY_STRING("File"), CLAY_TEXT_CONFIG({
                    .fontId = FONT_ID_BODY_16,
                    .fontSize = 16,
                    .textColor = { 255, 255, 255, 255 }
                }));

                bool fileMenuVisible =
                    Clay_PointerOver(Clay_GetElementId(CLAY_STRING("FileButton")))
                    ||
                    Clay_PointerOver(Clay_GetElementId(CLAY_STRING("FileMenu")));

                if (fileMenuVisible) { // Below has been changed slightly to fix the small bug where the menu would dismiss when mousing over the top gap
                    CLAY({ .id = CLAY_ID("FileMenu"),
                        .floating = {
                            .attachTo = CLAY_ATTACH_TO_PARENT,
                            .attachPoints = {
                                .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM
                            },
                        },
                        .layout = {
                            .padding = {0, 0, 8, 8 }
                        }
                    }) {
                        CLAY({
                            .layout = {
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .sizing = {
                                        .width = CLAY_SIZING_FIXED(200)
                                },
                            },
                            .backgroundColor = {40, 40, 40, 255 },
                            .cornerRadius = CLAY_CORNER_RADIUS(8)
                        }) {
                            // Render dropdown items here
                            RenderDropdownMenuItem(CLAY_STRING("New"));
                            RenderDropdownMenuItem(CLAY_STRING("Open"));
                            RenderDropdownMenuItem(CLAY_STRING("Close"));
                        }
                    }
                }
            }
            RenderHeaderButton(CLAY_STRING("Edit"));
            CLAY({ .layout = { .sizing = { CLAY_SIZING_GROW(0) }}}) {}
            RenderHeaderButton(CLAY_STRING("Upload"));
            RenderHeaderButton(CLAY_STRING("Media"));
            RenderHeaderButton(CLAY_STRING("Support"));
        }

        CLAY({
            .id = CLAY_ID("LowerContent"),
            .layout = { .sizing = layoutExpand, .childGap = 16 }
        }) {
            CLAY({
                .id = CLAY_ID("Sidebar"),
                .backgroundColor = contentBackgroundColor,
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = CLAY_PADDING_ALL(16),
                    .childGap = 8,
                    .sizing = {
                        .width = CLAY_SIZING_FIXED(250),
                        .height = CLAY_SIZING_GROW(0)
                    }
                }
            }) {
                for (int i = 0; i < documents.length; i++) {
                    Document document = documents.documents[i];
                    Clay_LayoutConfig sidebarButtonLayout = {
                        .sizing = { .width = CLAY_SIZING_GROW(0) },
                        .padding = CLAY_PADDING_ALL(16)
                    };

                    if (i == data->selectedDocumentIndex) {
                        CLAY({
                            .layout = sidebarButtonLayout,
                            .backgroundColor = {120, 120, 120, 255 },
                            .cornerRadius = CLAY_CORNER_RADIUS(8)
                        }) {
                            CLAY_TEXT(document.title, CLAY_TEXT_CONFIG({
                                .fontId = FONT_ID_BODY_16,
                                .fontSize = 20,
                                .textColor = { 255, 255, 255, 255 }
                            }));
                        }
                    } else {
                        SidebarClickData *clickData = (SidebarClickData *)(data->frameArena.memory + data->frameArena.offset);
                        *clickData = (SidebarClickData) { .requestedDocumentIndex = i, .selectedDocumentIndex = &data->selectedDocumentIndex };
                        data->frameArena.offset += sizeof(SidebarClickData);
                        CLAY({ .layout = sidebarButtonLayout, .backgroundColor = (Clay_Color) { 120, 120, 120, Clay_Hovered() ? 120 : 0 }, .cornerRadius = CLAY_CORNER_RADIUS(8) }) {
                            Clay_OnHover(HandleSidebarInteraction, (intptr_t)clickData);
                            CLAY_TEXT(document.title, CLAY_TEXT_CONFIG({
                                .fontId = FONT_ID_BODY_16,
                                .fontSize = 20,
                                .textColor = { 255, 255, 255, 255 }
                            }));
                        }
                    }
                }
            }

            CLAY({ .id = CLAY_ID("MainContent"),
                .backgroundColor = contentBackgroundColor,
                .scroll = { .vertical = true },
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 16,
                    .padding = CLAY_PADDING_ALL(16),
                    .sizing = layoutExpand
                }
            }) {
                Document selectedDocument = documents.documents[data->selectedDocumentIndex];
                CLAY_TEXT(selectedDocument.title, CLAY_TEXT_CONFIG({
                    .fontId = FONT_ID_BODY_16,
                    .fontSize = 24,
                    .textColor = COLOR_WHITE
                }));
                CLAY({ .image = selectedDocument.image, .layout = { .sizing = layoutExpand } });
                CLAY_TEXT(selectedDocument.contents, CLAY_TEXT_CONFIG({
                    .fontId = FONT_ID_BODY_16,
                    .fontSize = 24,
                    .textColor = COLOR_WHITE
                }));
            }
        }
    }

    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    for (int32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommandArray_Get(&renderCommands, i)->boundingBox.y += data->yOffset;
    }
    return renderCommands;
}

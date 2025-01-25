#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#define CLAY_IMPLEMENTATION
#include "../../clay.h"
#include "../../renderers/NanoVG/clay_renderer_NanoVG.c"
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


int FONT_ID_BODY_16 = -1;
Clay_Color COLOR_WHITE = { 255, 255, 255, 255};

void RenderHeaderButton(Clay_String text) {
    glViewport(0, 0, 800, 600);
    CLAY(
        CLAY_LAYOUT({ .padding = { 16, 8 }}),
        CLAY_RECTANGLE({
            .color = { 140, 140, 140, 255 },
            .cornerRadius = 5
        })
    ) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({
            .fontId = FONT_ID_BODY_16,
            .fontSize = 16,
            .textColor = { 255, 255, 255, 255 }
        }));
    }
}

void RenderDropdownMenuItem(Clay_String text) {
    CLAY(CLAY_LAYOUT({ .padding = { 16, 16 }})) {
        CLAY_TEXT(text, CLAY_TEXT_CONFIG({
            .fontId = FONT_ID_BODY_16,
            .fontSize = 16,
            .textColor = { 255, 255, 255, 255 }
        }));
    }
}

typedef struct {
    Clay_String title;
    Clay_String contents;
} Document;

typedef struct {
    Document *documents;
    uint32_t length;
} DocumentArray;

extern DocumentArray documents;

uint32_t selectedDocumentIndex = 0;

void HandleSidebarInteraction(
    Clay_ElementId elementId,
    Clay_PointerData pointerData,
    intptr_t userData
) {
    // If this button was clicked
    if (pointerData.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME) {
        if (userData >= 0 && userData < documents.length) {
            // Select the corresponding document
            selectedDocumentIndex = userData;
        }
    }
}

static Clay_RenderCommandArray CreateLayout() {
    Clay_BeginLayout();
    Clay_Sizing layoutExpand = {
        .width = CLAY_SIZING_GROW(),
        .height = CLAY_SIZING_GROW()
    };

    Clay_RectangleElementConfig contentBackgroundConfig = {
        .color = { 90, 90, 90, 255 },
        .cornerRadius = 8
    };

    Clay_BeginLayout();
    // Build UI here
    CLAY(
        CLAY_ID("OuterContainer"),
        CLAY_RECTANGLE({ .color = { 43, 41, 51, 255 } }),
        CLAY_LAYOUT({
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = layoutExpand,
            .padding = { 16, 16 },
            .childGap = 16
        })
    ) {
        // Child elements go inside braces
        CLAY(
            CLAY_ID("HeaderBar"),
            CLAY_RECTANGLE(contentBackgroundConfig),
            CLAY_LAYOUT({
                .sizing = {
                    .height = CLAY_SIZING_FIXED(60),
                    .width = CLAY_SIZING_GROW()
                },
                .padding = { 16 },
                .childGap = 16,
                .childAlignment = {
                    .y = CLAY_ALIGN_Y_CENTER
                }
            })
        ) {
            // Header buttons go here
            CLAY(
                CLAY_ID("FileButton"),
                CLAY_LAYOUT({ .padding = { 16, 8 }}),
                CLAY_RECTANGLE({
                    .color = { 140, 140, 140, 255 },
                    .cornerRadius = 5
                })
            ) {
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
                    CLAY(
                        CLAY_ID("FileMenu"),
                        CLAY_FLOATING({
                            .attachment = {
                                .parent = CLAY_ATTACH_POINT_LEFT_BOTTOM
                            },
                        }),
                        CLAY_LAYOUT({
                            .padding = {0, 8 }
                        })
                    ) {
                        CLAY(
                            CLAY_LAYOUT({
                                .layoutDirection = CLAY_TOP_TO_BOTTOM,
                                .sizing = {
                                        .width = CLAY_SIZING_FIXED(200)
                                },
                            }),
                            CLAY_RECTANGLE({
                                .color = { 40, 40, 40, 255 },
                                .cornerRadius = 8
                            })
                        ) {
                            // Render dropdown items here
                            RenderDropdownMenuItem(CLAY_STRING("New"));
                            RenderDropdownMenuItem(CLAY_STRING("Open"));
                            RenderDropdownMenuItem(CLAY_STRING("Close"));
                        }
                    }
                }
            }
            RenderHeaderButton(CLAY_STRING("Edit"));
            CLAY(CLAY_LAYOUT({ .sizing = { CLAY_SIZING_GROW() }})) {}
            RenderHeaderButton(CLAY_STRING("Upload"));
            RenderHeaderButton(CLAY_STRING("Media"));
            RenderHeaderButton(CLAY_STRING("Support"));
        }

        CLAY(
            CLAY_ID("LowerContent"),
            CLAY_LAYOUT({ .sizing = layoutExpand, .childGap = 16 })
        ) {
            CLAY(
                CLAY_ID("Sidebar"),
                CLAY_RECTANGLE(contentBackgroundConfig),
                CLAY_LAYOUT({
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = { 16, 16 },
                    .childGap = 8,
                    .sizing = {
                        .width = CLAY_SIZING_FIXED(250),
                        .height = CLAY_SIZING_GROW()
                    }
                })
            ) {
                for (int i = 0; i < documents.length; i++) {
                    Document document = documents.documents[i];
                    Clay_LayoutConfig sidebarButtonLayout = {
                        .sizing = { .width = CLAY_SIZING_GROW() },
                        .padding = { 16, 16 }
                    };

                    if (i == selectedDocumentIndex) {
                        CLAY(
                            CLAY_LAYOUT(sidebarButtonLayout),
                            CLAY_RECTANGLE({
                                .color = { 120, 120, 120, 255 },
                                .cornerRadius = 8,
                            })
                        ) {
                            CLAY_TEXT(document.title, CLAY_TEXT_CONFIG({
                                .fontId = FONT_ID_BODY_16,
                                .fontSize = 20,
                                .textColor = { 255, 255, 255, 255 }
                            }));
                        }
                    } else {
                        CLAY(
                            CLAY_LAYOUT(sidebarButtonLayout),
                            Clay_OnHover(HandleSidebarInteraction, i),
                            Clay_Hovered()
                                ? CLAY_RECTANGLE({
                                    .color = { 120, 120, 120, 120 },
                                    .cornerRadius = 8
                                })
                                : 0
                        ) {
                            CLAY_TEXT(document.title, CLAY_TEXT_CONFIG({
                                .fontId = FONT_ID_BODY_16,
                                .fontSize = 20,
                                .textColor = { 255, 255, 255, 255 }
                            }));
                        }
                    }
                }
            }

            CLAY(
                CLAY_ID("MainContent"),
                CLAY_RECTANGLE(contentBackgroundConfig),
                CLAY_SCROLL({ .vertical = true }),
                CLAY_LAYOUT({
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 16,
                    .padding = { 16, 16 },
                    .sizing = layoutExpand
                })
            ) {
                Document selectedDocument = documents.documents[selectedDocumentIndex];
                CLAY_TEXT(selectedDocument.title, CLAY_TEXT_CONFIG({
                    .fontId = FONT_ID_BODY_16,
                    .fontSize = 24,
                    .textColor = COLOR_WHITE
                }));
                CLAY_TEXT(selectedDocument.contents, CLAY_TEXT_CONFIG({
                    .fontId = FONT_ID_BODY_16,
                    .fontSize = 24,
                    .textColor = COLOR_WHITE
                }));
            }
        }
    }

    return Clay_EndLayout();
}

void HandleClayErrors(Clay_ErrorData errorData) {
    printf("%s", errorData.errorText.chars);
}

double deltaTime = 0;
static void HandleScroll(GLFWwindow *window, double x, double y)
{
    Clay_UpdateScrollContainers(true, (Clay_Vector2){ x, y }, deltaTime);
}

int main(void) {
    if (!glfwInit()) {
        const char *error;
        glfwGetError(&error);
        fprintf(stderr, "Error: could not initialize GLFW: %s\n", error);
        return 1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow *window = glfwCreateWindow(800, 600, "NanoVG-video-demo", NULL, NULL);
    if (!window) {
        const char *error;
        glfwGetError(&error);
        fprintf(stderr, "Error: could not create window: %s\n", error);
        return 1;
    }
    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }

    NVGcontext *nvg = nvgCreateGL3(NVG_ANTIALIAS | NVG_DEBUG);
    if (!nvg) {
        fprintf(stderr, "Error: could not create NanoVG context\n");
        return 1;
    }

    FONT_ID_BODY_16 = nvgCreateFont(nvg, "roboto-regular", "resources/Roboto-Regular.ttf");
    if (FONT_ID_BODY_16 < 0) {
        fprintf(stderr, "Error: could not load font\n");
        return 1;
    }

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));


    Clay_SetMeasureTextFunction(Clay_NanoVG_MeasureText, (uintptr_t)nvg);

    int windowWidth = 0;
    int windowHeight = 0;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    Clay_Initialize(clayMemory, (Clay_Dimensions) { (float)windowWidth, (float)windowHeight }, (Clay_ErrorHandler) { HandleClayErrors });
    double NOW = glfwGetTime();
    double LAST = 0;

    glfwSetScrollCallback(window, HandleScroll);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        LAST = NOW;
        NOW = glfwGetTime();
        deltaTime = (double)((NOW - LAST)*1000 / (double)glfwGetTime());

        bool leftDown = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
        double mouseX = 0;
        double mouseY = 0;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        Clay_Vector2 mousePosition = (Clay_Vector2){ (float)mouseX, (float)mouseY };
        Clay_SetPointerState(mousePosition, leftDown);

        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        Clay_SetLayoutDimensions((Clay_Dimensions) { (float)windowWidth, (float)windowHeight });

        int frameBufferWidth;
        int frameBufferHeight;
        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
		float devicePixelRatio = (float)frameBufferWidth / (float)windowWidth;

        Clay_RenderCommandArray renderCommands = CreateLayout();
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(nvg, windowWidth, windowHeight, devicePixelRatio);

        Clay_NanoVG_Render(nvg, renderCommands);

        nvgEndFrame(nvg);
        glfwSwapBuffers(window);
    }

quit:
    glfwDestroyWindow(window);
    return 0;
}


static Document documentsArray[] = {
    { .title = CLAY_STRING("Squirrels"), .contents = CLAY_STRING("The Secret Life of Squirrels: Nature's Clever Acrobats\n""Squirrels are often overlooked creatures, dismissed as mere park inhabitants or backyard nuisances. Yet, beneath their fluffy tails and twitching noses lies an intricate world of cunning, agility, and survival tactics that are nothing short of fascinating. As one of the most common mammals in North America, squirrels have adapted to a wide range of environments from bustling urban centers to tranquil forests and have developed a variety of unique behaviors that continue to intrigue scientists and nature enthusiasts alike.\n""\n""Master Tree Climbers\n""At the heart of a squirrel's skill set is its impressive ability to navigate trees with ease. Whether they're darting from branch to branch or leaping across wide gaps, squirrels possess an innate talent for acrobatics. Their powerful hind legs, which are longer than their front legs, give them remarkable jumping power. With a tail that acts as a counterbalance, squirrels can leap distances of up to ten times the length of their body, making them some of the best aerial acrobats in the animal kingdom.\n""But it's not just their agility that makes them exceptional climbers. Squirrels' sharp, curved claws allow them to grip tree bark with precision, while the soft pads on their feet provide traction on slippery surfaces. Their ability to run at high speeds and scale vertical trunks with ease is a testament to the evolutionary adaptations that have made them so successful in their arboreal habitats.\n""\n""Food Hoarders Extraordinaire\n""Squirrels are often seen frantically gathering nuts, seeds, and even fungi in preparation for winter. While this behavior may seem like instinctual hoarding, it is actually a survival strategy that has been honed over millions of years. Known as \"scatter hoarding,\" squirrels store their food in a variety of hidden locations, often burying it deep in the soil or stashing it in hollowed-out tree trunks.\n""Interestingly, squirrels have an incredible memory for the locations of their caches. Research has shown that they can remember thousands of hiding spots, often returning to them months later when food is scarce. However, they don't always recover every stash some forgotten caches eventually sprout into new trees, contributing to forest regeneration. This unintentional role as forest gardeners highlights the ecological importance of squirrels in their ecosystems.\n""\n""The Great Squirrel Debate: Urban vs. Wild\n""While squirrels are most commonly associated with rural or wooded areas, their adaptability has allowed them to thrive in urban environments as well. In cities, squirrels have become adept at finding food sources in places like parks, streets, and even garbage cans. However, their urban counterparts face unique challenges, including traffic, predators, and the lack of natural shelters. Despite these obstacles, squirrels in urban areas are often observed using human infrastructure such as buildings, bridges, and power lines as highways for their acrobatic escapades.\n""There is, however, a growing concern regarding the impact of urban life on squirrel populations. Pollution, deforestation, and the loss of natural habitats are making it more difficult for squirrels to find adequate food and shelter. As a result, conservationists are focusing on creating squirrel-friendly spaces within cities, with the goal of ensuring these resourceful creatures continue to thrive in both rural and urban landscapes.\n""\n""A Symbol of Resilience\n""In many cultures, squirrels are symbols of resourcefulness, adaptability, and preparation. Their ability to thrive in a variety of environments while navigating challenges with agility and grace serves as a reminder of the resilience inherent in nature. Whether you encounter them in a quiet forest, a city park, or your own backyard, squirrels are creatures that never fail to amaze with their endless energy and ingenuity.\n""In the end, squirrels may be small, but they are mighty in their ability to survive and thrive in a world that is constantly changing. So next time you spot one hopping across a branch or darting across your lawn, take a moment to appreciate the remarkable acrobat at work a true marvel of the natural world.\n") },
    { .title = CLAY_STRING("Lorem Ipsum"), .contents = CLAY_STRING("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.") },
    { .title = CLAY_STRING("Vacuum Instructions"), .contents = CLAY_STRING("Chapter 3: Getting Started - Unpacking and Setup\n""\n""Congratulations on your new SuperClean Pro 5000 vacuum cleaner! In this section, we will guide you through the simple steps to get your vacuum up and running. Before you begin, please ensure that you have all the components listed in the \"Package Contents\" section on page 2.\n""\n""1. Unboxing Your Vacuum\n""Carefully remove the vacuum cleaner from the box. Avoid using sharp objects that could damage the product. Once removed, place the unit on a flat, stable surface to proceed with the setup. Inside the box, you should find:\n""\n""    The main vacuum unit\n""    A telescoping extension wand\n""    A set of specialized cleaning tools (crevice tool, upholstery brush, etc.)\n""    A reusable dust bag (if applicable)\n""    A power cord with a 3-prong plug\n""    A set of quick-start instructions\n""\n""2. Assembling Your Vacuum\n""Begin by attaching the extension wand to the main body of the vacuum cleaner. Line up the connectors and twist the wand into place until you hear a click. Next, select the desired cleaning tool and firmly attach it to the wand's end, ensuring it is securely locked in.\n""\n""For models that require a dust bag, slide the bag into the compartment at the back of the vacuum, making sure it is properly aligned with the internal mechanism. If your vacuum uses a bagless system, ensure the dust container is correctly seated and locked in place before use.\n""\n""3. Powering On\n""To start the vacuum, plug the power cord into a grounded electrical outlet. Once plugged in, locate the power switch, usually positioned on the side of the handle or body of the unit, depending on your model. Press the switch to the \"On\" position, and you should hear the motor begin to hum. If the vacuum does not power on, check that the power cord is securely plugged in, and ensure there are no blockages in the power switch.\n""\n""Note: Before first use, ensure that the vacuum filter (if your model has one) is properly installed. If unsure, refer to \"Section 5: Maintenance\" for filter installation instructions.") },
    { .title = CLAY_STRING("Article 4"), .contents = CLAY_STRING("Article 4") },
    { .title = CLAY_STRING("Article 5"), .contents = CLAY_STRING("Article 5") },
};
DocumentArray documents = {
    .documents = documentsArray,
    .length = 5
};

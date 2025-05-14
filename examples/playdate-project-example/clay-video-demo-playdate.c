// This is the video demo with some adjustments so it works on the playdate
// console The playdate screen is only 400x240 pixels and it can only display
// black and white, so some fixed sizes and colours needed tweaking! The file
// menu was also removed as it does not really make sense when there is no
// pointer
//
// Note: The playdate console also does not support dynamic font sizes - fonts must be
// created at a specific size with the pdc tool - so any font size set in the clay layout
// will have no effect.
#include "pd_api.h"
#include "../../clay.h"
#include <stdlib.h>

const int FONT_ID_BODY = 0;
const int FONT_ID_BUTTON = 1;

Clay_Color COLOR_WHITE = { 255, 255, 255, 255 };
Clay_Color COLOR_BLACK = { 0, 0, 0, 255 };

void RenderHeaderButton(Clay_String text) {
    CLAY({
        .layout = { .padding = { 8, 8, 4, 4 } },
        .backgroundColor = COLOR_BLACK,
        .cornerRadius = CLAY_CORNER_RADIUS(4)
    }) {
        CLAY_TEXT(
            text,
            CLAY_TEXT_CONFIG({ .fontId = FONT_ID_BUTTON, .textColor = COLOR_WHITE })
        );
    }
}

typedef struct {
    Clay_String title;
    Clay_String contents;
    LCDBitmap* image;
} Document;

typedef struct {
    Document *documents;
    uint32_t length;
} DocumentArray;

#define MAX_DOCUMENTS 3
Document documentsRaw[MAX_DOCUMENTS];

DocumentArray documents = { .length = MAX_DOCUMENTS, .documents = documentsRaw };

void ClayVideoDemoPlaydate_Initialize(PlaydateAPI* pd) {
    documents.documents[0] = (Document){
        .title = CLAY_STRING("Squirrels"),
        .image = pd->graphics->loadBitmap("star.png", NULL),
        .contents = CLAY_STRING(
            "The Secret Life of Squirrels: Nature's Clever Acrobats\n"
            "Squirrels are often overlooked creatures, dismissed as mere park "
            "inhabitants or backyard nuisances. Yet, beneath their fluffy tails "
            "and twitching noses lies an intricate world of cunning, agility, "
            "and survival tactics that are nothing short of fascinating. As one "
            "of the most common mammals in North America, squirrels have adapted "
            "to a wide range of environments from bustling urban centers to "
            "tranquil forests and have developed a variety of unique behaviors "
            "that continue to intrigue scientists and nature enthusiasts alike.\n"
            "\n"
            "Master Tree Climbers\n"
            "At the heart of a squirrel's skill set is its impressive ability to "
            "navigate trees with ease. Whether they're darting from branch to "
            "branch or leaping across wide gaps, squirrels possess an innate "
            "talent for acrobatics. Their powerful hind legs, which are longer "
            "than their front legs, give them remarkable jumping power. With a "
            "tail that acts as a counterbalance, squirrels can leap distances of "
            "up to ten times the length of their body, making them some of the "
            "best aerial acrobats in the animal kingdom.\n"
            "But it's not just their agility that makes them exceptional "
            "climbers. Squirrels' sharp, curved claws allow them to grip tree "
            "bark with precision, while the soft pads on their feet provide "
            "traction on slippery surfaces. Their ability to run at high speeds "
            "and scale vertical trunks with ease is a testament to the "
            "evolutionary adaptations that have made them so successful in their "
            "arboreal habitats.\n"
            "\n"
            "Food Hoarders Extraordinaire\n"
            "Squirrels are often seen frantically gathering nuts, seeds, and "
            "even fungi in preparation for winter. While this behavior may seem "
            "like instinctual hoarding, it is actually a survival strategy that "
            "has been honed over millions of years. Known as \"scatter "
            "hoarding,\" squirrels store their food in a variety of hidden "
            "locations, often burying it deep in the soil or stashing it in "
            "hollowed-out tree trunks.\n"
            "Interestingly, squirrels have an incredible memory for the "
            "locations of their caches. Research has shown that they can "
            "remember thousands of hiding spots, often returning to them months "
            "later when food is scarce. However, they don't always recover every "
            "stash some forgotten caches eventually sprout into new trees, "
            "contributing to forest regeneration. This unintentional role as "
            "forest gardeners highlights the ecological importance of squirrels "
            "in their ecosystems.\n"
            "\n"
            "The Great Squirrel Debate: Urban vs. Wild\n"
            "While squirrels are most commonly associated with rural or wooded "
            "areas, their adaptability has allowed them to thrive in urban "
            "environments as well. In cities, squirrels have become adept at "
            "finding food sources in places like parks, streets, and even "
            "garbage cans. However, their urban counterparts face unique "
            "challenges, including traffic, predators, and the lack of natural "
            "shelters. Despite these obstacles, squirrels in urban areas are "
            "often observed using human infrastructure such as buildings, "
            "bridges, and power lines as highways for their acrobatic "
            "escapades.\n"
            "There is, however, a growing concern regarding the impact of urban "
            "life on squirrel populations. Pollution, deforestation, and the "
            "loss of natural habitats are making it more difficult for squirrels "
            "to find adequate food and shelter. As a result, conservationists "
            "are focusing on creating squirrel-friendly spaces within cities, "
            "with the goal of ensuring these resourceful creatures continue to "
            "thrive in both rural and urban landscapes.\n"
            "\n"
            "A Symbol of Resilience\n"
            "In many cultures, squirrels are symbols of resourcefulness, "
            "adaptability, and preparation. Their ability to thrive in a variety "
            "of environments while navigating challenges with agility and grace "
            "serves as a reminder of the resilience inherent in nature. Whether "
            "you encounter them in a quiet forest, a city park, or your own "
            "backyard, squirrels are creatures that never fail to amaze with "
            "their endless energy and ingenuity.\n"
            "In the end, squirrels may be small, but they are mighty in their "
            "ability to survive and thrive in a world that is constantly "
            "changing. So next time you spot one hopping across a branch or "
            "darting across your lawn, take a moment to appreciate the "
            "remarkable acrobat at work a true marvel of the natural world.\n"
        )
    };
    documents.documents[1] = (Document){
        .title = CLAY_STRING("Lorem Ipsum"),
        .image = pd->graphics->loadBitmap("star.png", NULL),
        .contents = CLAY_STRING(
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do "
            "eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim "
            "ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut "
            "aliquip ex ea commodo consequat. Duis aute irure dolor in "
            "reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla "
            "pariatur. Excepteur sint occaecat cupidatat non proident, sunt in "
            "culpa qui officia deserunt mollit anim id est laborum."
        )
    };
    documents.documents[2] = (Document){
        .title = CLAY_STRING("Vacuum Instructions"),
        .image = pd->graphics->loadBitmap("star.png", NULL),
        .contents = CLAY_STRING(
            "Chapter 3: Getting Started - Unpacking and Setup\n"
            "\n"
            "Congratulations on your new SuperClean Pro 5000 vacuum cleaner! In "
            "this section, we will guide you through the simple steps to get "
            "your vacuum up and running. Before you begin, please ensure that "
            "you have all the components listed in the \"Package Contents\" "
            "section on page 2.\n"
            "\n"
            "1. Unboxing Your Vacuum\n"
            "Carefully remove the vacuum cleaner from the box. Avoid using sharp "
            "objects that could damage the product. Once removed, place the unit "
            "on a flat, stable surface to proceed with the setup. Inside the "
            "box, you should find:\n"
            "\n"
            "    The main vacuum unit\n"
            "    A telescoping extension wand\n"
            "    A set of specialized cleaning tools (crevice tool, upholstery "
            "brush, etc.)\n"
            "    A reusable dust bag (if applicable)\n"
            "    A power cord with a 3-prong plug\n"
            "    A set of quick-start instructions\n"
            "\n"
            "2. Assembling Your Vacuum\n"
            "Begin by attaching the extension wand to the main body of the "
            "vacuum cleaner. Line up the connectors and twist the wand into "
            "place until you hear a click. Next, select the desired cleaning "
            "tool and firmly attach it to the wand's end, ensuring it is "
            "securely locked in.\n"
            "\n"
            "For models that require a dust bag, slide the bag into the "
            "compartment at the back of the vacuum, making sure it is properly "
            "aligned with the internal mechanism. If your vacuum uses a bagless "
            "system, ensure the dust container is correctly seated and locked in "
            "place before use.\n"
            "\n"
            "3. Powering On\n"
            "To start the vacuum, plug the power cord into a grounded electrical "
            "outlet. Once plugged in, locate the power switch, usually "
            "positioned on the side of the handle or body of the unit, depending "
            "on your model. Press the switch to the \"On\" position, and you "
            "should hear the motor begin to hum. If the vacuum does not power "
            "on, check that the power cord is securely plugged in, and ensure "
            "there are no blockages in the power switch.\n"
            "\n"
            "Note: Before first use, ensure that the vacuum filter (if your "
            "model has one) is properly installed. If unsure, refer to \"Section "
            "5: Maintenance\" for filter installation instructions."
        )
    };
}

Clay_RenderCommandArray ClayVideoDemoPlaydate_CreateLayout(int selectedDocumentIndex) {

    Clay_BeginLayout();

    Clay_Sizing layoutExpand = {
        .width = CLAY_SIZING_GROW(0),
        .height = CLAY_SIZING_GROW(0)
    };

    Clay_BorderElementConfig contentBorders = {
        .color = COLOR_BLACK,
        .width = { .top = 1, .left = 1, .right = 1, .bottom = 1 }
    };

    // Build UI here
    CLAY({
        .id = CLAY_ID("OuterContainer"),
        .backgroundColor = COLOR_WHITE,
        .layout = {
            .layoutDirection = CLAY_TOP_TO_BOTTOM,
            .sizing = layoutExpand,
            .padding = CLAY_PADDING_ALL(8),
            .childGap = 4
         }
    }) {
        // Child elements go inside braces
        CLAY({
            .id = CLAY_ID("HeaderBar"),
            .layout = {
                .sizing = {
                    .height = CLAY_SIZING_FIXED(30),
                    .width = CLAY_SIZING_GROW(0)
                },
                .childGap = 8,
                .childAlignment = { .y = CLAY_ALIGN_Y_CENTER }
            },
        }) {
            // Header buttons go here
            CLAY({
                .id = CLAY_ID("FileButton"),
                .layout = {
                    .padding = { 8, 8, 4, 4 }
                },
                .backgroundColor = COLOR_BLACK,
                .cornerRadius = CLAY_CORNER_RADIUS(4)
            }) {
                CLAY_TEXT(
                    CLAY_STRING("File"),
                    CLAY_TEXT_CONFIG({
                        .fontId = FONT_ID_BUTTON,
                        .textColor = COLOR_WHITE
                    })
                );
            }
            RenderHeaderButton(CLAY_STRING("Edit"));
            CLAY({ .layout = { .sizing = { CLAY_SIZING_GROW(0) } } }) {}
            RenderHeaderButton(CLAY_STRING("Upload"));
            RenderHeaderButton(CLAY_STRING("Media"));
            RenderHeaderButton(CLAY_STRING("Support"));
        }

        CLAY({
            .id = CLAY_ID("LowerContent"),
            .layout = { .sizing = layoutExpand, .childGap = 8 },
        }) {
            CLAY({
                .id = CLAY_ID("Sidebar"),
                .border = contentBorders,
                .cornerRadius = CLAY_CORNER_RADIUS(4),
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .padding = CLAY_PADDING_ALL(8),
                    .childGap = 4,
                    .sizing = {
                        .width = CLAY_SIZING_FIXED(125),
                        .height = CLAY_SIZING_GROW(0)
                    }
                }
            }) {
                for (int i = 0; i < documents.length; i++) {
                    Document document = documents.documents[i];
                    Clay_LayoutConfig sidebarButtonLayout = {
                        .sizing = { .width = CLAY_SIZING_GROW(0) },
                        .padding = CLAY_PADDING_ALL(8)
                    };

                    if (i == selectedDocumentIndex) {
                        CLAY({
                            .layout = sidebarButtonLayout,
                            .backgroundColor = COLOR_BLACK,
                            .cornerRadius = CLAY_CORNER_RADIUS(4)
                        }) {
                            CLAY_TEXT(
                                document.title,
                                CLAY_TEXT_CONFIG({
                                    .fontId = FONT_ID_BUTTON,
                                    .textColor = COLOR_WHITE
                                })
                            );
                        }
                    } else {
                        CLAY({
                            .layout = sidebarButtonLayout,
                            .backgroundColor = (Clay_Color){ 0, 0, 0, Clay_Hovered() ? 120 : 0 },
                            .cornerRadius = CLAY_CORNER_RADIUS(4),
                            .border = contentBorders
                        }) {
                            CLAY_TEXT(
                                document.title,
                                CLAY_TEXT_CONFIG({
                                    .fontId = FONT_ID_BUTTON,
                                    .textColor = COLOR_BLACK,
                                })
                            );
                        }
                    }
                }
            }

            CLAY({
                .id = CLAY_ID("MainContent"),
                .border = contentBorders,
                .cornerRadius = CLAY_CORNER_RADIUS(4),
                .clip = { .vertical = true, .childOffset = Clay_GetScrollOffset() },
                .layout = {
                    .layoutDirection = CLAY_TOP_TO_BOTTOM,
                    .childGap = 8,
                    .padding = CLAY_PADDING_ALL(8),
                    .sizing = layoutExpand
                }
            }) {
                Document selectedDocument = documents.documents[selectedDocumentIndex];
                CLAY({
                    .layout = {
                        .layoutDirection = CLAY_LEFT_TO_RIGHT,
                        .childGap = 4,
                        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_BOTTOM }
                    }
                }) {
                    CLAY_TEXT(
                        selectedDocument.title,
                        CLAY_TEXT_CONFIG({ .fontId = FONT_ID_BODY, .textColor = COLOR_BLACK })
                    );
                    CLAY({
                        .layout = {
                            .sizing = {
                                .width = CLAY_SIZING_FIXED(32),
                                .height = CLAY_SIZING_FIXED(30)
                            }
                        },
                        .image = { .imageData = selectedDocument.image, .sourceDimensions = { 32, 30 } }
                    }) {}
                }
                CLAY_TEXT(
                    selectedDocument.contents,
                    CLAY_TEXT_CONFIG({ .fontId = FONT_ID_BODY, .textColor = COLOR_BLACK })
                );
            }
        }
    }

    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    for (int32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommandArray_Get(&renderCommands, i);
    }
    return renderCommands;
}

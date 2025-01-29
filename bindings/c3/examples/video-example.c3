module videoexample;

import clay;
import raylib;

const uint FONT_ID_BODY_16 = 0;

fn void errorHandler(ErrorData errorData)
{
    // std::io::printfn("ERROR: \"%s\"", errorData.errorText.chars);
}

fn void renderDropdownMenuItem(String text) {
    @clay(
        clay::layout({ .padding = clay::paddingUni(16)})
    ) {
        clay::text(
            text, clay::textConfig({
            .textColor = { 255, 255, 255, 255 },
            .fontId = FONT_ID_BODY_16,
            .fontSize = 16,
        }));
    };
}

fn void renderHeaderButton(String text) {
    @clay(
        clay::layout({ .padding = { 16, 16, 8, 8 }}),
        clay::rectangle({
            .color = { 140, 140, 140, 255 },
            .cornerRadius = clay::cornerRadiusUni(5)
        })
    ) {
        clay::text(text, clay::textConfig({
            .textColor = { 255, 255, 255, 255 },
            .fontId = FONT_ID_BODY_16,
            .fontSize = 16,
        }));
    };
}

fn void handleSidebarInteraction(
    ElementId elementId,
    PointerData pointerData,
    iptr userData
) {
    // If this button was clicked
    if (pointerData.state == PointerState.RELEASED_THIS_FRAME) {
        if (userData >= 0 && userData < ALL_DOCUMENTS.len) {
            // Select the corresponding document
            selectedDocumentIndex = (uint)userData;
        }
    }
}

struct Document {
    String title;
    String contents;
}

const Document[*] ALL_DOCUMENTS = {
    { "Squirrels", "The Secret Life of Squirrels: Nature's Clever Acrobats\n""Squirrels are often overlooked creatures, dismissed as mere park inhabitants or backyard nuisances. Yet, beneath their fluffy tails and twitching noses lies an intricate world of cunning, agility, and survival tactics that are nothing short of fascinating. As one of the most common mammals in North America, squirrels have adapted to a wide range of environments from bustling urban centers to tranquil forests and have developed a variety of unique behaviors that continue to intrigue scientists and nature enthusiasts alike.\n""\n""Master Tree Climbers\n""At the heart of a squirrel's skill set is its impressive ability to navigate trees with ease. Whether they're darting from branch to branch or leaping across wide gaps, squirrels possess an innate talent for acrobatics. Their powerful hind legs, which are longer than their front legs, give them remarkable jumping power. With a tail that acts as a counterbalance, squirrels can leap distances of up to ten times the length of their body, making them some of the best aerial acrobats in the animal kingdom.\n""But it's not just their agility that makes them exceptional climbers. Squirrels' sharp, curved claws allow them to grip tree bark with precision, while the soft pads on their feet provide traction on slippery surfaces. Their ability to run at high speeds and scale vertical trunks with ease is a testament to the evolutionary adaptations that have made them so successful in their arboreal habitats.\n""\n""Food Hoarders Extraordinaire\n""Squirrels are often seen frantically gathering nuts, seeds, and even fungi in preparation for winter. While this behavior may seem like instinctual hoarding, it is actually a survival strategy that has been honed over millions of years. Known as \"scatter hoarding,\" squirrels store their food in a variety of hidden locations, often burying it deep in the soil or stashing it in hollowed-out tree trunks.\n""Interestingly, squirrels have an incredible memory for the locations of their caches. Research has shown that they can remember thousands of hiding spots, often returning to them months later when food is scarce. However, they don't always recover every stash some forgotten caches eventually sprout into new trees, contributing to forest regeneration. This unintentional role as forest gardeners highlights the ecological importance of squirrels in their ecosystems.\n""\n""The Great Squirrel Debate: Urban vs. Wild\n""While squirrels are most commonly associated with rural or wooded areas, their adaptability has allowed them to thrive in urban environments as well. In cities, squirrels have become adept at finding food sources in places like parks, streets, and even garbage cans. However, their urban counterparts face unique challenges, including traffic, predators, and the lack of natural shelters. Despite these obstacles, squirrels in urban areas are often observed using human infrastructure such as buildings, bridges, and power lines as highways for their acrobatic escapades.\n""There is, however, a growing concern regarding the impact of urban life on squirrel populations. Pollution, deforestation, and the loss of natural habitats are making it more difficult for squirrels to find adequate food and shelter. As a result, conservationists are focusing on creating squirrel-friendly spaces within cities, with the goal of ensuring these resourceful creatures continue to thrive in both rural and urban landscapes.\n""\n""A Symbol of Resilience\n""In many cultures, squirrels are symbols of resourcefulness, adaptability, and preparation. Their ability to thrive in a variety of environments while navigating challenges with agility and grace serves as a reminder of the resilience inherent in nature. Whether you encounter them in a quiet forest, a city park, or your own backyard, squirrels are creatures that never fail to amaze with their endless energy and ingenuity.\n""In the end, squirrels may be small, but they are mighty in their ability to survive and thrive in a world that is constantly changing. So next time you spot one hopping across a branch or darting across your lawn, take a moment to appreciate the remarkable acrobat at work a true marvel of the natural world.\n" },
    { "Lorem Ipsum", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum." },
    { "Vacuum Instructions", "Chapter 3: Getting Started - Unpacking and Setup\n""\n""Congratulations on your new SuperClean Pro 5000 vacuum cleaner! In this section, we will guide you through the simple steps to get your vacuum up and running. Before you begin, please ensure that you have all the components listed in the \"Package Contents\" section on page 2.\n""\n""1. Unboxing Your Vacuum\n""Carefully remove the vacuum cleaner from the box. Avoid using sharp objects that could damage the product. Once removed, place the unit on a flat, stable surface to proceed with the setup. Inside the box, you should find:\n""\n""    The main vacuum unit\n""    A telescoping extension wand\n""    A set of specialized cleaning tools (crevice tool, upholstery brush, etc.)\n""    A reusable dust bag (if applicable)\n""    A power cord with a 3-prong plug\n""    A set of quick-start instructions\n""\n""2. Assembling Your Vacuum\n""Begin by attaching the extension wand to the main body of the vacuum cleaner. Line up the connectors and twist the wand into place until you hear a click. Next, select the desired cleaning tool and firmly attach it to the wand's end, ensuring it is securely locked in.\n""\n""For models that require a dust bag, slide the bag into the compartment at the back of the vacuum, making sure it is properly aligned with the internal mechanism. If your vacuum uses a bagless system, ensure the dust container is correctly seated and locked in place before use.\n""\n""3. Powering On\n""To start the vacuum, plug the power cord into a grounded electrical outlet. Once plugged in, locate the power switch, usually positioned on the side of the handle or body of the unit, depending on your model. Press the switch to the \"On\" position, and you should hear the motor begin to hum. If the vacuum does not power on, check that the power cord is securely plugged in, and ensure there are no blockages in the power switch.\n""\n""Note: Before first use, ensure that the vacuum filter (if your model has one) is properly installed. If unsure, refer to \"Section 5: Maintenance\" for filter installation instructions." },
    { "Article 4", "Article 4" },
    { "Article 5", "Article 5" }  
};

uint selectedDocumentIndex = 0;

RectangleElementConfig contentBackgroundConfig = {
    { 90, 90, 90, 255 },
    clay::@cornerRadiusUniCT(8)
};

Sizing layoutExpand = Sizing{ clay::sizingGrow(), clay::sizingGrow() };

LayoutConfig sidebarButtonLayout = LayoutConfig{
    .sizing = { .width = clay::sizingGrow() },
    .padding = clay::paddingCT(16, 16, 16, 16)
};

bool isDebugModeEnabled = false;

fn void main() @public
{    
    raylib::setConfigFlags(raylib::FLAG_VSYNC_HINT | raylib::FLAG_WINDOW_RESIZABLE | raylib::FLAG_WINDOW_HIGHDPI | raylib::FLAG_MSAA_4X_HINT);
    raylib::initWindow(1024, 768, "Introducing Clay C3-Demo");
    defer raylib::closeWindow();

    uint clayRequiredMemory = clay::minMemorySize();
    Arena clayMemory = clay::createArena(clayRequiredMemory, malloc(clayRequiredMemory));
    clay::initialize(clayMemory, { raylib::getScreenWidth(), raylib::getScreenHeight() }, { &errorHandler, 0 });

    
    clay::setMeasureTextFunction(&clay::renderer::raylibMeasureText);
    clay::renderer::raylibFonts[FONT_ID_BODY_16] = { 
        FONT_ID_BODY_16, 
        raylib::loadFontEx("resources/Lexend-Regular.ttf", 48, null, 400)
    };

    raylib::setTextureFilter(
        clay::renderer::raylibFonts[FONT_ID_BODY_16].font.texture, 
        TextureFilter.BILINEAR
    );

    while (!raylib::windowShouldClose()) {
        clay::setLayoutDimensions({raylib::getScreenWidth(), raylib::getScreenHeight()});
        Vector2 mouse_position = raylib::getMousePosition();
        Vector2 scroll_delta = raylib::getMouseWheelMoveV();
        clay::setPointerState({mouse_position.x, mouse_position.y}, raylib::isMouseButtonDown(MouseButton.LEFT));
        clay::updateScrollContainer(true, {scroll_delta.x, scroll_delta.y}, raylib::getFrameTime());

        if (raylib::isKeyReleased(raylib::KEY_D)) { isDebugModeEnabled = !isDebugModeEnabled; clay::setDebugModeEnabled(isDebugModeEnabled); }

        clay::beginLayout();

        @clay(
            clay::id("OuterContainer"),
            clay::rectangle({ .color = { 43, 41, 51, 255 } }),
            clay::layout({
                .sizing = layoutExpand,
                .padding = clay::paddingUni(16),
                .childGap = 16,
                .layoutDirection = LayoutDirection.TOP_TO_BOTTOM,
            })
        ) {
            @clay(
                clay::id("HeaderBar"),
                clay::rectangle(contentBackgroundConfig),
                clay::layout({
                    .sizing = {
                        .height = clay::sizingFixed(60),
                        .width = clay::sizingGrow()
                    },
                    .padding = { 16, 16, 0, 0 },
                    .childGap = 16,
                    .childAlignment = {
                        .y = AlignY.CENTER
                    }
                })
            ) {
                // Header buttons go here
                @clay(
                    clay::id("FileButton"),
                    clay::layout({ .padding = { 16, 16, 8, 8 }}),
                    clay::rectangle({
                        .color = { 140, 140, 140, 255 },
                        .cornerRadius = clay::cornerRadiusUni(5)
                    })
                ) {
                    clay::text("File", clay::textConfig({
                        .textColor = { 255, 255, 255, 255 },
                        .fontId = FONT_ID_BODY_16,
                        .fontSize = 16,
                    }));

                    bool fileMenuVisible =
                        clay::pointerOver(clay::getElementId("FileButton"))
                        ||
                        clay::pointerOver(clay::getElementId("FileMenu"));

                    if (fileMenuVisible) { // Below has been changed slightly to fix the small bug where the menu would dismiss when mousing over the top gap
                        @clay(
                            clay::id("FileMenu"),
                            clay::floating({
                                .attachment = {
                                    .parent = AttachPoint.LEFT_BOTTOM
                                },
                            }),
                            clay::layout({
                                .padding = {0, 0, 8, 8 }
                            })
                        ) {
                            @clay(
                                clay::layout({
                                    .sizing = {
                                            .width = clay::sizingFixed(200)
                                    },
                                    .layoutDirection = LayoutDirection.TOP_TO_BOTTOM,
                                }),
                                clay::rectangle({
                                    .color = { 40, 40, 40, 255 },
                                    .cornerRadius = clay::cornerRadiusUni(8)
                                })
                            ) {
                                // Render dropdown items here
                                renderDropdownMenuItem("New");
                                renderDropdownMenuItem("Open");
                                renderDropdownMenuItem("Close");
                            };
                        };
                    }
                };
                renderHeaderButton("Edit");
                @clay(clay::layout({ .sizing = { .width = clay::sizingGrow() }})) {};
                renderHeaderButton("Upload");
                renderHeaderButton("Media");
                renderHeaderButton("Support");
            };

            @clay(
                clay::id("LowerContent"),
                clay::layout({ .sizing = layoutExpand, .childGap = 16 })
            ) {
                @clay(
                    clay::id("Sidebar"),
                    contentBackgroundConfig,
                    clay::layout({
                        .sizing = {
                            .width = clay::sizingFixed(250),
                            .height = clay::sizingGrow()
                        },
                        .padding = clay::paddingUni(16),
                        .childGap = 8,
                        .layoutDirection = LayoutDirection.TOP_TO_BOTTOM,
                    })
                ) {
                    for (int i = 0; i < ALL_DOCUMENTS.len; i++) {
                        Document document = ALL_DOCUMENTS[i];

                        if (i == selectedDocumentIndex) {
                            @clay(
                                clay::layout(sidebarButtonLayout),
                                clay::rectangle({
                                    .color = { 120, 120, 120, 255 },
                                    .cornerRadius = clay::cornerRadiusUni(8),
                                })
                            ) {
                                clay::text(document.title, clay::textConfig({
                                    .textColor = { 255, 255, 255, 255 },
                                    .fontId = FONT_ID_BODY_16,
                                    .fontSize = 20,
                                }));
                            };
                        } else {
                            @clay(
                                clay::layout(sidebarButtonLayout),
                                clay::onHover(&handleSidebarInteraction, i),
                                clay::@bodyIf(
                                    clay::hovered(), 
                                    clay::rectangle({
                                        .color = { 120, 120, 120, 120 },
                                        .cornerRadius = clay::cornerRadiusUni(8)
                                    })
                                ),  
                            ) {
                                clay::text(document.title, clay::textConfig({
                                    .textColor = { 255, 255, 255, 255 },
                                    .fontId = FONT_ID_BODY_16,
                                    .fontSize = 20,
                                }));
                            };
                        }
                    }
                };

                @clay(
                    clay::id("MainContent"),
                    clay::rectangle(contentBackgroundConfig),
                    clay::scroll({ .vertical = true }),
                    clay::layout({
                        .layoutDirection = LayoutDirection.TOP_TO_BOTTOM,
                        .childGap = 14,
                        .padding = clay::paddingUni(16),
                        .sizing = layoutExpand,
                    }),
                ) {
                    Document document = ALL_DOCUMENTS[selectedDocumentIndex];
                    clay::text(document.title, clay::textConfig({
                        .fontId = FONT_ID_BODY_16,
                        .fontSize = 24,
                        .textColor = {255, 255, 255, 255},
                    }));
                    clay::text(document.contents, clay::textConfig({
                        .fontId = FONT_ID_BODY_16,
                        .fontSize = 24,
                        .textColor = {255, 255, 255, 255},
                    }));
                };
            };
        };

        RenderCommandArray renderCommands = clay::endLayout();
        

        raylib::beginDrawing();
        raylib::clearBackground(raylib::WHITE);
        clay::renderer::raylibRender(renderCommands);
        raylib::endDrawing();
    }
}
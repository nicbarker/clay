using Basic :: #import "Basic";

Clay :: #import,file "../../clay-jai/module.jai";
Raylib :: #import "raylib-jai";

for_expansion :: Clay.for_expansion;;

#load "clay_renderer_raylib.jai";

FONT_ID_BODY_16 :: 0;
COLOR_WHITE :: Clay.Color.{255, 255, 255, 255};

window_width: s32 = 1024;
window_height: s32 = 768;

Document :: struct {
    title: string;
    contents: string;
}

documents :: Document.[
    .{"Squirrels", "The Secret Life of Squirrels: Nature's Clever Acrobats\nSquirrels are often overlooked creatures, dismissed as mere park inhabitants or backyard nuisances. Yet, beneath their fluffy tails and twitching noses lies an intricate world of cunning, agility, and survival tactics that are nothing short of fascinating. As one of the most common mammals in North America, squirrels have adapted to a wide range of environments from bustling urban centers to tranquil forests and have developed a variety of unique behaviors that continue to intrigue scientists and nature enthusiasts alike.\n\nMaster Tree Climbers\nAt the heart of a squirrel's skill set is its impressive ability to navigate trees with ease. Whether they're darting from branch to branch or leaping across wide gaps, squirrels possess an innate talent for acrobatics. Their powerful hind legs, which are longer than their front legs, give them remarkable jumping power. With a tail that acts as a counterbalance, squirrels can leap distances of up to ten times the length of their body, making them some of the best aerial acrobats in the animal kingdom.\nBut it's not just their agility that makes them exceptional climbers. Squirrels' sharp, curved claws allow them to grip tree bark with precision, while the soft pads on their feet provide traction on slippery surfaces. Their ability to run at high speeds and scale vertical trunks with ease is a testament to the evolutionary adaptations that have made them so successful in their arboreal habitats.\n\nFood Hoarders Extraordinaire\nSquirrels are often seen frantically gathering nuts, seeds, and even fungi in preparation for winter. While this behavior may seem like instinctual hoarding, it is actually a survival strategy that has been honed over millions of years. Known as \"scatter hoarding,\" squirrels store their food in a variety of hidden locations, often burying it deep in the soil or stashing it in hollowed-out tree trunks.\nInterestingly, squirrels have an incredible memory for the locations of their caches. Research has shown that they can remember thousands of hiding spots, often returning to them months later when food is scarce. However, they don't always recover every stash some forgotten caches eventually sprout into new trees, contributing to forest regeneration. This unintentional role as forest gardeners highlights the ecological importance of squirrels in their ecosystems.\n\nThe Great Squirrel Debate: Urban vs. Wild\nWhile squirrels are most commonly associated with rural or wooded areas, their adaptability has allowed them to thrive in urban environments as well. In cities, squirrels have become adept at finding food sources in places like parks, streets, and even garbage cans. However, their urban counterparts face unique challenges, including traffic, predators, and the lack of natural shelters. Despite these obstacles, squirrels in urban areas are often observed using human infrastructure such as buildings, bridges, and power lines as highways for their acrobatic escapades.\nThere is, however, a growing concern regarding the impact of urban life on squirrel populations. Pollution, deforestation, and the loss of natural habitats are making it more difficult for squirrels to find adequate food and shelter. As a result, conservationists are focusing on creating squirrel-friendly spaces within cities, with the goal of ensuring these resourceful creatures continue to thrive in both rural and urban landscapes.\n\nA Symbol of Resilience\nIn many cultures, squirrels are symbols of resourcefulness, adaptability, and preparation. Their ability to thrive in a variety of environments while navigating challenges with agility and grace serves as a reminder of the resilience inherent in nature. Whether you encounter them in a quiet forest, a city park, or your own backyard, squirrels are creatures that never fail to amaze with their endless energy and ingenuity.\nIn the end, squirrels may be small, but they are mighty in their ability to survive and thrive in a world that is constantly changing. So next time you spot one hopping across a branch or darting across your lawn, take a moment to appreciate the remarkable acrobat at work a true marvel of the natural world."},
    .{"Lorem Ipsum", "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."},
    .{"Vacuum Instructions", "Chapter 3: Getting Started - Unpacking and Setup\n\nCongratulations on your new SuperClean Pro 5000 vacuum cleaner! In this section, we will guide you through the simple steps to get your vacuum up and running. Before you begin, please ensure that you have all the components listed in the \"Package Contents\" section on page 2.\n\n1. Unboxing Your Vacuum\nCarefully remove the vacuum cleaner from the box. Avoid using sharp objects that could damage the product. Once removed, place the unit on a flat, stable surface to proceed with the setup. Inside the box, you should find:\n\n    The main vacuum unit\n    A telescoping extension wand\n    A set of specialized cleaning tools (crevice tool, upholstery brush, etc.)\n    A reusable dust bag (if applicable)\n    A power cord with a 3-prong plug\n    A set of quick-start instructions\n\n2. Assembling Your Vacuum\nBegin by attaching the extension wand to the main body of the vacuum cleaner. Line up the connectors and twist the wand into place until you hear a click. Next, select the desired cleaning tool and firmly attach it to the wand's end, ensuring it is securely locked in.\n\nFor models that require a dust bag, slide the bag into the compartment at the back of the vacuum, making sure it is properly aligned with the internal mechanism. If your vacuum uses a bagless system, ensure the dust container is correctly seated and locked in place before use.\n\n3. Powering On\nTo start the vacuum, plug the power cord into a grounded electrical outlet. Once plugged in, locate the power switch, usually positioned on the side of the handle or body of the unit, depending on your model. Press the switch to the \"On\" position, and you should hear the motor begin to hum. If the vacuum does not power on, check that the power cord is securely plugged in, and ensure there are no blockages in the power switch.\n\nNote: Before first use, ensure that the vacuum filter (if your model has one) is properly installed. If unsure, refer to \"Section 5: Maintenance\" for filter installation instructions."},
    .{"Article 4", "Article 4"},
    .{"Article 5", "Article 5"},
];

to_jai_string :: (str: Clay.String) -> string {
    return .{data = str.chars, count = cast(s64, str.length)};
}

handle_clay_errors :: (error_data: Clay.ErrorData) #c_call {
    push_context {
        log_error(
            "Clay Error [%]: % | %", 
            error_data.errorType, 
            to_jai_string(error_data.errorText),
            error_data.userData
        );
    }
}

render_header_button :: (text: string) {
    for Clay.Element(
        Clay.Layout(.{padding = .{16, 8}}),
        Clay.Rectangle(.{
            color = .{140, 140, 140, 255},
            cornerRadius = .{5, 5, 5, 5},
        })
    ) {
        Clay.Text(text, Clay.TextConfig(.{
            fontId = FONT_ID_BODY_16,
            fontSize = 16,
            textColor = COLOR_WHITE,
        }));
    }
}

render_dropdown_menu_item :: (text: string) {
    for Clay.Element(
        Clay.Layout(.{padding = .{16, 16}})
    ) {
        Clay.Text(text, Clay.TextConfig(.{
            fontId = FONT_ID_BODY_16,
            fontSize = 16,
            textColor = COLOR_WHITE,
        }));
    }
}

selected_document_index : int = 0;

main :: () {
    flags := Raylib.ConfigFlags.WINDOW_RESIZABLE | .MSAA_4X_HINT | .VSYNC_HINT;
    raylib_initialize(1024, 768, "Introducing Clay Demo", flags);

    // For some reason, on linux, this is 8 bytes off ???
    clay_required_memory := Clay.MinMemorySize() + 8;
    memory := alloc(clay_required_memory);
    clay_memory := Clay.CreateArenaWithCapacityAndMemory(clay_required_memory, memory);
    Clay.Initialize(
        clay_memory, 
        Clay.Dimensions.{cast(float, Raylib.GetScreenWidth()), cast(float, Raylib.GetScreenHeight())},
        .{handle_clay_errors, 0}
    );

    Clay.SetMeasureTextFunction(raylib_measure_text);
    g_raylib_fonts[FONT_ID_BODY_16] = .{
        FONT_ID_BODY_16,
        Raylib.LoadFontEx("resources/Roboto-Regular.ttf", 48, null, 400),
    };
    Raylib.SetTextureFilter(g_raylib_fonts[FONT_ID_BODY_16].font.texture, .BILINEAR);

    while !Raylib.WindowShouldClose() {
        Clay.SetLayoutDimensions(.{
            cast(float, Raylib.GetScreenWidth()),
            cast(float, Raylib.GetScreenHeight()),
        });

        mouse_position := Raylib.GetMousePosition();
        scroll_delta := Raylib.GetMouseWheelMoveV();
        Clay.SetPointerState(mouse_position, Raylib.IsMouseButtonDown(0));
        Clay.UpdateScrollContainers(true, scroll_delta, Raylib.GetFrameTime());

        layout_expand := Clay.Sizing.{
            Clay.SizingGrow(),
            Clay.SizingGrow(),
        };

        content_background_config := Clay.RectangleElementConfig.{
            color = .{90, 90, 90, 255},
            cornerRadius = .{8, 8, 8, 8},
        };

        Clay.BeginLayout();
        for Clay.Element(
            Clay.ID("OuterContainer"),
            Clay.Rectangle(.{color = .{43, 41, 51, 255}}),
            Clay.Layout(.{
                layoutDirection = .TOP_TO_BOTTOM,
                sizing = layout_expand,
                padding = .{16, 16},
                childGap = 16,
            }),
        ) {
            for Clay.Element(
                Clay.ID("HeaderBar"),
                Clay.Rectangle(content_background_config),
                Clay.Layout(.{
                    sizing = .{
                        height = Clay.SizingFixed(60),
                        width = Clay.SizingGrow(),
                    },
                    padding = .{16, 16},
                    childGap = 16,
                    childAlignment = .{
                        y = .CENTER,
                    }
                }),
            ) {
                for Clay.Element(
                    Clay.ID("FileButton"),
                    Clay.Layout(.{padding = .{16, 8}}),
                    Clay.Rectangle(.{
                        color = .{140, 140, 140, 255},
                        cornerRadius = .{5, 5, 5, 5},
                    }),
                ) {
                    Clay.Text("File", Clay.TextConfig(.{
                        fontId = FONT_ID_BODY_16,
                        fontSize = 16,
                        textColor = COLOR_WHITE,
                    }));

                    file_menu_visible := Clay.PointerOver(Clay.GetElementId("FileButton")) ||
                    Clay.PointerOver(Clay.GetElementId("FileMenu"));

                    if file_menu_visible {
                        for Clay.Element(
                            Clay.ID("FileMenu"),
                            Clay.Floating(.{attachment = .{parent = .LEFT_BOTTOM}}),
                            Clay.Layout(.{padding = .{0, 8}})
                        ) {
                            for Clay.Element(
                                Clay.Layout(.{
                                    layoutDirection=.TOP_TO_BOTTOM,
                                    sizing = .{width = Clay.SizingFixed(200)}
                                }),
                                Clay.Rectangle(.{
                                    color = .{40, 40, 40, 255},
                                    cornerRadius = .{8, 8, 8, 8}
                                })
                            ) {
                                // Render dropdown items here
                                render_dropdown_menu_item("New");
                                render_dropdown_menu_item("Open");
                                render_dropdown_menu_item("Close");
                            }
                        }
                    }
                }

                // Render header buttons
                render_header_button("Edit");
                for Clay.Element(Clay.Layout(.{
                    sizing = .{Clay.SizingGrow(), Clay.SizingGrow()}})) {}
                render_header_button("Upload");
                render_header_button("Media");
                render_header_button("Support");
            }

            for Clay.Element(
                Clay.ID("LowerContent"),
                Clay.Layout(.{sizing = layout_expand, childGap = 16}),
            ) {
                for Clay.Element(
                    Clay.ID("Sidebar"),
                    Clay.Rectangle(content_background_config),
                    Clay.Layout(.{
                        layoutDirection = .TOP_TO_BOTTOM,
                        padding = .{16, 16},
                        childGap = 8,
                        sizing = .{
                            width = Clay.SizingFixed(250),
                            height = Clay.SizingGrow(),
                        }
                    })
                ) {
                    for document : documents {
                        sidebar_button_layout := Clay.LayoutConfig.{
                            sizing = .{width = Clay.SizingGrow()},
                            padding = .{16, 16},
                        };

                        if it_index == selected_document_index {
                            for Clay.Element(
                                Clay.Layout(sidebar_button_layout),
                                Clay.Rectangle(.{
                                    color = .{120, 120, 120, 255},
                                    cornerRadius = .{8, 8, 8, 8},
                                })
                            ) {
                                Clay.Text(document.title, Clay.TextConfig(.{
                                    fontId = FONT_ID_BODY_16,
                                    fontSize = 20,
                                    textColor = COLOR_WHITE,
                                }));
                            }
                        } else {
                            id := tprint("Parnets %", it_index);
                            is_hovered := Clay.PointerOver(Clay.GetElementId(id));
                            if is_hovered && Raylib.IsMouseButtonPressed(0) {
                                selected_document_index = it_index;
                            }

                            for Clay.Element(
                                Clay.ID(id)
                            ) {
                                for Clay.Element(
                                    Clay.Layout(sidebar_button_layout),
                                    ifx is_hovered then Clay.Rectangle(.{
                                        color = .{120, 120, 120, 120},
                                        cornerRadius = .{8, 8, 8, 8},
                                    }) else .{}
                                ) {
                                    Clay.Text(document.title, Clay.TextConfig(.{
                                        fontId = FONT_ID_BODY_16,
                                        fontSize = 20,
                                        textColor = COLOR_WHITE
                                    }));
                                }
                            }
                        }
                    }
                }

                for Clay.Element(
                    Clay.ID("MainContent"),
                    Clay.Rectangle(content_background_config),
                    Clay.Scroll(.{vertical = true}),
                    Clay.Layout(.{
                        layoutDirection = .TOP_TO_BOTTOM,
                        childGap = 16,
                        padding = .{16, 16},
                        sizing = layout_expand,
                    }),
                ) {
                    selected_document := documents[selected_document_index];
                    Clay.Text(selected_document.title, Clay.TextConfig(.{
                        fontId = FONT_ID_BODY_16,
                        fontSize = 24,
                        textColor = COLOR_WHITE,
                    }));
                    Clay.Text(selected_document.contents, Clay.TextConfig(.{
                        fontId = FONT_ID_BODY_16,
                        fontSize = 24,
                        textColor = COLOR_WHITE
                    }));
                }
            }
        }

        render_commands := Clay.EndLayout();

        Raylib.BeginDrawing();
        Raylib.ClearBackground(Raylib.BLACK);
        clay_raylib_render(render_commands);
        Raylib.EndDrawing();

        reset_temporary_storage();
    }
}
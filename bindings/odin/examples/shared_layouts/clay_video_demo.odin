// Mimicing the project structure for C examples
package shared_layouts

import clay "../../clay-odin"

VIDEO_DEMO_FONT_ID_BODY :: 0

@(private="file")
COLOR_WHITE := clay.Color{255, 255, 255, 255}

@(private="file")
header_button :: proc(text: string) {
    if clay.UI()({layout = {padding = {16, 16, 8, 8}}, backgroundColor = {140, 140, 140, 255}, cornerRadius = clay.CornerRadiusAll(5)}) {
        clay.TextDynamic(text, clay.TextConfig({fontId = VIDEO_DEMO_FONT_ID_BODY, fontSize = 16, textColor = {255, 255, 255, 255}}))
    }
}

@(private="file")
dropdown_menu_item :: proc(text: string) {
    if clay.UI()({layout = {padding = clay.PaddingAll(16)}}) {
        clay.TextDynamic(text, clay.TextConfig({fontId = VIDEO_DEMO_FONT_ID_BODY, fontSize = 16, textColor = {255, 255, 255, 255}}))
    }
}

@(private="file")
Document :: struct {
    title:    string,
    contents: string,
}

@(private="file")
documents := [5]Document{}

@(private="file")
Video_Demo_Data :: struct {
    selectedDocumentIndex: int,
    yOffset:               f32,
}

@(private="file")
Sidebar_Click_Data :: struct {
    requestedDocumentIndex: int,
    selectedDocumentIndex:  ^int,
}

@(private="file")
sidebar_callback :: proc "c" (elementId: clay.ElementId, pointerData: clay.PointerData, userData: rawptr) {
    clickData := (^Sidebar_Click_Data)(userData)
    // If this button was clicked
    if (pointerData.state == .PressedThisFrame) {
        if (clickData.requestedDocumentIndex >= 0 && clickData.requestedDocumentIndex < len(documents)) {
            // Select the corresponding document
            clickData.selectedDocumentIndex^ = clickData.requestedDocumentIndex
        }
    }
}

video_demo_init :: proc() -> Video_Demo_Data {
    documents[0] = Document {
        title    = "Squirrels",
        contents = "The Secret Life of Squirrels: Nature's Clever Acrobats\nSquirrels are often overlooked creatures, dismissed as mere park inhabitants or backyard nuisances. Yet, beneath their fluffy tails and twitching noses lies an intricate world of cunning, agility, and survival tactics that are nothing short of fascinating. As one of the most common mammals in North America, squirrels have adapted to a wide range of environments from bustling urban centers to tranquil forests and have developed a variety of unique behaviors that continue to intrigue scientists and nature enthusiasts alike.\n\nMaster Tree Climbers\nAt the heart of a squirrel's skill set is its impressive ability to navigate trees with ease. Whether they're darting from branch to branch or leaping across wide gaps, squirrels possess an innate talent for acrobatics. Their powerful hind legs, which are longer than their front legs, give them remarkable jumping power. With a tail that acts as a counterbalance, squirrels can leap distances of up to ten times the length of their body, making them some of the best aerial acrobats in the animal kingdom.\nBut it's not just their agility that makes them exceptional climbers. Squirrels' sharp, curved claws allow them to grip tree bark with precision, while the soft pads on their feet provide traction on slippery surfaces. Their ability to run at high speeds and scale vertical trunks with ease is a testament to the evolutionary adaptations that have made them so successful in their arboreal habitats.\n\nFood Hoarders Extraordinaire\nSquirrels are often seen frantically gathering nuts, seeds, and even fungi in preparation for winter. While this behavior may seem like instinctual hoarding, it is actually a survival strategy that has been honed over millions of years. Known as \"scatter hoarding,\" squirrels store their food in a variety of hidden locations, often burying it deep in the soil or stashing it in hollowed-out tree trunks.\nInterestingly, squirrels have an incredible memory for the locations of their caches. Research has shown that they can remember thousands of hiding spots, often returning to them months later when food is scarce. However, they don't always recover every stash some forgotten caches eventually sprout into new trees, contributing to forest regeneration. This unintentional role as forest gardeners highlights the ecological importance of squirrels in their ecosystems.\n\nThe Great Squirrel Debate: Urban vs. Wild\nWhile squirrels are most commonly associated with rural or wooded areas, their adaptability has allowed them to thrive in urban environments as well. In cities, squirrels have become adept at finding food sources in places like parks, streets, and even garbage cans. However, their urban counterparts face unique challenges, including traffic, predators, and the lack of natural shelters. Despite these obstacles, squirrels in urban areas are often observed using human infrastructure such as buildings, bridges, and power lines as highways for their acrobatic escapades.\nThere is, however, a growing concern regarding the impact of urban life on squirrel populations. Pollution, deforestation, and the loss of natural habitats are making it more difficult for squirrels to find adequate food and shelter. As a result, conservationists are focusing on creating squirrel-friendly spaces within cities, with the goal of ensuring these resourceful creatures continue to thrive in both rural and urban landscapes.\n\nA Symbol of Resilience\nIn many cultures, squirrels are symbols of resourcefulness, adaptability, and preparation. Their ability to thrive in a variety of environments while navigating challenges with agility and grace serves as a reminder of the resilience inherent in nature. Whether you encounter them in a quiet forest, a city park, or your own backyard, squirrels are creatures that never fail to amaze with their endless energy and ingenuity.\nIn the end, squirrels may be small, but they are mighty in their ability to survive and thrive in a world that is constantly changing. So next time you spot one hopping across a branch or darting across your lawn, take a moment to appreciate the remarkable acrobat at work a true marvel of the natural world.\n",
    }
    documents[1] = Document {
        title    = "Lorem Ipsum",
        contents = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.",
    }
    documents[2] = Document {
        title    = "Vacuum Instructions",
        contents = "Chapter 3: Getting Started - Unpacking and Setup\n\nCongratulations on your new SuperClean Pro 5000 vacuum cleaner! In this section, we will guide you through the simple steps to get your vacuum up and running. Before you begin, please ensure that you have all the components listed in the \"Package Contents\" section on page 2.\n\n1. Unboxing Your Vacuum\nCarefully remove the vacuum cleaner from the box. Avoid using sharp objects that could damage the product. Once removed, place the unit on a flat, stable surface to proceed with the setup. Inside the box, you should find:\n\n    The main vacuum unit\n    A telescoping extension wand\n    A set of specialized cleaning tools (crevice tool, upholstery brush, etc.)\n    A reusable dust bag (if applicable)\n    A power cord with a 3-prong plug\n    A set of quick-start instructions\n\n2. Assembling Your Vacuum\nBegin by attaching the extension wand to the main body of the vacuum cleaner. Line up the connectors and twist the wand into place until you hear a click. Next, select the desired cleaning tool and firmly attach it to the wand's end, ensuring it is securely locked in.\n\nFor models that require a dust bag, slide the bag into the compartment at the back of the vacuum, making sure it is properly aligned with the internal mechanism. If your vacuum uses a bagless system, ensure the dust container is correctly seated and locked in place before use.\n\n3. Powering On\nTo start the vacuum, plug the power cord into a grounded electrical outlet. Once plugged in, locate the power switch, usually positioned on the side of the handle or body of the unit, depending on your model. Press the switch to the \"On\" position, and you should hear the motor begin to hum. If the vacuum does not power on, check that the power cord is securely plugged in, and ensure there are no blockages in the power switch.\n\nNote: Before first use, ensure that the vacuum filter (if your model has one) is properly installed. If unsure, refer to \"Section 5: Maintenance\" for filter installation instructions.",
    }
    documents[3] = Document {
        title    = "Article 4",
        contents = "Article 4",
    }
    documents[4] = Document {
        title    = "Article 5",
        contents = "Article 5",
    }

    data := Video_Demo_Data{}
    return data
}

video_demo_layout :: proc(data: ^Video_Demo_Data) -> clay.ClayArray(clay.RenderCommand) {
    free_all(context.temp_allocator)

    clay.BeginLayout()

    layoutExpand := clay.Sizing {
        width  = clay.SizingGrow(),
        height = clay.SizingGrow(),
    }

    contentBackgroundColor := clay.Color{90, 90, 90, 255}

    // Build UI here
    if clay.UI(clay.ID("OuterContainer"))(
    {backgroundColor = {43, 41, 51, 255}, layout = {layoutDirection = .TopToBottom, sizing = layoutExpand, padding = clay.PaddingAll(16), childGap = 16}},
    ) {
        // Child elements go inside braces
        if clay.UI(clay.ID("HeaderBar"))(
        {
            layout = {sizing = {height = clay.SizingFixed(60), width = clay.SizingFixed(0)}, padding = {16, 16, 0, 0}, childGap = 16, childAlignment = {y = .Center}},
            backgroundColor = contentBackgroundColor,
            cornerRadius = clay.CornerRadiusAll(8),
        },
        ) {
            // Header buttons go here
            if clay.UI(clay.ID("FileButton"))({layout = {padding = {16, 16, 8, 8}}, backgroundColor = {140, 140, 140, 255}, cornerRadius = clay.CornerRadiusAll(5)}) {
                clay.TextDynamic("File", clay.TextConfig({fontId = VIDEO_DEMO_FONT_ID_BODY, fontSize = 16, textColor = {255, 255, 255, 255}}))

                fileMenuVisible := clay.PointerOver(clay.GetElementId(clay.MakeString("FileButton"))) || clay.PointerOver(clay.GetElementId(clay.MakeString("FileMenu")))

                if (fileMenuVisible) {     // Below has been changed slightly to fix the small bug where the menu would dismiss when mousing over the top gap
                    if clay.UI(clay.ID("FileMenu"))({floating = {attachTo = .Parent, attachment = {parent = .LeftBottom}}, layout = {padding = {0, 0, 8, 8}}}) {
                        if clay.UI()(
                        {
                            layout = {layoutDirection = .TopToBottom, sizing = {width = clay.SizingFixed(200)}},
                            backgroundColor = {40, 40, 40, 255},
                            cornerRadius = clay.CornerRadiusAll(8),
                        },
                        ) {
                            // Render dropdown items here
                            dropdown_menu_item("New")
                            dropdown_menu_item("Open")
                            dropdown_menu_item("Close")
                        }
                    }
                }
            }
            header_button("Edit")
            if clay.UI()({layout = {sizing = {width = clay.SizingGrow()}}}) {}
            header_button("Upload")
            header_button("Media")
            header_button("Support")
        }

        if clay.UI(clay.ID("LowerContent"))( {layout = {sizing = layoutExpand, childGap = 16}}) {
            if clay.UI(
                clay.ID("Sidebar"))(
                {
                    backgroundColor = contentBackgroundColor,
                    layout = {layoutDirection = .TopToBottom, padding = clay.PaddingAll(16), childGap = 8, sizing = {width = clay.SizingFixed(250), height = clay.SizingGrow()}},
                },
            ) {
                for document, i in documents {
                    sidebarButtonLayout := clay.LayoutConfig {
                        sizing = {width = clay.SizingGrow()},
                        padding = clay.PaddingAll(16),
                    }

                    if (i == data.selectedDocumentIndex) {
                        if clay.UI()({layout = sidebarButtonLayout, backgroundColor = {120, 120, 120, 255}, cornerRadius = clay.CornerRadiusAll(8)}) {
                            clay.TextDynamic(document.title, clay.TextConfig({fontId = VIDEO_DEMO_FONT_ID_BODY, fontSize = 20, textColor = {255, 255, 255, 255}}))
                        }
                    } else {
                        clickData := new_clone((Sidebar_Click_Data) {
                            requestedDocumentIndex = i,
                            selectedDocumentIndex = &data.selectedDocumentIndex,
                        }, context.temp_allocator)
                        if clay.UI()(
                        {layout = sidebarButtonLayout, backgroundColor = (clay.Color){120, 120, 120, clay.Hovered() ? 120 : 0}, cornerRadius = clay.CornerRadiusAll(8)},
                        ) {
                            clay.OnHover(sidebar_callback, rawptr(clickData))
                            clay.TextDynamic(document.title, clay.TextConfig({fontId = VIDEO_DEMO_FONT_ID_BODY, fontSize = 20, textColor = {255, 255, 255, 255}}))
                        }
                    }
                }
            }

            if clay.UI(
                clay.ID("MainContent"))(
                {
                    backgroundColor = contentBackgroundColor,
                    clip = {vertical = true, childOffset = clay.GetScrollOffset()},
                    layout = {layoutDirection = .TopToBottom, childGap = 16, padding = clay.PaddingAll(16), sizing = layoutExpand},
                },
            ) {
                selectedDocument := documents[data.selectedDocumentIndex]
                clay.TextDynamic(selectedDocument.title, clay.TextConfig({fontId = VIDEO_DEMO_FONT_ID_BODY, fontSize = 24, textColor = COLOR_WHITE}))
                clay.TextDynamic(selectedDocument.contents, clay.TextConfig({fontId = VIDEO_DEMO_FONT_ID_BODY, fontSize = 24, textColor = COLOR_WHITE}))
            }
        }
    }

    renderCommands := clay.EndLayout()
    for i in 0 ..< renderCommands.length {
        clay.RenderCommandArray_Get(&renderCommands, i).boundingBox.y += data.yOffset
    }
    return renderCommands
}

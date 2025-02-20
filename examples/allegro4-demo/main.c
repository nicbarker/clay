#include "renderers/alleg4/alleg4.h"
#include <stdio.h>
#include <math.h>

// Locked 50 FPS
#define TICKRATE_MS 20
#define DEG_TO_RAD(x) ((x) / 180.0 * M_PI)

static volatile int pending_main_loop_update = 0;

void main_loop_ticker() {
  pending_main_loop_update = 1;
}
END_OF_FUNCTION(main_loop_ticker);

static const int FONT_ID_BODY_16 = 0;
static const int FONT_ID_LARGE_TITLE = 1;
static Clay_Color COLOR_BLACK = { 0, 0, 0, 255 };
static Clay_Color COLOR_WHITE = { 255, 255, 255, 255 };
static Clay_Color COLOR_RED = { 255, 0, 0, 255 };

#define EXPANDING_LAYOUT { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) }

#define BORDER_DEPTH { .width = { 1, 2, 1, 2, 0 }, .color = COLOR_BLACK }

#define BEVELED(PADDING, BG, WIDTH, ...) \
  CLAY({ \
    .backgroundColor = BG, \
    .border = { .width = { 1, 0, 1, 0, 0 }, .color = COLOR_WHITE }, \
    .layout.sizing.height = CLAY_SIZING_GROW(0), \
    .layout.sizing.width = WIDTH > 0 ? CLAY_SIZING_FIXED(WIDTH) : WIDTH == 0 ? CLAY_SIZING_GROW() : CLAY_SIZING_FIT() \
  }) { \
    CLAY({ \
      .layout.padding.left = PADDING, \
      .layout.padding.right = PADDING, \
      .layout.sizing.width = CLAY_SIZING_GROW(), \
      .layout.sizing.height = CLAY_SIZING_GROW(), \
      .layout.childAlignment.x = CLAY_ALIGN_X_CENTER, \
      .layout.childAlignment.y = CLAY_ALIGN_Y_CENTER, \
      .border = { .width = { 0, 1, 0, 1, 0 }, .color = { 128, 128, 128, 255 }, } \
    }) { \
      __VA_ARGS__ \
    } \
  }

static struct {
  BITMAP *lib_logo;
  BITMAP *environment_category;
  BITMAP *coral_reef;
} R;

static void draw_bouncy_ball(BITMAP *buffer, Clay_BoundingBox box, void *user_data) {
#define BALL_R 4
#define BORDER 1
  static int x = BALL_R+BORDER, y = BALL_R+BORDER;
  static Clay_Vector2 dir = { 1, 1 };

  circlefill(buffer, box.x + x, box.y + y, BALL_R, ALLEGCOLOR(COLOR_RED));

  x += dir.x;
  y += dir.y;

  if (x <= BALL_R+BORDER || x >= box.width - (BALL_R+BORDER)) {
    dir.x *= -1;
  }
  if (y <= BALL_R+BORDER || y >= box.height - (BALL_R+BORDER)) {
    dir.y *= -1;
  }
}

static Clay_String test_article;

typedef struct {
  intptr_t offset;
  intptr_t memory;
} EncartaDemo_Arena;

typedef struct {
  EncartaDemo_Arena frameArena;
  unsigned int counter;
} EncartaDemo_Data;

EncartaDemo_Data EncartaDemo_Initialize() {
	test_article = CLAY_STRING("A coral reef is an underwater ecosystem characterized by reef-building corals. Reefs are formed of colonies of coral polyps held together by calcium carbonate. Most coral reefs are built from stony corals, whose polyps cluster in groups.\n\nCoral belongs to the class Anthozoa in the animal phylum Cnidaria, which includes sea anemones and jellyfish. Unlike sea anemones, corals secrete hard carbonate exoskeletons that support and protect the coral. Most reefs grow best in warm, shallow, clear, sunny and agitated water. Coral reefs first appeared 485 million years ago, at the dawn of the Early Ordovician, displacing the microbial and sponge reefs of the Cambrian.\n\nSometimes called rainforests of the sea, shallow coral reefs form some of Earth's most diverse ecosystems. They occupy less than 0.1% of the world's ocean area, about half the area of France, yet they provide a home for at least 25% of all marine species, including fish, mollusks, worms, crustaceans, echinoderms, sponges, tunicates and other cnidarians. Coral reefs flourish in ocean waters that provide few nutrients. They are most commonly found at shallow depths in tropical waters, but deep water and cold water coral reefs exist on smaller scales in other areas.\n\nShallow tropical coral reefs have declined by 50% since 1950, partly because they are sensitive to water conditions. They are under threat from excess nutrients (nitrogen and phosphorus), rising ocean heat content and acidification, overfishing (e.g., from blast fishing, cyanide fishing, spearfishing on scuba), sunscreen use, and harmful land-use practices, including runoff and seeps (e.g., from injection wells and cesspools).\n\nCoral reefs deliver ecosystem services for tourism, fisheries and shoreline protection. The annual global economic value of coral reefs has been estimated at anywhere from US$30-375 billion (1997 and 2003 estimates) to US$2.7 trillion (a 2020 estimate) to US$9.9 trillion (a 2014 estimate).\n\nThough the shallow water tropical coral reefs are best known, there are also deeper water reef-forming corals, which live in colder water and in temperate seas.\n\n\n(( Material ))\n\nAs the name implies, coral reefs are made up of coral skeletons from mostly intact coral colonies. As other chemical elements present in corals become incorporated into the calcium carbonate deposits, aragonite is formed. However, shell fragments and the remains of coralline algae such as the green-segmented genus Halimeda can add to the reef's ability to withstand damage from storms and other threats. Such mixtures are visible in structures such as Eniwetok Atoll.");

  EncartaDemo_Data data = {
    .frameArena = { .memory = (intptr_t)malloc(1024) },
    .counter = 0
  };
  return data;
}

void BeveledButton(Clay_String text, int padding, int fixedWidth) {
  BEVELED(padding, Clay_Hovered() ? ((Clay_Color) { 224, 224, 224, 255 }) : ((Clay_Color) { 192, 192, 192, 255 }), fixedWidth, {
    CLAY_TEXT(text, CLAY_TEXT_CONFIG({
      .fontId = FONT_ID_BODY_16,
      .fontSize = 16,
      .textColor = COLOR_BLACK,
      .textAlignment = CLAY_TEXT_ALIGN_CENTER,
      .lineHeight = 12
    }));
  })
}

static void menubar(EncartaDemo_Data *data) {
  CLAY({
    .id = CLAY_ID("menubar"),
    .backgroundColor = {64, 64, 64, 255 },
    .border = {
      .width = { 0, 0, 0, 1, 0 },
      .color = COLOR_BLACK
    },
    .layout = {
      .sizing = {
        .height = CLAY_SIZING_FIXED(24),
        .width = CLAY_SIZING_GROW(0)
      },
    }
  }) {
    BeveledButton(CLAY_STRING("Menu"), 12, -1);
    BeveledButton(CLAY_STRING("Contents"), 12, -1);
    BeveledButton(CLAY_STRING("Find"), 12, -1);
    BeveledButton(CLAY_STRING("Go Back"), 12, -1);
    BeveledButton(CLAY_STRING("Gallery"), 12, -1);
    BeveledButton(CLAY_STRING("Atlas"), 12, -1);
    BeveledButton(CLAY_STRING("Timeline"), 12, -1);
    BeveledButton(CLAY_STRING("Help"), 12, -1);
    BEVELED(0, ((Clay_Color) { 192, 192, 192, 255 }), 0, { });
  }
}

static void largeArticleHeading(EncartaDemo_Data *data) {
  CLAY({
    .id = CLAY_ID("titlebar"),
    .layout = {
      .sizing.width = CLAY_SIZING_GROW(),
      .padding = { 8, 8, 8, 0 },
    }
  }) {
    CLAY({
      .backgroundColor = {255, 255, 128, 255 },
      .cornerRadius = { 12, 0, 0, 12 },
      .layout = {
        .sizing = {
          .height = CLAY_SIZING_GROW(),
          .width = CLAY_SIZING_GROW()
        },
        .childAlignment = { .x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER }
      },
      .border = { .width = CLAY_BORDER_ALL(1), .color = COLOR_BLACK }
    }) {
      CLAY_TEXT(CLAY_STRING("Coral Reef"), CLAY_TEXT_CONFIG({
        .fontId = FONT_ID_LARGE_TITLE,
        .textColor = COLOR_BLACK
      }));
    }
  }
}

static void demoTitleBox(EncartaDemo_Data *data) {
  CLAY({
    .id = CLAY_ID("DemoHeader"),
    .backgroundColor = COLOR_WHITE,
    .layout.sizing.width = CLAY_SIZING_FIXED(250),
    .layout.padding = CLAY_PADDING_ALL(5),
    .layout.childGap = 5,
    .border = BORDER_DEPTH,
    .cornerRadius = { 8, 8, 0, 0 }
  }) {
    CLAY({
      .id = CLAY_ID("LibraryLogo"),
      .layout = { .sizing = CLAY_SIZING_FIXED(28) },
      .image = { .imageData = R.lib_logo, .sourceDimensions = {28, 28} },
      .backgroundColor = { 128+sin(DEG_TO_RAD(data->counter))*127, 0, 128+sin(DEG_TO_RAD(data->counter/2))*127, 128 }
    }) {}

    CLAY_TEXT(CLAY_STRING("Clay Encarta 95:\nAllegro4 Demo"), CLAY_TEXT_CONFIG({
      .fontId = FONT_ID_BODY_16,
      .textColor = COLOR_BLACK
    }));
  }
}

static void categoryBox(EncartaDemo_Data *data) {
  CLAY({
    .id = CLAY_ID("CategoryImage"),
    .backgroundColor = { 90, 90, 90, 255 },
    .layout = {
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .sizing = {
        .width = CLAY_SIZING_FIXED(250),
        .height = CLAY_SIZING_FIT()
      }
    },
    .border = BORDER_DEPTH
  }) {
    CLAY({
      .image = {
        .imageData = R.environment_category,
        .sourceDimensions = {250, 120}
      },
      .layout.sizing.height = CLAY_SIZING_FIXED(120),
      .layout.padding = CLAY_PADDING_ALL(6),
      .layout.childAlignment.y = CLAY_ALIGN_Y_BOTTOM
    }) {
      CLAY_TEXT(CLAY_STRING("Environment"), CLAY_TEXT_CONFIG({
        .fontId = FONT_ID_LARGE_TITLE,
        .textColor = COLOR_BLACK
      }));
    }

    CLAY({
      .backgroundColor = {64, 64, 64, 255 },
      .layout = {
        .padding = { 1, 2, 1, 2 },
        .sizing = {
          .width = CLAY_SIZING_GROW(),
          .height = CLAY_SIZING_FIT(32),
        }
      }
    }) {
      BeveledButton(CLAY_STRING("Show List"), 8, 0);
      BeveledButton(CLAY_STRING("Change\nCategory"), 8, 0);
      BeveledButton(CLAY_STRING("<"), 8, 28);
      BeveledButton(CLAY_STRING(">"), 8, 28);
    }
  }
}

static void articleBox(EncartaDemo_Data *data) {
  CLAY({
    .id = CLAY_ID("ArticleImage"),
    .backgroundColor = { 90, 90, 90, 255 },
    .layout = {
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .sizing.width = CLAY_SIZING_FIXED(250)
    },
    .border = BORDER_DEPTH
  }) {
    CLAY({
      .image = {
        .imageData = R.coral_reef,
        .sourceDimensions = {250, 230}
      },
      .layout.sizing.height = CLAY_SIZING_FIXED(230)
    }) {}

    CLAY({
      .layout.sizing.height = CLAY_SIZING_FIXED(20),
      .layout.sizing.width = CLAY_SIZING_GROW(),
      .layout.padding = { 1, 1, 1, 0 },
    }) {
      BEVELED(0, ((Clay_Color) { 192, 192, 192, 255 }), 0, {
        CLAY_TEXT(CLAY_STRING("Biodiversity of a coral reef"), CLAY_TEXT_CONFIG({
          .fontId = -1,
          .textColor = COLOR_BLACK
        }));
      });
    }

    CLAY({
      .backgroundColor = {64, 64, 64, 255 },
      .layout = {
        .padding = { 1, 2, 1, 2 },
        .sizing = {
          .height = CLAY_SIZING_FIT(32),
          .width = CLAY_SIZING_GROW()
        }
      }
    }) {
      BeveledButton(CLAY_STRING("Copy"), 8, 0);
      BeveledButton(CLAY_STRING("Print"), 8, 0);
      BeveledButton(CLAY_STRING("<"), 8, 28);
      BeveledButton(CLAY_STRING(">"), 8, 28);
    }
  }
}

static void sidebar(EncartaDemo_Data *data) {
  CLAY({
    .id = CLAY_ID("sidebar"),
    .layout = {
      .childGap = 8,
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .sizing.width = CLAY_SIZING_FIT()
    }
  }) {
    demoTitleBox(data);
    categoryBox(data);
    articleBox(data);
  }
}

static void article(EncartaDemo_Data *data) {
  CLAY({
    .id = CLAY_ID("article"),
    .layout = {
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .sizing = EXPANDING_LAYOUT
    },
    .backgroundColor = COLOR_WHITE,
    .border = BORDER_DEPTH,
    .cornerRadius = { 8, 8, 0, 0 },
  }) {
    CLAY({
      .scroll.vertical = true,
      .layout.padding = CLAY_PADDING_ALL(8),
    }) {
      CLAY_TEXT(test_article, CLAY_TEXT_CONFIG({
        .fontId = FONT_ID_BODY_16,
        .textColor = COLOR_BLACK
      }));
    }

    CLAY({
      .backgroundColor = {64, 64, 64, 255 },
      .layout = {
        .padding = { 1, 2, 1, 2 },
        .sizing = {
          .height = CLAY_SIZING_FIT(32),
          .width = CLAY_SIZING_GROW()
        }
      }
    }) {
      BeveledButton(CLAY_STRING("Outline"), 8, 0);
      BeveledButton(CLAY_STRING("See Also"), 8, 0);
      BeveledButton(CLAY_STRING("Copy"), 8, 0);
      BeveledButton(CLAY_STRING("Print"), 8, 0);
      BEVELED(0, ((Clay_Color) { 192, 192, 192, 255 }), 64, {
        alleg4_custom_element *ball_renderer = (alleg4_custom_element *)(data->frameArena.memory + data->frameArena.offset);
        data->frameArena.offset += sizeof(alleg4_custom_element);
        *ball_renderer = (alleg4_custom_element) {
          &draw_bouncy_ball, NULL
        };
        CLAY({
          .layout = { .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } },
          .custom = { .customData = ball_renderer }
        }) { }
      })
    }
  }
}

Clay_RenderCommandArray EncartaDemo_CreateLayout(EncartaDemo_Data *data) {
  data->frameArena.offset = 0;

  Clay_BeginLayout();

  // Build UI here
  CLAY({
    .id = CLAY_ID("outer-container"),
    .backgroundColor = { 64, 64, 64, 255 },
    .layout = {
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .sizing = EXPANDING_LAYOUT
    }
  }) {
    menubar(data);
    largeArticleHeading(data);

    CLAY({
      .id = CLAY_ID("lower-content"),
      .backgroundColor = { 64, 64, 64, 255 },
      .layout = {
        .sizing = EXPANDING_LAYOUT,
        .childGap = 8,
        .padding = CLAY_PADDING_ALL(8)
      }
    }) {
      sidebar(data);
      article(data);
    }
  }

  return Clay_EndLayout();
}

void HandleClayErrors(Clay_ErrorData errorData) {
  printf("%s", errorData.errorText.chars);
}

int main(int argc, char *argv[]) {
  EncartaDemo_Data demoData = EncartaDemo_Initialize();
  BITMAP *buffer;

  if (allegro_init() != 0) {
    return 1;
  }

  install_keyboard();
  install_mouse();
  install_timer();
  set_color_depth(16);

  int driver = GFX_AUTODETECT;

#ifdef _WIN32
  driver = GFX_AUTODETECT_WINDOWED;
#endif

  set_window_title("Clay Encarta 95");

  if (set_gfx_mode(driver, 800, 600, 0, 0) != 0) {
    if (set_gfx_mode(GFX_SAFE, 800, 600, 0, 0) != 0) {
      set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
      allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
      return 2;
    }
  }

  if (install_int(main_loop_ticker, TICKRATE_MS) < 0) {
    allegro_message("Error installing interrupt\n%s\n", allegro_error);
    return 4;
  }

  LOCK_VARIABLE(pending_main_loop_update);
  LOCK_FUNCTION(main_loop_ticker);

  alleg4_init_fonts(2); /* Allocate space for 1 font */
  alleg4_set_font(FONT_ID_BODY_16, load_font("res/8x16.pcx", NULL, NULL));
  alleg4_set_font(FONT_ID_LARGE_TITLE, load_font("res/ex06.pcx", NULL, NULL));

  buffer = create_bitmap(SCREEN_W, SCREEN_H);

  R.lib_logo = load_bitmap("res/liblogo.pcx", NULL);
  R.environment_category = load_bitmap("res/env.pcx", NULL);
  R.coral_reef = load_bitmap("res/coral.pcx", NULL);

  uint64_t totalMemorySize = Clay_MinMemorySize();
  Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));

  Clay_Initialize(clayMemory, (Clay_Dimensions) {
    (float)SCREEN_W, (float)SCREEN_H
  }, (Clay_ErrorHandler) {
    HandleClayErrors
  });
  Clay_SetMeasureTextFunction(alleg4_measure_text, NULL);

  show_os_cursor(MOUSE_CURSOR_ARROW);
  enable_hardware_cursor();

  int _mouse_w = mouse_w, _mouse_z = mouse_z;

  while (!keypressed()) {
    if (!pending_main_loop_update) {
      continue;
    }

    pending_main_loop_update = false;

    clear_to_color(buffer, makecol(0, 0, 0));

    if (mouse_needs_poll()) {
      poll_mouse();
    }

    Clay_SetLayoutDimensions((Clay_Dimensions) {
      (float)SCREEN_W, (float)SCREEN_H
    });
    Clay_SetPointerState((Clay_Vector2) {
      mouse_x, mouse_y
    }, mouse_b & 1);
    Clay_UpdateScrollContainers(true, (Clay_Vector2) {
      mouse_w - _mouse_w, mouse_z - _mouse_z
    }, (float)TICKRATE_MS / 1000.f);

    _mouse_w = mouse_w;
    _mouse_z = mouse_z;

    Clay_RenderCommandArray renderCommands = EncartaDemo_CreateLayout(&demoData);
    alleg4_render(buffer, renderCommands);

    demoData.counter ++;

    if (gfx_capabilities & GFX_HW_CURSOR) {
      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    } else {
      show_mouse(NULL);
      blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      show_mouse(screen);
    }
  }

  return 0;
}
END_OF_MAIN()

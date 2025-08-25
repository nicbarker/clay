#ifndef SOKOL_CLAY_INCLUDED
#define SOKOL_CLAY_INCLUDED (1)
/*
    sokol_clay.h -- drop-in Clay renderer for sokol_gfx.h

    Do this:
        #define SOKOL_CLAY_IMPL

    before you include this file in *one* C file to create the
    implementation.

    Optionally provide the following configuration define both before including the
    the declaration and implementation:

    SOKOL_CLAY_NO_SOKOL_APP    - don't depend on sokol_app.h (see below for details)

    Include the following headers before sokol_clay.h (both before including
    the declaration and implementation):

        sokol_gl.h
        sokol_fontstash.h
        sokol_app.h         (except SOKOL_CLAY_NO_SOKOL_APP)
        clay.h

    FEATURE OVERVIEW:
    =================
    sokol_clay.h implements the rendering and event-handling code for Clay
    (https://github.com/nicbarker/clay) on top of sokol_gl.h and (optionally)
    sokol_app.h.

    Since sokol_fontstash.h already depends on sokol_gl.h, the rendering is
    implemented using sokol_gl calls. (TODO: make fontstash optional?)

    The sokol_app.h dependency is optional and used for input event handling.
    If you only use sokol_gfx.h but not sokol_app.h in your application,
    define SOKOL_CLAY_NO_SOKOL_APP before including the implementation
    of sokol_clay.h, this will remove any dependency to sokol_app.h, but
    you must call sclay_set_layout_dimensions and handle input yourself.

    sokol_clay.h is not thread-safe, all calls must be made from the
    same thread where sokol_gfx.h is running.

    HOWTO:
    ======

    --- To initialize sokol-clay, call sclay_setup(). This can be done
        before or after Clay_Initialize.

    --- Create an array of sclay_font_t and fill it by calling one of:

            sclay_font_t sclay_add_font(const char *filename);
            sclay_font_t sclay_add_font_mem(unsigned char *data, int dataLen);

        The fontId value in Clay corresponds to indices in this array. After calling
        Clay_Initialize but before calling any layout code, do this:

            Clay_SetMeasureTextFunction(sclay_measure_text, &fonts);

        where `fonts` is the abovementioned array.

    --- At the start of a frame, call sclay_new_frame() if you're using sokol_app.h.
        If you're not using sokol_app.h, call:
 
            void sclay_set_layout_dimensions(Clay_Dimensions size, float dpi_scale);

        at the start of the frame (or just when the window is resized.)

        Either way, do some layout, then at the end of the frame call sclay_render:

            sg_begin_pass(...)
            // other rendering...
            sclay_render(renderCommands, &fonts);
            // other rendering...
            sgl_draw();
            sg_end_pass();
            sg_commit();

        One caveat: sclay_render assumes the default gl view matrix, and handles scaling
        automatically. If you've adjusted the view matrix, remember to first call:

            sgl_matrix_mode_modelview();
            sgl_load_identity();

        before calling sclay_render.

    --- if you're using sokol_app.h, from inside the sokol_app.h event callback,
        call:

            void sclay_handle_event(const sapp_event* ev);

        Unfortunately Clay does not currently provide feedback on whether a mouse
        click was handled or not.

    --- if you want to use images with clay, you should pass a pointer to a
        sclay_image to the CLAY macro, like this:
                CLAY({
                   ...
                   .image = { .imageData = &(sclay_image){ .view = view, .sampler = 0 } },
                })
        Using 0 as a sampler uses the sokol default sampler with linear interpolation.
        The image should be created using sg_make_image from sokol_gfx.

    --- finally, on application shutdown, call

            sclay_shutdown()
 */
#if !defined(SOKOL_CLAY_NO_SOKOL_APP) && !defined(SOKOL_APP_INCLUDED)
#error "Please include sokol_app.h before sokol_clay.h (or define SOKOL_CLAY_NO_SOKOL_APP)"
#endif

typedef int sclay_font_t;

typedef struct sclay_image {
    sg_view view;
    sg_sampler sampler;
    struct {
        float u0, v0, u1, v1;
    } uv;
} sclay_image;


void sclay_setup();
void sclay_shutdown();

sclay_font_t sclay_add_font(const char *filename);
sclay_font_t sclay_add_font_mem(unsigned char *data, int dataLen);
Clay_Dimensions sclay_measure_text(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);

#ifndef SOKOL_CLAY_NO_SOKOL_APP
void sclay_new_frame();
void sclay_handle_event(const sapp_event *ev);
#endif  /* SOKOL_CLAY_NO_SOKOL_APP */

/* Use this if you don't call sclay_new_frame. `size` is the "virtual" size which
 * your layout is relative to (ie. the actual framebuffer size divided by dpi_scale.)
 * Set dpi_scale to 1 if you're not using high-dpi support. */
void sclay_set_layout_dimensions(Clay_Dimensions size, float dpi_scale);

void sclay_render(Clay_RenderCommandArray renderCommands, sclay_font_t *fonts);

#endif /* SOKOL_CLAY_INCLUDED */

#ifdef SOKOL_CLAY_IMPL
#define SOKOL_CLAY_IMPL_INCLUDED (1)
#ifndef SOKOL_GL_INCLUDED
#error "Please include sokol_gl.h before sokol_clay.h"
#endif
#ifndef SOKOL_FONTSTASH_INCLUDED
#error "Please include sokol_fontstash.h before sokol_clay.h"
#endif
#ifndef CLAY_HEADER
#error "Please include clay.h before sokol_clay.h"
#endif

typedef struct {
    sgl_pipeline pip;
#ifndef SOKOL_CLAY_NO_SOKOL_APP
    Clay_Vector2 mouse_pos, scroll;
    bool mouse_down;
#endif
    Clay_Dimensions size;
    float dpi_scale;
    FONScontext *fonts;
} _sclay_state_t;
static _sclay_state_t _sclay;

void sclay_setup() {
    _sclay.pip = sgl_make_pipeline(&(sg_pipeline_desc){
        .colors[0] = {
            .blend = {
                .enabled = true,
                .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
                .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
            },
        }
    });
#ifndef SOKOL_CLAY_NO_SOKOL_APP
    _sclay.mouse_pos = (Clay_Vector2){0, 0};
    _sclay.scroll = (Clay_Vector2){0, 0};
    _sclay.mouse_down = false;
#endif
    _sclay.size = (Clay_Dimensions){1, 1};
    _sclay.dpi_scale = 1;
    _sclay.fonts = sfons_create(&(sfons_desc_t){ 0 });
    //TODO clay error handler?
}

void sclay_shutdown() {
    sgl_destroy_pipeline(_sclay.pip);
    sfons_destroy(_sclay.fonts);
}

#ifndef SOKOL_CLAY_NO_SOKOL_APP
void sclay_handle_event(const sapp_event* ev) {
    switch(ev->type){
    case SAPP_EVENTTYPE_MOUSE_MOVE:
        _sclay.mouse_pos.x = ev->mouse_x / _sclay.dpi_scale;
        _sclay.mouse_pos.y = ev->mouse_y / _sclay.dpi_scale;
        break;
    case SAPP_EVENTTYPE_MOUSE_DOWN:
        _sclay.mouse_down = true;
        break;
    case SAPP_EVENTTYPE_MOUSE_UP:
        _sclay.mouse_down = false;
        break;
    case SAPP_EVENTTYPE_MOUSE_SCROLL:
        _sclay.scroll.x += ev->scroll_x;
        _sclay.scroll.y += ev->scroll_y;
        break;
    default: break;
    }
}

void sclay_new_frame() {
    sclay_set_layout_dimensions((Clay_Dimensions){ (float)sapp_width(), (float)sapp_height() },
                                sapp_dpi_scale());
    Clay_SetPointerState(_sclay.mouse_pos, _sclay.mouse_down);
    Clay_UpdateScrollContainers(true, _sclay.scroll, sapp_frame_duration());
    _sclay.scroll = (Clay_Vector2){0, 0};
}
#endif  /* SOKOL_CLAY_NO_SOKOL_APP */

void sclay_set_layout_dimensions(Clay_Dimensions size, float dpi_scale) {
    size.width /= dpi_scale;
    size.height /= dpi_scale;
    _sclay.size = size;
    if(_sclay.dpi_scale != dpi_scale){
        _sclay.dpi_scale = dpi_scale;
        Clay_ResetMeasureTextCache();
    }
    Clay_SetLayoutDimensions(size);
}

sclay_font_t sclay_add_font(const char *filename) {
    //TODO log something if we get FONS_INVALID
    return fonsAddFont(_sclay.fonts, "", filename);
}

sclay_font_t sclay_add_font_mem(unsigned char *data, int dataLen) {
    //TODO log something if we get FONS_INVALID
    return fonsAddFontMem(_sclay.fonts, "", data, dataLen, false);
}

Clay_Dimensions sclay_measure_text(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    sclay_font_t *fonts = (sclay_font_t *)userData;
    if(!fonts) return (Clay_Dimensions){ 0 };
    fonsSetFont(_sclay.fonts, fonts[config->fontId]);
    fonsSetSize(_sclay.fonts, config->fontSize * _sclay.dpi_scale);
    fonsSetSpacing(_sclay.fonts, config->letterSpacing * _sclay.dpi_scale);
    fonsSetAlign(_sclay.fonts, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
    float ascent, descent, lineh;
    fonsVertMetrics(_sclay.fonts, &ascent, &descent, &lineh);
    return (Clay_Dimensions) {
        .width = fonsTextBounds(_sclay.fonts, 0, 0, text.chars, text.chars + text.length, NULL) / _sclay.dpi_scale,
        .height = (ascent - descent) / _sclay.dpi_scale
    };
}

static void _draw_rect(float x, float y, float w, float h){
    sgl_v2f(x, y);
    sgl_v2f(x, y);
    sgl_v2f(x+w, y);
    sgl_v2f(x, y+h);
    sgl_v2f(x+w, y+h);
    sgl_v2f(x+w, y+h);
}

static void _draw_rect_textured(float x, float y, float w, float h, float u0, float v0, float u1, float v1){
    sgl_v2f_t2f(x, y, u0, v0);
    sgl_v2f_t2f(x, y, u0, v0);
    sgl_v2f_t2f(x+w, y, u1, v0);
    sgl_v2f_t2f(x, y+h, u0, v1);
    sgl_v2f_t2f(x+w, y+h, u1, v1);
    sgl_v2f_t2f(x+w, y+h, u1, v1);
}

static float _SIN[16] = {
    0.000000f, 0.104528f, 0.207912f, 0.309017f,
    0.406737f, 0.500000f, 0.587785f, 0.669131f,
    0.743145f, 0.809017f, 0.866025f, 0.913545f,
    0.951057f, 0.978148f, 0.994522f, 1.000000f,
};

/* rx,ry = radius */
static void _draw_corner(float x, float y, float rx, float ry){
    x -= rx;
    y -= ry;
    sgl_v2f(x, y);
    for(int i = 0; i < 16; ++i){
        sgl_v2f(x, y);
        sgl_v2f(x+(rx*_SIN[15-i]), y+(ry*_SIN[i]));
    }
    sgl_v2f(x+(rx*_SIN[0]), y+(ry*_SIN[15]));
}

static void _draw_corner_textured(float x, float y, float rx, float ry, float bx, float by, float bw, float bh, float u0, float v0, float u1, float v1) {
    x -= rx;
    y -= ry;
#define MAP_U(x) (u0+(((x)-bx)/bw)*(u1-u0))
#define MAP_V(y) (v0+(((y)-by)/bh)*(v1-v0))
    sgl_v2f_t2f(x, y, MAP_U(x), MAP_V(y));
    for(int i = 0; i < 16; ++i){
        sgl_v2f_t2f(x, y, MAP_U(x), MAP_V(y));
        float px = x+(rx*_SIN[15-i]);
        float py = y+(ry*_SIN[i]);
        sgl_v2f_t2f(px, py, MAP_U(px), MAP_V(py));
    }
    sgl_v2f_t2f(x+(rx*_SIN[0]), y+(ry*_SIN[15]), MAP_U(x+(rx*_SIN[0])), MAP_V(y+(ry*_SIN[15])));
#undef MAP_U
#undef MAP_V
}

/* rx,ry = radius   ix,iy = inner radius */
static void _draw_corner_border(float x, float y, float rx, float ry, float ix, float iy){
    x -= rx;
    y -= ry;
    sgl_v2f(x+(ix*_SIN[15]), y+(iy*_SIN[0]));
    for(int i = 0; i < 16; ++i){
        sgl_v2f(x+(ix*_SIN[15-i]), y+(iy*_SIN[i]));
        sgl_v2f(x+(rx*_SIN[15-i]), y+(ry*_SIN[i]));
    }
    sgl_v2f(x+(rx*_SIN[0]), y+(ry*_SIN[15]));
}

void sclay_render(Clay_RenderCommandArray renderCommands, sclay_font_t *fonts) {
    sgl_matrix_mode_modelview();
    sgl_translate(-1.0f, 1.0f, 0.0f);
    sgl_scale(2.0f/_sclay.size.width, -2.0f/_sclay.size.height, 1.0f);
    sgl_disable_texture();
    sgl_push_pipeline();
    sgl_load_pipeline(_sclay.pip);
    for (uint32_t i = 0; i < renderCommands.length; i++) {
        Clay_RenderCommand *renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
        Clay_BoundingBox bbox = renderCommand->boundingBox;
        switch (renderCommand->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleRenderData *config = &renderCommand->renderData.rectangle;
                sgl_c4f(config->backgroundColor.r / 255.0f,
                        config->backgroundColor.g / 255.0f,
                        config->backgroundColor.b / 255.0f,
                        config->backgroundColor.a / 255.0f);
                Clay_CornerRadius r = config->cornerRadius;
                sgl_begin_triangle_strip();
                if(r.topLeft > 0 || r.topRight > 0){
                    _draw_corner(bbox.x, bbox.y, -r.topLeft, -r.topLeft);
                    _draw_corner(bbox.x+bbox.width, bbox.y, r.topRight, -r.topRight);
                    _draw_rect(bbox.x+r.topLeft, bbox.y,
                               bbox.width-r.topLeft-r.topRight, CLAY__MAX(r.topLeft, r.topRight));
                }
                if(r.bottomLeft > 0 || r.bottomRight > 0){
                    _draw_corner(bbox.x, bbox.y+bbox.height, -r.bottomLeft, r.bottomLeft);
                    _draw_corner(bbox.x+bbox.width, bbox.y+bbox.height, r.bottomRight, r.bottomRight);
                    _draw_rect(bbox.x+r.bottomLeft,
                               bbox.y+bbox.height-CLAY__MAX(r.bottomLeft, r.bottomRight),
                               bbox.width-r.bottomLeft-r.bottomRight, CLAY__MAX(r.bottomLeft, r.bottomRight));
                }
                if(r.topLeft < r.bottomLeft){
                    if(r.topLeft < r.topRight){
                        _draw_rect(bbox.x, bbox.y+r.topLeft, r.topLeft, bbox.height-r.topLeft-r.bottomLeft);
                        _draw_rect(bbox.x+r.topLeft, bbox.y+r.topRight,
                                   r.bottomLeft-r.topLeft, bbox.height-r.topRight-r.bottomLeft);
                    } else {
                        _draw_rect(bbox.x, bbox.y+r.topLeft, r.bottomLeft, bbox.height-r.topLeft-r.bottomLeft);
                    }
                } else {
                    if(r.bottomLeft < r.bottomRight){
                        _draw_rect(bbox.x, bbox.y+r.topLeft, r.bottomLeft, bbox.height-r.topLeft-r.bottomLeft);
                        _draw_rect(bbox.x+r.bottomLeft, bbox.y+r.topLeft,
                                   r.topLeft-r.bottomLeft, bbox.height-r.topLeft-r.bottomRight);
                    } else {
                        _draw_rect(bbox.x, bbox.y+r.topLeft, r.topLeft, bbox.height-r.topLeft-r.bottomLeft);
                    }
                }
                if(r.topRight < r.bottomRight){
                    if(r.topRight < r.topLeft){
                        _draw_rect(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topLeft,
                                   r.bottomRight-r.topRight, bbox.height-r.topLeft-r.bottomRight);
                        _draw_rect(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight, bbox.height-r.topRight-r.bottomRight);
                    } else {
                        _draw_rect(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topRight,
                                   r.bottomRight, bbox.height-r.topRight-r.bottomRight);
                    }
                } else {
                    if(r.bottomRight < r.bottomLeft){
                        _draw_rect(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight-r.bottomRight, bbox.height-r.topRight-r.bottomLeft);
                        _draw_rect(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topRight,
                                   r.bottomRight, bbox.height-r.topRight-r.bottomRight);
                    } else {
                        _draw_rect(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight, bbox.height-r.topRight-r.bottomRight);
                    }
                }
                _draw_rect(bbox.x+CLAY__MAX(r.topLeft, r.bottomLeft),
                           bbox.y+CLAY__MAX(r.topLeft, r.topRight),
                           bbox.width-CLAY__MAX(r.topLeft, r.bottomLeft)-CLAY__MAX(r.topRight, r.bottomRight),
                           bbox.height-CLAY__MAX(r.topLeft, r.topRight)-CLAY__MAX(r.bottomLeft, r.bottomRight));
                sgl_end();
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                if(!fonts) break;
                Clay_TextRenderData *config = &renderCommand->renderData.text;
                Clay_StringSlice text = config->stringContents;
                fonsSetFont(_sclay.fonts, fonts[config->fontId]);
                uint32_t color = sfons_rgba(
                        config->textColor.r,
                        config->textColor.g,
                        config->textColor.b,
                        config->textColor.a);
                fonsSetColor(_sclay.fonts, color);
                fonsSetSpacing(_sclay.fonts, config->letterSpacing * _sclay.dpi_scale);
                fonsSetAlign(_sclay.fonts, FONS_ALIGN_LEFT | FONS_ALIGN_TOP);
                fonsSetSize(_sclay.fonts, config->fontSize * _sclay.dpi_scale);
                sgl_matrix_mode_modelview();
                sgl_push_matrix();
                sgl_scale(1.0f/_sclay.dpi_scale, 1.0f/_sclay.dpi_scale, 1.0f);
                fonsDrawText(_sclay.fonts, bbox.x*_sclay.dpi_scale, bbox.y*_sclay.dpi_scale,
                             text.chars, text.chars + text.length);
                sgl_pop_matrix();
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
                sgl_scissor_rectf(bbox.x*_sclay.dpi_scale, bbox.y*_sclay.dpi_scale,
                                  bbox.width*_sclay.dpi_scale, bbox.height*_sclay.dpi_scale,
                                  true);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
                sgl_scissor_rectf(0, 0,
                                  _sclay.size.width*_sclay.dpi_scale, _sclay.size.height*_sclay.dpi_scale,
                                  true);
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                Clay_ImageRenderData *config = &renderCommand->renderData.image;
                sclay_image* img = (sclay_image*)config->imageData;
                // by default, u1 and v1 are 1. if we pass 0.
                // note, we are modifying a copy !
                float u0 = img->uv.u0;
                float v0 = img->uv.v0;
                float u1 = img->uv.u1;
                float v1 = img->uv.v1;
                if (u1 == 0.f) {
                    u1 = 1.f;
                }
                if (v1 == 0.f) {
                    v1 = 1.f;
                }

                int untinted = config->backgroundColor.r == 0 && config->backgroundColor.g == 0 && config->backgroundColor.b == 0 && config->backgroundColor.a == 0;
                float cr = untinted ? 1.f : (config->backgroundColor.r / 255.0f);
                float gr = untinted ? 1.f : (config->backgroundColor.g / 255.0f);
                float br = untinted ? 1.f : (config->backgroundColor.b / 255.0f);
                float ar = untinted ? 1.f : (config->backgroundColor.a / 255.0f);

                sgl_c4f(cr, gr, br, ar);

                Clay_CornerRadius r = config->cornerRadius;

                sgl_enable_texture();
                sgl_texture(img->view, img->sampler);

                sgl_begin_triangle_strip();
                if(r.topLeft > 0 || r.topRight > 0){
                    _draw_corner_textured(bbox.x, bbox.y, -r.topLeft, -r.topLeft, bbox.x, bbox.y, bbox.width, bbox.height, u0, v0, u1, v1);
                    _draw_corner_textured(bbox.x+bbox.width, bbox.y, r.topRight, -r.topRight, bbox.x, bbox.y, bbox.width, bbox.height, u0, v0, u1, v1);
                    _draw_rect_textured(bbox.x+r.topLeft, bbox.y,
                               bbox.width-r.topLeft-r.topRight, CLAY__MAX(r.topLeft, r.topRight),
                               u0 + (r.topLeft/bbox.width)*(u1-u0), v0, u1 - (r.topRight/bbox.width)*(u1-u0), v0 + (CLAY__MAX(r.topLeft, r.topRight)/bbox.height)*(v1-v0));
                }
                if(r.bottomLeft > 0 || r.bottomRight > 0){
                    _draw_corner_textured(bbox.x, bbox.y+bbox.height, -r.bottomLeft, r.bottomLeft, bbox.x, bbox.y, bbox.width, bbox.height, u0, v0, u1, v1);
                    _draw_corner_textured(bbox.x+bbox.width, bbox.y+bbox.height, r.bottomRight, r.bottomRight, bbox.x, bbox.y, bbox.width, bbox.height, u0, v0, u1, v1);
                    _draw_rect_textured(bbox.x+r.bottomLeft,
                               bbox.y+bbox.height-CLAY__MAX(r.bottomLeft, r.bottomRight),
                               bbox.width-r.bottomLeft-r.bottomRight, CLAY__MAX(r.bottomLeft, r.bottomRight),
                               u0 + (r.bottomLeft/bbox.width)*(u1-u0), v1 - (CLAY__MAX(r.bottomLeft, r.bottomRight)/bbox.height)*(v1-v0), u1 - (r.bottomRight/bbox.width)*(u1-u0), v1);
                }
                if(r.topLeft < r.bottomLeft){
                    if(r.topLeft < r.topRight){
                        _draw_rect_textured(bbox.x, bbox.y+r.topLeft, r.topLeft, bbox.height-r.topLeft-r.bottomLeft,
                                u0, v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.topLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                        _draw_rect_textured(bbox.x+r.topLeft, bbox.y+r.topRight,
                               r.bottomLeft-r.topLeft, bbox.height-r.topRight-r.bottomLeft,
                               u0 + (r.topLeft/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u0 + (r.topLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                    } else {
                        _draw_rect_textured(bbox.x, bbox.y+r.topLeft, r.bottomLeft, bbox.height-r.topLeft-r.bottomLeft,
                                u0, v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.bottomLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                    }
                } else {
                    if(r.bottomLeft < r.bottomRight){
                        _draw_rect_textured(bbox.x, bbox.y+r.topLeft, r.bottomLeft, bbox.height-r.topLeft-r.bottomLeft,
                               u0, v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.bottomLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                        _draw_rect_textured(bbox.x+r.bottomLeft, bbox.y+r.topLeft,
                                   r.topLeft-r.bottomLeft, bbox.height-r.topLeft-r.bottomRight,
                                   u0 + (r.bottomLeft/bbox.width)*(u1-u0), v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.topLeft/bbox.width)*(u1-u0), v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    } else {
                        _draw_rect_textured(bbox.x, bbox.y+r.topLeft, r.topLeft, bbox.height-r.topLeft-r.bottomLeft,
                                u0, v0 + (r.topLeft/bbox.height)*(v1-v0), u0 + (r.topLeft/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                    }
                }
                if(r.topRight < r.bottomRight){
                    if(r.topRight < r.topLeft){
                        _draw_rect_textured(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topLeft,
                                   r.bottomRight-r.topRight, bbox.height-r.topLeft-r.bottomRight,
                                   u1 - (r.bottomRight/bbox.width)*(u1-u0), v0 + (r.topLeft/bbox.height)*(v1-v0), u1 - (r.topRight/bbox.width)*(u1-u0), v1 - (r.bottomRight/bbox.height)*(v1-v0));
                        _draw_rect_textured(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight, bbox.height-r.topRight-r.bottomRight,
                                   u1 - (r.topRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1, v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    } else {
                        _draw_rect_textured(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topRight,
                                   r.bottomRight, bbox.height-r.topRight-r.bottomRight,
                                   u1 - (r.bottomRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1, v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    }
                } else {
                    if(r.bottomRight < r.bottomLeft){
                        _draw_rect_textured(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight-r.bottomRight, bbox.height-r.topRight-r.bottomLeft,
                                   u1 - (r.topRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1 - (r.bottomRight/bbox.width)*(u1-u0), v1 - (r.bottomLeft/bbox.height)*(v1-v0));
                        _draw_rect_textured(bbox.x+bbox.width-r.bottomRight, bbox.y+r.topRight,
                                   r.bottomRight, bbox.height-r.topRight-r.bottomRight,
                                   u1 - (r.bottomRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1, v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    } else {
                        _draw_rect_textured(bbox.x+bbox.width-r.topRight, bbox.y+r.topRight,
                                   r.topRight, bbox.height-r.topRight-r.bottomRight,
                                   u1 - (r.topRight/bbox.width)*(u1-u0), v0 + (r.topRight/bbox.height)*(v1-v0), u1, v1 - (r.bottomRight/bbox.height)*(v1-v0));
                    }
                }
                _draw_rect_textured(bbox.x+CLAY__MAX(r.topLeft, r.bottomLeft),
                           bbox.y+CLAY__MAX(r.topLeft, r.topRight),
                           bbox.width-CLAY__MAX(r.topLeft, r.bottomLeft)-CLAY__MAX(r.topRight, r.bottomRight),
                           bbox.height-CLAY__MAX(r.topLeft, r.topRight)-CLAY__MAX(r.bottomLeft, r.bottomRight),
                           u0+CLAY__MAX(r.topLeft,r.bottomLeft)/bbox.width*(u1-u0), v0+CLAY__MAX(r.topLeft,r.topRight)/bbox.height*(v1-v0),
                           u1-CLAY__MAX(r.topRight,r.bottomRight)/bbox.width*(u1-u0), v1-CLAY__MAX(r.bottomLeft,r.bottomRight)/bbox.height*(v1-v0));
                sgl_end();
                sgl_disable_texture();
                break;
            }
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderRenderData *config = &renderCommand->renderData.border;
                sgl_c4f(config->color.r / 255.0f,
                        config->color.g / 255.0f,
                        config->color.b / 255.0f,
                        config->color.a / 255.0f);
                Clay_BorderWidth w = config->width;
                Clay_CornerRadius r = config->cornerRadius;
                sgl_begin_triangle_strip();
                if(w.left > 0){
                    _draw_rect(bbox.x, bbox.y + r.topLeft,
                               w.left, bbox.height - r.topLeft - r.bottomLeft);
                }
                if(w.right > 0){
                    _draw_rect(bbox.x + bbox.width - w.right, bbox.y + r.topRight,
                               w.right, bbox.height - r.topRight - r.bottomRight);
                }
                if(w.top > 0){
                    _draw_rect(bbox.x + r.topLeft, bbox.y,
                               bbox.width - r.topLeft - r.topRight, w.top);
                }
                if(w.bottom > 0){
                    _draw_rect(bbox.x + r.bottomLeft, bbox.y + bbox.height - w.bottom,
                               bbox.width - r.bottomLeft - r.bottomRight, w.bottom);
                }
                if(r.topLeft > 0 && (w.top > 0 || w.left > 0)){
                    _draw_corner_border(bbox.x, bbox.y,
                                        -r.topLeft, -r.topLeft,
                                        -r.topLeft+w.left, -r.topLeft+w.top);
                }
                if(r.topRight > 0 && (w.top > 0 || w.right > 0)){
                    _draw_corner_border(bbox.x+bbox.width, bbox.y,
                                        r.topRight, -r.topRight,
                                        r.topRight-w.right, -r.topRight+w.top);
                }
                if(r.bottomLeft > 0 && (w.bottom > 0 || w.left > 0)){
                    _draw_corner_border(bbox.x, bbox.y+bbox.height,
                                        -r.bottomLeft, r.bottomLeft,
                                        -r.bottomLeft+w.left, r.bottomLeft-w.bottom);
                }
                if(r.bottomRight > 0 && (w.bottom > 0 || w.right > 0)){
                    _draw_corner_border(bbox.x+bbox.width, bbox.y+bbox.height,
                                        r.bottomRight, r.bottomRight,
                                        r.bottomRight-w.right, r.bottomRight-w.bottom);
                }
                sgl_end();
                break;
            }
            default:
                break;
        }
    }
    sgl_pop_pipeline();
    sfons_flush(_sclay.fonts);
}
#endif /* SOKOL_CLAY_IMPL */

#ifndef CLAY_RENDERER_WEBGPU_H
#define CLAY_RENDERER_WEBGPU_H
/*
    clay_renderer_webgpu.h -- Clay UI renderer for WebGPU

    #define CLAY_RENDERER_WEBGPU_IMPL before including in *one* C file.

    Required headers (include before this file):
        clay.h
        stb_truetype.h
        webgpu/webgpu.h

    Thread safety: Thread-safe via explicit context. Each Clay_WebGPU_Context
    is independent. A single context must not be accessed concurrently.

    Usage:
        Clay_WebGPU_Context ctx = Clay_WebGPU_Setup(device, queue, WGPUTextureFormat_BGRA8Unorm);
        Clay_WebGPU_Font f = Clay_WebGPU_AddFont(ctx, "font.ttf");
        Clay_WebGPU_MeasureData md = { ctx, &f };
        Clay_SetMeasureTextFunction(Clay_WebGPU_MeasureText, &md);

        // Each frame:
        Clay_WebGPU_SetLayoutDimensions(ctx, (Clay_Dimensions){w,h}, dpi);
        Clay_SetPointerState(...);
        Clay_UpdateScrollContainers(...);
        Clay_RenderCommandArray cmds = Clay_EndLayout();

        Clay_WebGPU_Prepare(ctx, cmds, fonts);
        WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(enc, &rpd);
        Clay_WebGPU_Render(ctx, pass);
        wgpuRenderPassEncoderEnd(pass);
*/

#include <stdbool.h>

typedef int Clay_WebGPU_Font;

typedef struct _cwg_Ctx *Clay_WebGPU_Context;

typedef struct Clay_WebGPU_Image {
    WGPUTextureView view;
    WGPUSampler sampler;
    struct {
        float u0, v0, u1, v1;
    } uv;
} Clay_WebGPU_Image;

typedef struct {
    Clay_WebGPU_Context ctx;
    Clay_WebGPU_Font *fonts;
} Clay_WebGPU_MeasureData;

Clay_WebGPU_Context Clay_WebGPU_Setup(WGPUDevice device, WGPUQueue queue, WGPUTextureFormat target_format);
void Clay_WebGPU_Shutdown(Clay_WebGPU_Context ctx);

Clay_WebGPU_Font Clay_WebGPU_AddFont(Clay_WebGPU_Context ctx, const char *filename);
Clay_WebGPU_Font Clay_WebGPU_AddFontMem(Clay_WebGPU_Context ctx, unsigned char *data, int len, bool own);
Clay_Dimensions Clay_WebGPU_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData);

void Clay_WebGPU_SetLayoutDimensions(Clay_WebGPU_Context ctx, Clay_Dimensions size, float dpi_scale);
void Clay_WebGPU_Prepare(Clay_WebGPU_Context ctx, Clay_RenderCommandArray cmds, Clay_WebGPU_Font *fonts);
void Clay_WebGPU_Render(Clay_WebGPU_Context ctx, WGPURenderPassEncoder pass);
void Clay_WebGPU_FlushGlyphCache(Clay_WebGPU_Context ctx);

#ifndef CWG_SCROLL_SCALE
#define CWG_SCROLL_SCALE 1.0f
#endif

#endif /* CLAY_RENDERER_WEBGPU_H */

#ifdef CLAY_RENDERER_WEBGPU_IMPL
#ifndef CLAY_RENDERER_WEBGPU_IMPL_GUARD
#define CLAY_RENDERER_WEBGPU_IMPL_GUARD

#ifndef CLAY_HEADER
#error "Include clay.h before clay_renderer_webgpu.h"
#endif
#if !defined(STBTT_DEF) && !defined(STB_TRUETYPE_IMPLEMENTATION)
#error "Include stb_truetype.h before clay_renderer_webgpu.h"
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifndef CWG_NO_STDIO
#include <stdio.h>
#define _CWG_LOG(...) fprintf(stderr, "cwg: " __VA_ARGS__), fprintf(stderr, "\n")
#else
#define _CWG_LOG(msg, ...) ((void)0)
#endif

#ifndef CWG_MAX_FONTS
#define CWG_MAX_FONTS 16
#endif
#ifndef CWG_ATLAS_SIZE
#define CWG_ATLAS_SIZE 1024
#endif
#ifndef CWG_MAX_GLYPHS
#define CWG_MAX_GLYPHS 4096
#endif
#ifndef CWG_GLYPH_BUCKETS
#define CWG_GLYPH_BUCKETS 8192
#endif
#ifndef CWG_INIT_VERTS
#define CWG_INIT_VERTS (1 << 16)
#endif

/* Validate power-of-2 */
#if (CWG_GLYPH_BUCKETS & (CWG_GLYPH_BUCKETS - 1)) != 0
#error "CWG_GLYPH_BUCKETS must be a power of 2"
#endif
#if CWG_GLYPH_BUCKETS <= CWG_MAX_GLYPHS
#error "CWG_GLYPH_BUCKETS must be > CWG_MAX_GLYPHS"
#endif

#define _CWG_MAX(a, b) ((a) > (b) ? (a) : (b))
#ifdef __cplusplus
#define _CWG_WSTR(s) \
    WGPUStringView { (s), WGPU_STRLEN }
#else
#define _CWG_WSTR(s) \
    (WGPUStringView) { .data = (s), .length = WGPU_STRLEN }
#endif

static const char _cwg_wgsl_src[] =
    "struct Uniforms { screen_size: vec2f }\n"
    "@group(0) @binding(0) var<uniform> u: Uniforms;\n"
    "@group(1) @binding(0) var t: texture_2d<f32>;\n"
    "@group(1) @binding(1) var s: sampler;\n"
    "struct VI { @location(0) pos:vec2f, @location(1) uv:vec2f, @location(2) col:vec4f }\n"
    "struct VO { @builtin(position) pos:vec4f, @location(0) uv:vec2f, @location(1) col:vec4f }\n"
    "@vertex fn vs_main(i:VI)->VO {\n"
    "  var o:VO;\n"
    "  o.pos=vec4f(i.pos.x/u.screen_size.x*2.0-1.0,1.0-i.pos.y/u.screen_size.y*2.0,0,1);\n"
    "  o.uv=i.uv; o.col=i.col; return o;\n"
    "}\n"
    "@fragment fn fs_color(i:VO)->@location(0) vec4f {\n"
    "  return i.col*textureSample(t,s,i.uv);\n"
    "}\n"
    "@fragment fn fs_text(i:VO)->@location(0) vec4f {\n"
    "  let a=textureSample(t,s,i.uv).r;\n"
    "  return vec4f(i.col.rgb,i.col.a*a);\n"
    "}\n";

/* ---- UTF-8 decoder (rejects overlong + surrogates) ---- */
static int _cwg_utf8(const char *s, int len, int *out_bytes) {
    unsigned char c = (unsigned char)s[0];
    if (c < 0x80) {
        *out_bytes = 1;
        return c;
    }
    if (c < 0xC2) {
        *out_bytes = 1;
        return 0xFFFD;
    } /* reject C0/C1 overlong */
    int cp, need;
    if (c < 0xE0) {
        cp = c & 0x1F;
        need = 1;
    } else if (c < 0xF0) {
        cp = c & 0x0F;
        need = 2;
    } else if (c < 0xF5) {
        cp = c & 0x07;
        need = 3;
    } else {
        *out_bytes = 1;
        return 0xFFFD;
    }
    if (need > len - 1) {
        *out_bytes = 1;
        return 0xFFFD;
    }
    for (int i = 0; i < need; i++) {
        unsigned char b = (unsigned char)s[1 + i];
        if ((b & 0xC0) != 0x80) {
            *out_bytes = i + 1;
            return 0xFFFD;
        }
        cp = (cp << 6) | (b & 0x3F);
    }
    *out_bytes = need + 1;
    if (cp >= 0xD800 && cp <= 0xDFFF) return 0xFFFD; /* reject surrogates */
    if (need == 2 && cp < 0x800) return 0xFFFD;      /* reject overlong */
    if (need == 3 && cp < 0x10000) return 0xFFFD;
    return cp;
}

/* ---- Internal types ---- */
typedef struct {
    float x, y, u, v;
    uint8_t r, g, b, a;
} _cwg_Vert; /* 20 bytes */
typedef enum { _CWG_CMD_DRAW, _CWG_CMD_SCISSOR } _cwg_CmdType;
typedef struct {
    _cwg_CmdType type;
    union {
        struct {
            uint32_t idx_off, idx_count;
            WGPUBindGroup tex_bg;
            bool is_text;
        } draw;
        struct {
            uint32_t x, y, w, h;
        } scissor;
    } u;
} _cwg_Cmd;
typedef struct {
    int font_id, codepoint, size_q;
    uint16_t ax, ay, aw, ah;
    float xoff, yoff, xadvance;
    bool occupied;
} _cwg_Glyph;
typedef struct {
    stbtt_fontinfo info;
    unsigned char *data;
    bool owned;
} _cwg_FontInfo;

typedef struct _cwg_Ctx {
    WGPUDevice device;
    WGPUQueue queue;
    WGPUTextureFormat target_fmt;
    bool valid;
    WGPUShaderModule shader;
    WGPURenderPipeline pip_color, pip_text;
    WGPUBindGroupLayout bgl_uniform, bgl_texture;
    WGPUPipelineLayout pip_layout;
    WGPUSampler default_sampler;
    WGPUBuffer uniform_buf;
    WGPUBindGroup uniform_bg;
    WGPUTexture white_tex;
    WGPUTextureView white_view;
    WGPUBindGroup white_bg;
    WGPUTexture atlas_tex;
    WGPUTextureView atlas_view;
    WGPUBindGroup atlas_bg;
    unsigned char *atlas_pixels; /* heap-allocated CWG_ATLAS_SIZE^2 */
    int atlas_row_x, atlas_row_y, atlas_row_h;
    int atlas_dirty_ymin, atlas_dirty_ymax;
    bool atlas_needs_reset;
    float inv_atlas;
    _cwg_Glyph glyphs[CWG_GLYPH_BUCKETS];
    int glyph_count;
    _cwg_FontInfo fonts[CWG_MAX_FONTS];
    int font_count;
    _cwg_Vert *verts;
    uint32_t *indices;
    uint32_t vert_count, index_count, vert_cap, index_cap;
    _cwg_Cmd *cmds;
    uint32_t cmd_count, cmd_cap;
    WGPUBuffer vbuf, ibuf;
    uint64_t vbuf_size, ibuf_size;
    WGPUBindGroup *frame_bgs;
    int frame_bg_count, frame_bg_cap;
    Clay_Dimensions size;
    float dpi_scale;
    WGPUBindGroup cur_bg;
    bool cur_is_text;
    uint32_t batch_start;
} _cwg_Ctx;

static const float _cwg_sin[16] = {
    0.f, .104528f, .207912f, .309017f, .406737f, .5f, .587785f, .669131f, .743145f, .809017f, .866025f, .913545f, .951057f, .978148f, .994522f, 1.f,
};

/* ---- safe array growth ---- */
static bool _cwg_grow_v(_cwg_Ctx *ctx, uint32_t need) {
    while (ctx->vert_count + need > ctx->vert_cap) {
        ctx->vert_cap *= 2;
        _cwg_Vert *p = (_cwg_Vert *)realloc(ctx->verts, ctx->vert_cap * sizeof(_cwg_Vert));
        if (!p) {
            _CWG_LOG("vertex alloc failed");
            return false;
        }
        ctx->verts = p;
    }
    return true;
}

static bool _cwg_grow_i(_cwg_Ctx *ctx, uint32_t need) {
    while (ctx->index_count + need > ctx->index_cap) {
        ctx->index_cap *= 2;
        uint32_t *p = (uint32_t *)realloc(ctx->indices, ctx->index_cap * sizeof(uint32_t));
        if (!p) {
            _CWG_LOG("index alloc failed");
            return false;
        }
        ctx->indices = p;
    }
    return true;
}

static bool _cwg_grow_c(_cwg_Ctx *ctx) {
    if (ctx->cmd_count < ctx->cmd_cap) return true;
    ctx->cmd_cap *= 2;
    _cwg_Cmd *p = (_cwg_Cmd *)realloc(ctx->cmds, ctx->cmd_cap * sizeof(_cwg_Cmd));
    if (!p) {
        _CWG_LOG("cmd alloc failed");
        return false;
    }
    ctx->cmds = p;
    return true;
}

/* ---- vertex emission (checked + unchecked) ---- */
static inline uint8_t _cwg_f2u(float f) {
    int v = (int)(f * 255.f + .5f);
    return (uint8_t)(v < 0 ? 0 : v > 255 ? 255 : v);
}

static inline void _cwg_write_v(_cwg_Ctx *ctx, uint32_t idx, float x, float y, float u, float v, float cr, float cg, float cb, float ca) {
    ctx->verts[idx] = (_cwg_Vert){x, y, u, v, _cwg_f2u(cr), _cwg_f2u(cg), _cwg_f2u(cb), _cwg_f2u(ca)};
}

/* Checked: grows buffer, returns index */
static uint32_t _cwg_emit_v(_cwg_Ctx *ctx, float x, float y, float u, float v, float cr, float cg, float cb, float ca) {
    if (!_cwg_grow_v(ctx, 1)) return ctx->vert_count;
    uint32_t idx = ctx->vert_count++;
    _cwg_write_v(ctx, idx, x, y, u, v, cr, cg, cb, ca);
    return idx;
}

/* Unchecked: caller must ensure capacity */
static inline uint32_t _cwg_emit_vu(_cwg_Ctx *ctx, float x, float y, float u, float v, float cr, float cg, float cb, float ca) {
    uint32_t idx = ctx->vert_count++;
    _cwg_write_v(ctx, idx, x, y, u, v, cr, cg, cb, ca);
    return idx;
}

static void _cwg_emit_quad(_cwg_Ctx *ctx, float x, float y, float w, float h, float u0, float v0, float u1, float v1, float cr, float cg, float cb, float ca) {
    if (!_cwg_grow_v(ctx, 4) || !_cwg_grow_i(ctx, 6)) return;
    uint32_t b = ctx->vert_count;
    _cwg_emit_vu(ctx, x, y, u0, v0, cr, cg, cb, ca);
    _cwg_emit_vu(ctx, x + w, y, u1, v0, cr, cg, cb, ca);
    _cwg_emit_vu(ctx, x + w, y + h, u1, v1, cr, cg, cb, ca);
    _cwg_emit_vu(ctx, x, y + h, u0, v1, cr, cg, cb, ca);
    uint32_t *ix = &ctx->indices[ctx->index_count];
    ctx->index_count += 6;
    ix[0] = b;
    ix[1] = b + 1;
    ix[2] = b + 2;
    ix[3] = b;
    ix[4] = b + 2;
    ix[5] = b + 3;
}

static void _cwg_emit_corner(_cwg_Ctx *ctx, float cx, float cy, float rx, float ry, float cr, float cg, float cb, float ca) {
    if (!_cwg_grow_v(ctx, 17) || !_cwg_grow_i(ctx, 45)) return;
    float ox = cx - rx, oy = cy - ry;
    uint32_t c = _cwg_emit_vu(ctx, ox, oy, .5f, .5f, cr, cg, cb, ca);
    uint32_t f = ctx->vert_count;
    for (int i = 0; i < 16; i++) _cwg_emit_vu(ctx, ox + rx * _cwg_sin[15 - i], oy + ry * _cwg_sin[i], .5f, .5f, cr, cg, cb, ca);
    uint32_t *ix = &ctx->indices[ctx->index_count];
    ctx->index_count += 45;
    for (int i = 0; i < 15; i++) {
        ix[i * 3] = c;
        ix[i * 3 + 1] = f + i;
        ix[i * 3 + 2] = f + i + 1;
    }
}

static void _cwg_emit_corner_tex(_cwg_Ctx *ctx, float cx, float cy, float rx, float ry, float bx, float by, float bw, float bh, float u0, float v0, float u1, float v1, float cr, float cg, float cb, float ca) {
    if (!_cwg_grow_v(ctx, 17) || !_cwg_grow_i(ctx, 45)) return;
    float ox = cx - rx, oy = cy - ry;
#define _MU(px) (u0 + (((px) - bx) / bw) * (u1 - u0))
#define _MV(py) (v0 + (((py) - by) / bh) * (v1 - v0))
    uint32_t c = _cwg_emit_vu(ctx, ox, oy, _MU(ox), _MV(oy), cr, cg, cb, ca);
    uint32_t f = ctx->vert_count;
    for (int i = 0; i < 16; i++) {
        float px = ox + rx * _cwg_sin[15 - i], py = oy + ry * _cwg_sin[i];
        _cwg_emit_vu(ctx, px, py, _MU(px), _MV(py), cr, cg, cb, ca);
    }
#undef _MU
#undef _MV
    uint32_t *ix = &ctx->indices[ctx->index_count];
    ctx->index_count += 45;
    for (int i = 0; i < 15; i++) {
        ix[i * 3] = c;
        ix[i * 3 + 1] = f + i;
        ix[i * 3 + 2] = f + i + 1;
    }
}

static void _cwg_emit_corner_border(_cwg_Ctx *ctx, float cx, float cy, float rx, float ry, float ix2, float iy, float cr, float cg, float cb, float ca) {
    if (!_cwg_grow_v(ctx, 32) || !_cwg_grow_i(ctx, 90)) return;
    float ox = cx - rx, oy = cy - ry;
    uint32_t base = ctx->vert_count;
    for (int i = 0; i < 16; i++) {
        _cwg_emit_vu(ctx, ox + ix2 * _cwg_sin[15 - i], oy + iy * _cwg_sin[i], .5f, .5f, cr, cg, cb, ca);
        _cwg_emit_vu(ctx, ox + rx * _cwg_sin[15 - i], oy + ry * _cwg_sin[i], .5f, .5f, cr, cg, cb, ca);
    }
    uint32_t *ix = &ctx->indices[ctx->index_count];
    ctx->index_count += 90;
    for (int i = 0; i < 15; i++) {
        uint32_t i0 = base + i * 2, o0 = i0 + 1, i1 = i0 + 2, o1 = i0 + 3;
        ix[i * 6] = i0;
        ix[i * 6 + 1] = o0;
        ix[i * 6 + 2] = i1;
        ix[i * 6 + 3] = i1;
        ix[i * 6 + 4] = o0;
        ix[i * 6 + 5] = o1;
    }
}

/* ---- batch management ---- */
static void _cwg_flush(_cwg_Ctx *ctx) {
    uint32_t cnt = ctx->index_count - ctx->batch_start;
    if (!cnt) return;
    if (!_cwg_grow_c(ctx)) return;
    _cwg_Cmd *c = &ctx->cmds[ctx->cmd_count++];
    c->type = _CWG_CMD_DRAW;
    c->u.draw.idx_off = ctx->batch_start;
    c->u.draw.idx_count = cnt;
    c->u.draw.tex_bg = ctx->cur_bg;
    c->u.draw.is_text = ctx->cur_is_text;
    ctx->batch_start = ctx->index_count;
}

static WGPUBindGroup _cwg_make_tex_bg(_cwg_Ctx *ctx, WGPUTextureView view, WGPUSampler smp) {
    WGPUBindGroupEntry entries[2];
    memset(entries, 0, sizeof(entries));
    entries[0].binding = 0;
    entries[0].textureView = view;
    entries[1].binding = 1;
    entries[1].sampler = smp;
    WGPUBindGroupDescriptor desc;
    memset(&desc, 0, sizeof(desc));
    desc.layout = ctx->bgl_texture;
    desc.entryCount = 2;
    desc.entries = entries;
    return wgpuDeviceCreateBindGroup(ctx->device, &desc);
}

static void _cwg_store_frame_bg(_cwg_Ctx *ctx, WGPUBindGroup bg) {
    if (ctx->frame_bg_count >= ctx->frame_bg_cap) {
        int nc = ctx->frame_bg_cap ? ctx->frame_bg_cap * 2 : 64;
        WGPUBindGroup *p = (WGPUBindGroup *)realloc(ctx->frame_bgs, nc * sizeof(WGPUBindGroup));
        if (!p) {
            wgpuBindGroupRelease(bg);
            return;
        }
        ctx->frame_bgs = p;
        ctx->frame_bg_cap = nc;
    }
    ctx->frame_bgs[ctx->frame_bg_count++] = bg;
}

static void _cwg_set_tex(_cwg_Ctx *ctx, WGPUTextureView view, WGPUSampler smp, bool is_text) {
    if (!smp) smp = ctx->default_sampler;
    WGPUBindGroup bg;
    if (view == ctx->white_view && smp == ctx->default_sampler)
        bg = ctx->white_bg;
    else if (view == ctx->atlas_view && smp == ctx->default_sampler)
        bg = ctx->atlas_bg;
    else {
        bg = _cwg_make_tex_bg(ctx, view, smp);
        _cwg_store_frame_bg(ctx, bg);
    }
    if (bg != ctx->cur_bg || is_text != ctx->cur_is_text) {
        _cwg_flush(ctx);
        ctx->cur_bg = bg;
        ctx->cur_is_text = is_text;
    }
}

static void _cwg_push_scissor(_cwg_Ctx *ctx, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    _cwg_flush(ctx);
    if (!_cwg_grow_c(ctx)) return;
    _cwg_Cmd *c = &ctx->cmds[ctx->cmd_count++];
    c->type = _CWG_CMD_SCISSOR;
    c->u.scissor.x = x;
    c->u.scissor.y = y;
    c->u.scissor.w = w;
    c->u.scissor.h = h;
}

/* ---- glyph cache ---- */
static uint32_t _cwg_ghash(int fid, int cp, int sq) { return ((uint32_t)fid * 73856093u ^ (uint32_t)cp * 19349663u ^ (uint32_t)sq * 83492791u) & (CWG_GLYPH_BUCKETS - 1); }
static _cwg_Glyph *_cwg_find_glyph(_cwg_Ctx *ctx, int fid, int cp, int sq) {
    uint32_t idx = _cwg_ghash(fid, cp, sq);
    for (uint32_t i = 0; i < CWG_GLYPH_BUCKETS; i++) {
        uint32_t s = (idx + i) & (CWG_GLYPH_BUCKETS - 1);
        _cwg_Glyph *g = &ctx->glyphs[s];
        if (!g->occupied) return NULL;
        if (g->font_id == fid && g->codepoint == cp && g->size_q == sq) return g;
    }
    return NULL;
}

static void _cwg_mark_dirty(_cwg_Ctx *ctx, int y, int h) {
    if (h <= 0) return;
    int ymax = y + h;
    if (ctx->atlas_dirty_ymin < 0) {
        ctx->atlas_dirty_ymin = y;
        ctx->atlas_dirty_ymax = ymax;
    } else {
        if (y < ctx->atlas_dirty_ymin) ctx->atlas_dirty_ymin = y;
        if (ymax > ctx->atlas_dirty_ymax) ctx->atlas_dirty_ymax = ymax;
    }
}

static void _cwg_do_reset_atlas(_cwg_Ctx *ctx) {
    memset(ctx->atlas_pixels, 0, (size_t)CWG_ATLAS_SIZE * CWG_ATLAS_SIZE);
    memset(ctx->glyphs, 0, sizeof(ctx->glyphs));
    ctx->glyph_count = 0;
    ctx->atlas_row_x = ctx->atlas_row_y = ctx->atlas_row_h = 0;
    ctx->atlas_dirty_ymin = 0;
    ctx->atlas_dirty_ymax = CWG_ATLAS_SIZE;
    ctx->atlas_needs_reset = false;
}

void Clay_WebGPU_FlushGlyphCache(Clay_WebGPU_Context ctx) {
    ctx->atlas_needs_reset = true;
}

static _cwg_Glyph *_cwg_cache_glyph_inner(_cwg_Ctx *ctx, int fid, int cp, float fs, bool retried) {
    int sq = (int)(fs * 4.f + .5f);
    _cwg_Glyph *ex = _cwg_find_glyph(ctx, fid, cp, sq);
    if (ex) return ex;
    if (ctx->glyph_count >= CWG_MAX_GLYPHS) {
        if (retried) return NULL; /* glyph too large for atlas */
        _cwg_do_reset_atlas(ctx);
    }
    stbtt_fontinfo *fi = &ctx->fonts[fid].info;
    float scale = stbtt_ScaleForPixelHeight(fi, fs);
    int w = 0, h = 0, xoff = 0, yoff = 0;
    unsigned char *bmp = stbtt_GetCodepointBitmap(fi, scale, scale, cp, &w, &h, &xoff, &yoff);
    int adv, lsb;
    stbtt_GetCodepointHMetrics(fi, cp, &adv, &lsb);

    if (ctx->atlas_row_x + w + 1 > CWG_ATLAS_SIZE) {
        ctx->atlas_row_y += ctx->atlas_row_h + 1;
        ctx->atlas_row_x = 0;
        ctx->atlas_row_h = 0;
    }
    if (ctx->atlas_row_y + h + 1 > CWG_ATLAS_SIZE) {
        if (bmp) stbtt_FreeBitmap(bmp, NULL);
        if (retried) return NULL;
        _cwg_do_reset_atlas(ctx);
        return _cwg_cache_glyph_inner(ctx, fid, cp, fs, true);
    }
    int ax = ctx->atlas_row_x, ay = ctx->atlas_row_y;
    if (bmp) {
        for (int r = 0; r < h; r++) memcpy(&ctx->atlas_pixels[(ay + r) * CWG_ATLAS_SIZE + ax], &bmp[r * w], w);
        stbtt_FreeBitmap(bmp, NULL);
    }
    _cwg_mark_dirty(ctx, ay, h);
    ctx->atlas_row_x += w + 1;
    if (h + 1 > ctx->atlas_row_h) ctx->atlas_row_h = h + 1;

    uint32_t idx = _cwg_ghash(fid, cp, sq);
    for (uint32_t i = 0; i < CWG_GLYPH_BUCKETS; i++) {
        uint32_t s = (idx + i) & (CWG_GLYPH_BUCKETS - 1);
        if (!ctx->glyphs[s].occupied) {
            ctx->glyphs[s].font_id = fid;
            ctx->glyphs[s].codepoint = cp;
            ctx->glyphs[s].size_q = sq;
            ctx->glyphs[s].ax = (uint16_t)ax;
            ctx->glyphs[s].ay = (uint16_t)ay;
            ctx->glyphs[s].aw = (uint16_t)w;
            ctx->glyphs[s].ah = (uint16_t)h;
            ctx->glyphs[s].xoff = (float)xoff;
            ctx->glyphs[s].yoff = (float)yoff;
            ctx->glyphs[s].xadvance = (float)adv * scale;
            ctx->glyphs[s].occupied = true;
            ctx->glyph_count++;
            return &ctx->glyphs[s];
        }
    }
    return NULL;
}

static _cwg_Glyph *_cwg_cache_glyph(_cwg_Ctx *ctx, int fid, int cp, float fs) {
    return _cwg_cache_glyph_inner(ctx, fid, cp, fs, false);
}

/* ---- Setup ---- */
Clay_WebGPU_Context Clay_WebGPU_Setup(WGPUDevice device, WGPUQueue queue, WGPUTextureFormat fmt) {
    _cwg_Ctx *ctx = (_cwg_Ctx *)calloc(1, sizeof(_cwg_Ctx));
    if (!ctx) return NULL;
    ctx->device = device;
    ctx->queue = queue;
    ctx->target_fmt = fmt;
    ctx->dpi_scale = 1.f;
    ctx->size = (Clay_Dimensions){1, 1};
    ctx->atlas_dirty_ymin = -1;
    ctx->inv_atlas = 1.f / CWG_ATLAS_SIZE;
    ctx->atlas_pixels = (unsigned char *)calloc((size_t)CWG_ATLAS_SIZE * CWG_ATLAS_SIZE, 1);
    if (!ctx->atlas_pixels) {
        goto fail;
    }

    WGPUShaderSourceWGSL wgsl;
    memset(&wgsl, 0, sizeof(wgsl));
    wgsl.chain.sType = WGPUSType_ShaderSourceWGSL;
    wgsl.code = _CWG_WSTR(_cwg_wgsl_src);
    WGPUShaderModuleDescriptor smd;
    memset(&smd, 0, sizeof(smd));
    smd.nextInChain = &wgsl.chain;
    smd.label = _CWG_WSTR("cwg_shader");
    ctx->shader = wgpuDeviceCreateShaderModule(device, &smd);
    if (!ctx->shader) {
        _CWG_LOG("shader failed");
        goto fail;
    }

    {
        WGPUBindGroupLayoutEntry e;
        memset(&e, 0, sizeof(e));
        e.binding = 0;
        e.visibility = WGPUShaderStage_Vertex;
        e.buffer.type = WGPUBufferBindingType_Uniform;
        e.buffer.minBindingSize = 8;
        WGPUBindGroupLayoutDescriptor d;
        memset(&d, 0, sizeof(d));
        d.entryCount = 1;
        d.entries = &e;
        ctx->bgl_uniform = wgpuDeviceCreateBindGroupLayout(device, &d);
    }
    {
        WGPUBindGroupLayoutEntry e[2];
        memset(e, 0, sizeof(e));
        e[0].binding = 0;
        e[0].visibility = WGPUShaderStage_Fragment;
        e[0].texture.sampleType = WGPUTextureSampleType_Float;
        e[0].texture.viewDimension = WGPUTextureViewDimension_2D;
        e[1].binding = 1;
        e[1].visibility = WGPUShaderStage_Fragment;
        e[1].sampler.type = WGPUSamplerBindingType_Filtering;
        WGPUBindGroupLayoutDescriptor d;
        memset(&d, 0, sizeof(d));
        d.entryCount = 2;
        d.entries = e;
        ctx->bgl_texture = wgpuDeviceCreateBindGroupLayout(device, &d);
    }
    {
        WGPUBindGroupLayout bgls[2] = {ctx->bgl_uniform, ctx->bgl_texture};
        WGPUPipelineLayoutDescriptor d;
        memset(&d, 0, sizeof(d));
        d.bindGroupLayoutCount = 2;
        d.bindGroupLayouts = bgls;
        ctx->pip_layout = wgpuDeviceCreatePipelineLayout(device, &d);
    }
    {
        WGPUSamplerDescriptor d;
        memset(&d, 0, sizeof(d));
        d.magFilter = WGPUFilterMode_Linear;
        d.minFilter = WGPUFilterMode_Linear;
        d.addressModeU = d.addressModeV = d.addressModeW = WGPUAddressMode_ClampToEdge;
        d.mipmapFilter = WGPUMipmapFilterMode_Nearest;
        d.lodMaxClamp = 1.f;
        d.maxAnisotropy = 1;
        ctx->default_sampler = wgpuDeviceCreateSampler(device, &d);
    }

    WGPUVertexAttribute attrs[3];
    memset(attrs, 0, sizeof(attrs));
    attrs[0] = (WGPUVertexAttribute){WGPUVertexFormat_Float32x2, 0, 0};
    attrs[1] = (WGPUVertexAttribute){WGPUVertexFormat_Float32x2, 8, 1};
    attrs[2] = (WGPUVertexAttribute){WGPUVertexFormat_Unorm8x4, 16, 2};
    WGPUVertexBufferLayout vbl;
    memset(&vbl, 0, sizeof(vbl));
    vbl.arrayStride = 20;
    vbl.stepMode = WGPUVertexStepMode_Vertex;
    vbl.attributeCount = 3;
    vbl.attributes = attrs;
    WGPUBlendState blend;
    blend.color = (WGPUBlendComponent){WGPUBlendOperation_Add, WGPUBlendFactor_SrcAlpha, WGPUBlendFactor_OneMinusSrcAlpha};
    blend.alpha = (WGPUBlendComponent){WGPUBlendOperation_Add, WGPUBlendFactor_One, WGPUBlendFactor_OneMinusSrcAlpha};
    WGPUColorTargetState ct;
    memset(&ct, 0, sizeof(ct));
    ct.format = fmt;
    ct.blend = &blend;
    ct.writeMask = WGPUColorWriteMask_All;

    for (int pass = 0; pass < 2; pass++) {
        WGPUFragmentState fs;
        memset(&fs, 0, sizeof(fs));
        fs.module = ctx->shader;
        fs.entryPoint = pass == 0 ? _CWG_WSTR("fs_color") : _CWG_WSTR("fs_text");
        fs.targetCount = 1;
        fs.targets = &ct;
        WGPURenderPipelineDescriptor rpd;
        memset(&rpd, 0, sizeof(rpd));
        rpd.layout = ctx->pip_layout;
        rpd.vertex.module = ctx->shader;
        rpd.vertex.entryPoint = _CWG_WSTR("vs_main");
        rpd.vertex.bufferCount = 1;
        rpd.vertex.buffers = &vbl;
        rpd.primitive.topology = WGPUPrimitiveTopology_TriangleList;
        rpd.primitive.frontFace = WGPUFrontFace_CCW;
        rpd.primitive.cullMode = WGPUCullMode_None;
        rpd.multisample.count = 1;
        rpd.multisample.mask = ~0u;
        rpd.fragment = &fs;
        WGPURenderPipeline p = wgpuDeviceCreateRenderPipeline(device, &rpd);
        if (!p) {
            _CWG_LOG("pipeline %d failed", pass);
            goto fail;
        }
        if (pass == 0)
            ctx->pip_color = p;
        else
            ctx->pip_text = p;
    }
    {
        WGPUBufferDescriptor d;
        memset(&d, 0, sizeof(d));
        d.usage = WGPUBufferUsage_Uniform | WGPUBufferUsage_CopyDst;
        d.size = 8;
        ctx->uniform_buf = wgpuDeviceCreateBuffer(device, &d);
        WGPUBindGroupEntry e;
        memset(&e, 0, sizeof(e));
        e.binding = 0;
        e.buffer = ctx->uniform_buf;
        e.size = 8;
        WGPUBindGroupDescriptor bgd;
        memset(&bgd, 0, sizeof(bgd));
        bgd.layout = ctx->bgl_uniform;
        bgd.entryCount = 1;
        bgd.entries = &e;
        ctx->uniform_bg = wgpuDeviceCreateBindGroup(device, &bgd);
    }
    {
        WGPUTextureDescriptor td;
        memset(&td, 0, sizeof(td));
        td.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
        td.dimension = WGPUTextureDimension_2D;
        td.size = (WGPUExtent3D){1, 1, 1};
        td.format = WGPUTextureFormat_RGBA8Unorm;
        td.mipLevelCount = 1;
        td.sampleCount = 1;
        ctx->white_tex = wgpuDeviceCreateTexture(device, &td);
        ctx->white_view = wgpuTextureCreateView(ctx->white_tex, NULL);
        uint8_t px[4] = {255, 255, 255, 255};
        WGPUTexelCopyTextureInfo dst;
        memset(&dst, 0, sizeof(dst));
        dst.texture = ctx->white_tex;
        dst.aspect = WGPUTextureAspect_All;
        WGPUTexelCopyBufferLayout lay;
        memset(&lay, 0, sizeof(lay));
        lay.bytesPerRow = 4;
        lay.rowsPerImage = 1;
        WGPUExtent3D sz = {1, 1, 1};
        wgpuQueueWriteTexture(queue, &dst, px, 4, &lay, &sz);
        ctx->white_bg = _cwg_make_tex_bg(ctx, ctx->white_view, ctx->default_sampler);
    }
    {
        WGPUTextureDescriptor td;
        memset(&td, 0, sizeof(td));
        td.usage = WGPUTextureUsage_TextureBinding | WGPUTextureUsage_CopyDst;
        td.dimension = WGPUTextureDimension_2D;
        td.size = (WGPUExtent3D){CWG_ATLAS_SIZE, CWG_ATLAS_SIZE, 1};
        td.format = WGPUTextureFormat_R8Unorm;
        td.mipLevelCount = 1;
        td.sampleCount = 1;
        ctx->atlas_tex = wgpuDeviceCreateTexture(device, &td);
        ctx->atlas_view = wgpuTextureCreateView(ctx->atlas_tex, NULL);
        ctx->atlas_bg = _cwg_make_tex_bg(ctx, ctx->atlas_view, ctx->default_sampler);
    }

    ctx->vert_cap = CWG_INIT_VERTS;
    ctx->index_cap = CWG_INIT_VERTS * 3;
    ctx->cmd_cap = 256;
    ctx->verts = (_cwg_Vert *)calloc(ctx->vert_cap, sizeof(_cwg_Vert));
    ctx->indices = (uint32_t *)calloc(ctx->index_cap, sizeof(uint32_t));
    ctx->cmds = (_cwg_Cmd *)calloc(ctx->cmd_cap, sizeof(_cwg_Cmd));
    if (!ctx->verts || !ctx->indices || !ctx->cmds) {
        goto fail;
    }
    ctx->valid = true;
    return ctx;

fail:
    Clay_WebGPU_Shutdown(ctx);
    return NULL;
}

void Clay_WebGPU_Shutdown(Clay_WebGPU_Context ctx) {
    if (!ctx) return;
    for (int i = 0; i < ctx->frame_bg_count; i++) wgpuBindGroupRelease(ctx->frame_bgs[i]);
    free(ctx->frame_bgs);
    if (ctx->pip_color) wgpuRenderPipelineRelease(ctx->pip_color);
    if (ctx->pip_text) wgpuRenderPipelineRelease(ctx->pip_text);
    if (ctx->pip_layout) wgpuPipelineLayoutRelease(ctx->pip_layout);
    if (ctx->bgl_uniform) wgpuBindGroupLayoutRelease(ctx->bgl_uniform);
    if (ctx->bgl_texture) wgpuBindGroupLayoutRelease(ctx->bgl_texture);
    if (ctx->shader) wgpuShaderModuleRelease(ctx->shader);
    if (ctx->default_sampler) wgpuSamplerRelease(ctx->default_sampler);
    if (ctx->uniform_bg) wgpuBindGroupRelease(ctx->uniform_bg);
    if (ctx->uniform_buf) wgpuBufferRelease(ctx->uniform_buf);
    if (ctx->white_bg) wgpuBindGroupRelease(ctx->white_bg);
    if (ctx->white_view) wgpuTextureViewRelease(ctx->white_view);
    if (ctx->white_tex) wgpuTextureRelease(ctx->white_tex);
    if (ctx->atlas_bg) wgpuBindGroupRelease(ctx->atlas_bg);
    if (ctx->atlas_view) wgpuTextureViewRelease(ctx->atlas_view);
    if (ctx->atlas_tex) wgpuTextureRelease(ctx->atlas_tex);
    if (ctx->vbuf) wgpuBufferRelease(ctx->vbuf);
    if (ctx->ibuf) wgpuBufferRelease(ctx->ibuf);
    for (int i = 0; i < ctx->font_count; i++)
        if (ctx->fonts[i].owned) free(ctx->fonts[i].data);
    free(ctx->atlas_pixels);
    free(ctx->verts);
    free(ctx->indices);
    free(ctx->cmds);
    free(ctx);
}

/* ---- Fonts ---- */
#ifndef CWG_NO_STDIO
Clay_WebGPU_Font Clay_WebGPU_AddFont(Clay_WebGPU_Context ctx, const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        _CWG_LOG("can't open %s", filename);
        return -1;
    }
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (sz <= 0) {
        fclose(f);
        return -1;
    }
    unsigned char *data = (unsigned char *)malloc((size_t)sz);
    if (!data || fread(data, 1, (size_t)sz, f) != (size_t)sz) {
        free(data);
        fclose(f);
        return -1;
    }
    fclose(f);
    return Clay_WebGPU_AddFontMem(ctx, data, (int)sz, true);
}
#endif

Clay_WebGPU_Font Clay_WebGPU_AddFontMem(Clay_WebGPU_Context ctx, unsigned char *data, int len, bool own) {
    if (ctx->font_count>=CWG_MAX_FONTS) return -1;
    int id=ctx->font_count;
    ctx->fonts[id].data=data; ctx->fonts[id].owned=own;
    if (!stbtt_InitFont(&ctx->fonts[id].info,data,stbtt_GetFontOffsetForIndex(data,0))) {
        if (own) free(data);
        ctx->fonts[id].data=NULL; ctx->fonts[id].owned=false;
        return -1;
    }
    ctx->font_count++; return id;
}

Clay_Dimensions Clay_WebGPU_MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, void *userData) {
    const Clay_WebGPU_MeasureData *md = (const Clay_WebGPU_MeasureData *)userData;
    if (!md || !md->ctx || !md->fonts) return (Clay_Dimensions){0, 0};
    _cwg_Ctx *ctx = md->ctx;
    int fid = md->fonts[config->fontId];
    if (fid < 0 || fid >= ctx->font_count) return (Clay_Dimensions){0, 0};
    float fs = config->fontSize * ctx->dpi_scale;
    stbtt_fontinfo *fi = &ctx->fonts[fid].info;
    float scale = stbtt_ScaleForPixelHeight(fi, fs);
    int asc, desc, lg;
    stbtt_GetFontVMetrics(fi, &asc, &desc, &lg);
    float width = 0;
    int pos = 0, prev = 0;
    while (pos < (int)text.length) {
        int bytes, cp = _cwg_utf8(text.chars + pos, (int)text.length - pos, &bytes);
        int adv, lsb;
        stbtt_GetCodepointHMetrics(fi, cp, &adv, &lsb);
        if (pos > 0) {
            width += stbtt_GetCodepointKernAdvance(fi, prev, cp) * scale;
            width += config->letterSpacing * ctx->dpi_scale;
        }
        width += adv * scale;
        prev = cp;
        pos += bytes;
    }
    return (Clay_Dimensions){width / ctx->dpi_scale, (float)(asc - desc) * scale / ctx->dpi_scale};
}

void Clay_WebGPU_SetLayoutDimensions(Clay_WebGPU_Context ctx, Clay_Dimensions size, float dpi) {
    size.width /= dpi;
    size.height /= dpi;
    ctx->size = size;
    if (ctx->dpi_scale != dpi) {
        ctx->dpi_scale = dpi;
        ctx->atlas_needs_reset = true;
        Clay_ResetMeasureTextCache();
    }
    Clay_SetLayoutDimensions(size);
}

/* ---- Rounded rect ---- */
static void _cwg_rrect(_cwg_Ctx *ctx, Clay_BoundingBox b, Clay_CornerRadius r, float cr, float cg, float cb, float ca) {
    float ml = _CWG_MAX(r.topLeft, r.bottomLeft), mr = _CWG_MAX(r.topRight, r.bottomRight);
    float mt = _CWG_MAX(r.topLeft, r.topRight), mb = _CWG_MAX(r.bottomLeft, r.bottomRight);
    if (r.topLeft > 0 || r.topRight > 0) {
        _cwg_emit_corner(ctx, b.x, b.y, -r.topLeft, -r.topLeft, cr, cg, cb, ca);
        _cwg_emit_corner(ctx, b.x + b.width, b.y, r.topRight, -r.topRight, cr, cg, cb, ca);
        _cwg_emit_quad(ctx, b.x + r.topLeft, b.y, b.width - r.topLeft - r.topRight, mt, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
    }
    if (r.bottomLeft > 0 || r.bottomRight > 0) {
        _cwg_emit_corner(ctx, b.x, b.y + b.height, -r.bottomLeft, r.bottomLeft, cr, cg, cb, ca);
        _cwg_emit_corner(ctx, b.x + b.width, b.y + b.height, r.bottomRight, r.bottomRight, cr, cg, cb, ca);
        _cwg_emit_quad(ctx, b.x + r.bottomLeft, b.y + b.height - mb, b.width - r.bottomLeft - r.bottomRight, mb, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
    }
    _cwg_emit_quad(ctx, b.x, b.y + mt, ml, b.height - mt - mb, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
    _cwg_emit_quad(ctx, b.x + b.width - mr, b.y + mt, mr, b.height - mt - mb, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
    _cwg_emit_quad(ctx, b.x + ml, b.y + mt, b.width - ml - mr, b.height - mt - mb, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
}

static void _cwg_rimg(_cwg_Ctx *ctx, Clay_BoundingBox b, Clay_CornerRadius r, float u0, float v0, float u1, float v1, float cr, float cg, float cb, float ca) {
#define _UL(px) (u0 + (((px) - b.x) / b.width) * (u1 - u0))
#define _VL(py) (v0 + (((py) - b.y) / b.height) * (v1 - v0))
    float mt = _CWG_MAX(r.topLeft, r.topRight), mb = _CWG_MAX(r.bottomLeft, r.bottomRight);
    float ml = _CWG_MAX(r.topLeft, r.bottomLeft), mr = _CWG_MAX(r.topRight, r.bottomRight);
    if (r.topLeft > 0 || r.topRight > 0) {
        _cwg_emit_corner_tex(ctx, b.x, b.y, -r.topLeft, -r.topLeft, b.x, b.y, b.width, b.height, u0, v0, u1, v1, cr, cg, cb, ca);
        _cwg_emit_corner_tex(ctx, b.x + b.width, b.y, r.topRight, -r.topRight, b.x, b.y, b.width, b.height, u0, v0, u1, v1, cr, cg, cb, ca);
        _cwg_emit_quad(ctx, b.x + r.topLeft, b.y, b.width - r.topLeft - r.topRight, mt, _UL(b.x + r.topLeft), v0, _UL(b.x + b.width - r.topRight), _VL(b.y + mt), cr, cg, cb, ca);
    }
    if (r.bottomLeft > 0 || r.bottomRight > 0) {
        _cwg_emit_corner_tex(ctx, b.x, b.y + b.height, -r.bottomLeft, r.bottomLeft, b.x, b.y, b.width, b.height, u0, v0, u1, v1, cr, cg, cb, ca);
        _cwg_emit_corner_tex(ctx, b.x + b.width, b.y + b.height, r.bottomRight, r.bottomRight, b.x, b.y, b.width, b.height, u0, v0, u1, v1, cr, cg, cb, ca);
        _cwg_emit_quad(ctx, b.x + r.bottomLeft, b.y + b.height - mb, b.width - r.bottomLeft - r.bottomRight, mb, _UL(b.x + r.bottomLeft), _VL(b.y + b.height - mb), _UL(b.x + b.width - r.bottomRight), v1, cr, cg, cb, ca);
    }
    float ly = b.y + mt, lh = b.height - mt - mb;
    if (ml > 0 && lh > 0) _cwg_emit_quad(ctx, b.x, ly, ml, lh, _UL(b.x), _VL(ly), _UL(b.x + ml), _VL(ly + lh), cr, cg, cb, ca);
    if (mr > 0 && lh > 0) {
        float rx = b.x + b.width - mr;
        _cwg_emit_quad(ctx, rx, ly, mr, lh, _UL(rx), _VL(ly), _UL(rx + mr), _VL(ly + lh), cr, cg, cb, ca);
    }
    float cx = b.x + ml, cw = b.width - ml - mr;
    _cwg_emit_quad(ctx, cx, ly, cw, lh, _UL(cx), _VL(ly), _UL(cx + cw), _VL(ly + lh), cr, cg, cb, ca);
#undef _UL
#undef _VL
}

/* ---- Prepare ---- */
void Clay_WebGPU_Prepare(Clay_WebGPU_Context ctx, Clay_RenderCommandArray cmds, Clay_WebGPU_Font *fonts) {
    if (!ctx->valid) return;
    if (ctx->atlas_needs_reset) _cwg_do_reset_atlas(ctx);
    for (int i = 0; i < ctx->frame_bg_count; i++) wgpuBindGroupRelease(ctx->frame_bgs[i]);
    ctx->frame_bg_count = 0;
    ctx->vert_count = ctx->index_count = ctx->cmd_count = 0;
    ctx->batch_start = 0;
    ctx->cur_bg = ctx->white_bg;
    ctx->cur_is_text = false;
    float ds = ctx->dpi_scale, inv = ctx->inv_atlas;
    _cwg_push_scissor(ctx, 0, 0, (uint32_t)(ctx->size.width * ds), (uint32_t)(ctx->size.height * ds));

    for (uint32_t i = 0; i < cmds.length; i++) {
        Clay_RenderCommand *rc = Clay_RenderCommandArray_Get(&cmds, i);
        Clay_BoundingBox bb = rc->boundingBox;
        switch (rc->commandType) {
            case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                Clay_RectangleRenderData *d = &rc->renderData.rectangle;
                _cwg_set_tex(ctx, ctx->white_view, ctx->default_sampler, false);
                _cwg_rrect(ctx, bb, d->cornerRadius, d->backgroundColor.r / 255.f, d->backgroundColor.g / 255.f, d->backgroundColor.b / 255.f, d->backgroundColor.a / 255.f);
            } break;
            case CLAY_RENDER_COMMAND_TYPE_TEXT: {
                if (!fonts) break;
                Clay_TextRenderData *d = &rc->renderData.text;
                if (d->fontId < 0) break;
                int fid = fonts[d->fontId];
                if (fid < 0 || fid >= ctx->font_count) break;
                float fs = d->fontSize * ds;
                float cr = d->textColor.r / 255.f, cg = d->textColor.g / 255.f, cb = d->textColor.b / 255.f, ca = d->textColor.a / 255.f;
                _cwg_set_tex(ctx, ctx->atlas_view, ctx->default_sampler, true);
                stbtt_fontinfo *fi = &ctx->fonts[fid].info;
                float scale = stbtt_ScaleForPixelHeight(fi, fs);
                int asc, dsc, lg;
                stbtt_GetFontVMetrics(fi, &asc, &dsc, &lg);
                float ascent = (float)asc * scale, pen_x = bb.x;
                int pos = 0, prev_cp = 0;
                while (pos < (int)d->stringContents.length) {
                    int bytes, cp = _cwg_utf8(d->stringContents.chars + pos, (int)d->stringContents.length - pos, &bytes);
                    if (pos > 0) {
                        pen_x += stbtt_GetCodepointKernAdvance(fi, prev_cp, cp) * scale / ds;
                        pen_x += d->letterSpacing;
                    }
                    _cwg_Glyph *g = _cwg_cache_glyph(ctx, fid, cp, fs);
                    if (g && g->aw > 0 && g->ah > 0) {
                        float gx = pen_x + g->xoff / ds, gy = bb.y + (ascent + g->yoff) / ds;
                        _cwg_emit_quad(ctx, gx, gy, (float)g->aw / ds, (float)g->ah / ds, g->ax * inv, g->ay * inv, (g->ax + g->aw) * inv, (g->ay + g->ah) * inv, cr, cg, cb, ca);
                    }
                    if (g) pen_x += g->xadvance / ds;
                    prev_cp = cp;
                    pos += bytes;
                }
            } break;
            case CLAY_RENDER_COMMAND_TYPE_IMAGE: {
                Clay_ImageRenderData *d = &rc->renderData.image;
                Clay_WebGPU_Image *img = (Clay_WebGPU_Image *)d->imageData;
                if (!img || !img->view) break;
                float u0 = img->uv.u0, v0 = img->uv.v0, u1 = img->uv.u1 ? img->uv.u1 : 1.f, v1 = img->uv.v1 ? img->uv.v1 : 1.f;
                int ut = d->backgroundColor.r == 0 && d->backgroundColor.g == 0 && d->backgroundColor.b == 0 && d->backgroundColor.a == 0;
                float cr = ut ? 1.f : d->backgroundColor.r / 255.f, cg = ut ? 1.f : d->backgroundColor.g / 255.f;
                float cb = ut ? 1.f : d->backgroundColor.b / 255.f, ca = ut ? 1.f : d->backgroundColor.a / 255.f;
                _cwg_set_tex(ctx, img->view, img->sampler, false);
                bool hr = d->cornerRadius.topLeft > 0 || d->cornerRadius.topRight > 0 || d->cornerRadius.bottomLeft > 0 || d->cornerRadius.bottomRight > 0;
                if (hr)
                    _cwg_rimg(ctx, bb, d->cornerRadius, u0, v0, u1, v1, cr, cg, cb, ca);
                else
                    _cwg_emit_quad(ctx, bb.x, bb.y, bb.width, bb.height, u0, v0, u1, v1, cr, cg, cb, ca);
            } break;
            case CLAY_RENDER_COMMAND_TYPE_BORDER: {
                Clay_BorderRenderData *d = &rc->renderData.border;
                _cwg_set_tex(ctx, ctx->white_view, ctx->default_sampler, false);
                float cr = d->color.r / 255.f, cg = d->color.g / 255.f, cb = d->color.b / 255.f, ca = d->color.a / 255.f;
                Clay_BorderWidth w = d->width;
                Clay_CornerRadius r = d->cornerRadius;
                if (w.left > 0) _cwg_emit_quad(ctx, bb.x, bb.y + r.topLeft, w.left, bb.height - r.topLeft - r.bottomLeft, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
                if (w.right > 0) _cwg_emit_quad(ctx, bb.x + bb.width - w.right, bb.y + r.topRight, w.right, bb.height - r.topRight - r.bottomRight, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
                if (w.top > 0) _cwg_emit_quad(ctx, bb.x + r.topLeft, bb.y, bb.width - r.topLeft - r.topRight, w.top, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
                if (w.bottom > 0) _cwg_emit_quad(ctx, bb.x + r.bottomLeft, bb.y + bb.height - w.bottom, bb.width - r.bottomLeft - r.bottomRight, w.bottom, .5f, .5f, .5f, .5f, cr, cg, cb, ca);
                if (r.topLeft > 0 && (w.top > 0 || w.left > 0)) _cwg_emit_corner_border(ctx, bb.x, bb.y, -r.topLeft, -r.topLeft, -r.topLeft + w.left, -r.topLeft + w.top, cr, cg, cb, ca);
                if (r.topRight > 0 && (w.top > 0 || w.right > 0)) _cwg_emit_corner_border(ctx, bb.x + bb.width, bb.y, r.topRight, -r.topRight, r.topRight - w.right, -r.topRight + w.top, cr, cg, cb, ca);
                if (r.bottomLeft > 0 && (w.bottom > 0 || w.left > 0)) _cwg_emit_corner_border(ctx, bb.x, bb.y + bb.height, -r.bottomLeft, r.bottomLeft, -r.bottomLeft + w.left, r.bottomLeft - w.bottom, cr, cg, cb, ca);
                if (r.bottomRight > 0 && (w.bottom > 0 || w.right > 0)) _cwg_emit_corner_border(ctx, bb.x + bb.width, bb.y + bb.height, r.bottomRight, r.bottomRight, r.bottomRight - w.right, r.bottomRight - w.bottom, cr, cg, cb, ca);
            } break;
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START:
                _cwg_push_scissor(ctx, (uint32_t)(bb.x * ds), (uint32_t)(bb.y * ds), (uint32_t)(bb.width * ds), (uint32_t)(bb.height * ds));
                break;
            case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END:
                _cwg_push_scissor(ctx, 0, 0, (uint32_t)(ctx->size.width * ds), (uint32_t)(ctx->size.height * ds));
                break;
            default:
                break;
        }
    }
    _cwg_flush(ctx);
    if (ctx->vert_count == 0) return;
    uint64_t vsize = ctx->vert_count * sizeof(_cwg_Vert), isize = ctx->index_count * sizeof(uint32_t);
    if (!ctx->vbuf || ctx->vbuf_size < vsize) {
        if (ctx->vbuf) wgpuBufferRelease(ctx->vbuf);
        ctx->vbuf_size = vsize * 2;
        WGPUBufferDescriptor bd;
        memset(&bd, 0, sizeof(bd));
        bd.usage = WGPUBufferUsage_Vertex | WGPUBufferUsage_CopyDst;
        bd.size = ctx->vbuf_size;
        ctx->vbuf = wgpuDeviceCreateBuffer(ctx->device, &bd);
    }
    if (!ctx->ibuf || ctx->ibuf_size < isize) {
        if (ctx->ibuf) wgpuBufferRelease(ctx->ibuf);
        ctx->ibuf_size = isize * 2;
        WGPUBufferDescriptor bd;
        memset(&bd, 0, sizeof(bd));
        bd.usage = WGPUBufferUsage_Index | WGPUBufferUsage_CopyDst;
        bd.size = ctx->ibuf_size;
        ctx->ibuf = wgpuDeviceCreateBuffer(ctx->device, &bd);
    }
    wgpuQueueWriteBuffer(ctx->queue, ctx->vbuf, 0, ctx->verts, (size_t)vsize);
    wgpuQueueWriteBuffer(ctx->queue, ctx->ibuf, 0, ctx->indices, (size_t)isize);
    float screen[2] = {ctx->size.width, ctx->size.height};
    wgpuQueueWriteBuffer(ctx->queue, ctx->uniform_buf, 0, screen, 8);
    if (ctx->atlas_dirty_ymin >= 0) {
        int dy = ctx->atlas_dirty_ymin, dh = ctx->atlas_dirty_ymax - dy;
        if (dh < 1) dh = 1;
        WGPUTexelCopyTextureInfo dst;
        memset(&dst, 0, sizeof(dst));
        dst.texture = ctx->atlas_tex;
        dst.origin = (WGPUOrigin3D){0, (uint32_t)dy, 0};
        dst.aspect = WGPUTextureAspect_All;
        WGPUTexelCopyBufferLayout lay;
        memset(&lay, 0, sizeof(lay));
        lay.bytesPerRow = CWG_ATLAS_SIZE;
        lay.rowsPerImage = (uint32_t)dh;
        WGPUExtent3D sz = {CWG_ATLAS_SIZE, (uint32_t)dh, 1};
        wgpuQueueWriteTexture(ctx->queue, &dst, &ctx->atlas_pixels[dy * CWG_ATLAS_SIZE], (size_t)(CWG_ATLAS_SIZE * dh), &lay, &sz);
        ctx->atlas_dirty_ymin = -1;
    }
}

void Clay_WebGPU_Render(Clay_WebGPU_Context ctx, WGPURenderPassEncoder pass) {
    if (!ctx->valid || ctx->cmd_count == 0 || !ctx->vbuf || !ctx->ibuf) return;
    wgpuRenderPassEncoderSetVertexBuffer(pass, 0, ctx->vbuf, 0, ctx->vbuf_size);
    wgpuRenderPassEncoderSetIndexBuffer(pass, ctx->ibuf, WGPUIndexFormat_Uint32, 0, ctx->ibuf_size);
    wgpuRenderPassEncoderSetBindGroup(pass, 0, ctx->uniform_bg, 0, NULL);
    WGPURenderPipeline bound = NULL;
    WGPUBindGroup bound_bg = NULL;
    for (uint32_t i = 0; i < ctx->cmd_count; i++) {
        _cwg_Cmd *c = &ctx->cmds[i];
        if (c->type == _CWG_CMD_SCISSOR) {
            wgpuRenderPassEncoderSetScissorRect(pass, c->u.scissor.x, c->u.scissor.y, c->u.scissor.w, c->u.scissor.h);
            continue;
        }
        WGPURenderPipeline pip = c->u.draw.is_text ? ctx->pip_text : ctx->pip_color;
        if (pip != bound) {
            wgpuRenderPassEncoderSetPipeline(pass, pip);
            bound = pip;
        }
        if (c->u.draw.tex_bg != bound_bg) {
            wgpuRenderPassEncoderSetBindGroup(pass, 1, c->u.draw.tex_bg, 0, NULL);
            bound_bg = c->u.draw.tex_bg;
        }
        wgpuRenderPassEncoderDrawIndexed(pass, c->u.draw.idx_count, 1, c->u.draw.idx_off, 0, 0);
    }
}

#endif /* CLAY_RENDERER_WEBGPU_IMPL_GUARD */
#endif /* CLAY_RENDERER_WEBGPU_IMPL */

#if defined(SOKOL_IMPL) && !defined(SOKOL_GLUE_IMPL)
#define SOKOL_GLUE_IMPL
#endif
#ifndef SOKOL_GLUE_INCLUDED
/*
    sokol_glue.h -- glue helper functions for sokol headers

    Project URL: https://github.com/floooh/sokol

    Do this:
        #define SOKOL_IMPL or
        #define SOKOL_GLUE_IMPL
    before you include this file in *one* C or C++ file to create the
    implementation.

    ...optionally provide the following macros to override defaults:

    SOKOL_ASSERT(c)     - your own assert macro (default: assert(c))
    SOKOL_GLUE_API_DECL - public function declaration prefix (default: extern)
    SOKOL_API_DECL      - same as SOKOL_GLUE_API_DECL
    SOKOL_API_IMPL      - public function implementation prefix (default: -)

    If sokol_glue.h is compiled as a DLL, define the following before
    including the declaration or implementation:

    SOKOL_DLL

    On Windows, SOKOL_DLL will define SOKOL_GLUE_API_DECL as __declspec(dllexport)
    or __declspec(dllimport) as needed.

    OVERVIEW
    ========
    The sokol core headers should not depend on each other, but sometimes
    it's useful to have a set of helper functions as "glue" between
    two or more sokol headers.

    This is what sokol_glue.h is for. Simply include the header after other
    sokol headers (both for the implementation and declaration), and
    depending on what headers have been included before, sokol_glue.h
    will make available "glue functions".

    PROVIDED FUNCTIONS
    ==================

    - if sokol_app.h and sokol_gfx.h is included:

        sg_context_desc sapp_sgcontext(void):

            Returns an initialized sg_context_desc function initialized
            by calling sokol_app.h functions.

    LICENSE
    =======
    zlib/libpng license

    Copyright (c) 2018 Andre Weissflog

    This software is provided 'as-is', without any express or implied warranty.
    In no event will the authors be held liable for any damages arising from the
    use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software in a
        product, an acknowledgment in the product documentation would be
        appreciated but is not required.

        2. Altered source versions must be plainly marked as such, and must not
        be misrepresented as being the original software.

        3. This notice may not be removed or altered from any source
        distribution.
*/
#define SOKOL_GLUE_INCLUDED

#if defined(SOKOL_API_DECL) && !defined(SOKOL_GLUE_API_DECL)
#define SOKOL_GLUE_API_DECL SOKOL_API_DECL
#endif
#ifndef SOKOL_GLUE_API_DECL
#if defined(_WIN32) && defined(SOKOL_DLL) && defined(SOKOL_GLUE_IMPL)
#define SOKOL_GLUE_API_DECL __declspec(dllexport)
#elif defined(_WIN32) && defined(SOKOL_DLL)
#define SOKOL_GLUE_API_DECL __declspec(dllimport)
#else
#define SOKOL_GLUE_API_DECL extern
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SOKOL_GFX_INCLUDED) && defined(SOKOL_APP_INCLUDED)
SOKOL_GLUE_API_DECL sg_context_desc sapp_sgcontext(void);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* SOKOL_GLUE_INCLUDED */

/*-- IMPLEMENTATION ----------------------------------------------------------*/
#ifdef SOKOL_GLUE_IMPL
#define SOKOL_GLUE_IMPL_INCLUDED (1)
#include <string.h> /* memset */

#ifndef SOKOL_API_IMPL
    #define SOKOL_API_IMPL
#endif

#if defined(SOKOL_GFX_INCLUDED) && defined(SOKOL_APP_INCLUDED)
SOKOL_API_IMPL sg_context_desc sapp_sgcontext(void) {
    sg_context_desc desc;
    memset(&desc, 0, sizeof(desc));
    desc.color_format = (sg_pixel_format) sapp_color_format();
    desc.depth_format = (sg_pixel_format) sapp_depth_format();
    desc.sample_count = sapp_sample_count();
    desc.gl.force_gles2 = sapp_gles2();
    desc.metal.device = sapp_metal_get_device();
    desc.metal.renderpass_descriptor_cb = sapp_metal_get_renderpass_descriptor;
    desc.metal.drawable_cb = sapp_metal_get_drawable;
    desc.d3d11.device = sapp_d3d11_get_device();
    desc.d3d11.device_context = sapp_d3d11_get_device_context();
    desc.d3d11.render_target_view_cb = sapp_d3d11_get_render_target_view;
    desc.d3d11.depth_stencil_view_cb = sapp_d3d11_get_depth_stencil_view;
    desc.wgpu.device = sapp_wgpu_get_device();
    desc.wgpu.render_view_cb = sapp_wgpu_get_render_view;
    desc.wgpu.resolve_view_cb = sapp_wgpu_get_resolve_view;
    desc.wgpu.depth_stencil_view_cb = sapp_wgpu_get_depth_stencil_view;
    return desc;
}
#endif

#endif /* SOKOL_GLUE_IMPL */

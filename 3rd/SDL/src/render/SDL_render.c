/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2018 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "../SDL_internal.h"

/* The SDL 2D rendering system */

#include "SDL_assert.h"
#include "SDL_hints.h"
#include "SDL_log.h"
#include "SDL_render.h"
#include "SDL_sysrender.h"
#include "software/SDL_render_sw_c.h"


#define SDL_WINDOWRENDERDATA    "_SDL_WindowRenderData"

#define CHECK_RENDERER_MAGIC(renderer, retval) \
    SDL_assert(renderer && renderer->magic == &renderer_magic); \
    if (!renderer || renderer->magic != &renderer_magic) { \
        SDL_SetError("Invalid renderer"); \
        return retval; \
    }

#define CHECK_TEXTURE_MAGIC(texture, retval) \
    SDL_assert(texture && texture->magic == &texture_magic); \
    if (!texture || texture->magic != &texture_magic) { \
        SDL_SetError("Invalid texture"); \
        return retval; \
    }

/* Predefined blend modes */
#define SDL_COMPOSE_BLENDMODE(srcColorFactor, dstColorFactor, colorOperation, \
                              srcAlphaFactor, dstAlphaFactor, alphaOperation) \
    (SDL_BlendMode)(((Uint32)colorOperation << 0) | \
                    ((Uint32)srcColorFactor << 4) | \
                    ((Uint32)dstColorFactor << 8) | \
                    ((Uint32)alphaOperation << 16) | \
                    ((Uint32)srcAlphaFactor << 20) | \
                    ((Uint32)dstAlphaFactor << 24))

#define SDL_BLENDMODE_NONE_FULL \
    SDL_COMPOSE_BLENDMODE(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD, \
                          SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD)

#define SDL_BLENDMODE_BLEND_FULL \
    SDL_COMPOSE_BLENDMODE(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, \
                          SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD)

#define SDL_BLENDMODE_ADD_FULL \
    SDL_COMPOSE_BLENDMODE(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, \
                          SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD)

#define SDL_BLENDMODE_MOD_FULL \
    SDL_COMPOSE_BLENDMODE(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_SRC_COLOR, SDL_BLENDOPERATION_ADD, \
                          SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD)

#if !SDL_RENDER_DISABLED
static const SDL_RenderDriver *render_drivers[] = {
#if SDL_VIDEO_RENDER_D3D11
    &D3D11_RenderDriver,
#endif
#if SDL_VIDEO_RENDER_D3D
    &D3D_RenderDriver,
#endif
#if SDL_VIDEO_RENDER_OGL
    &GL_RenderDriver,
#endif
#if SDL_VIDEO_RENDER_OGL_ES2
    &GLES2_RenderDriver,
#endif
#if SDL_VIDEO_RENDER_OGL_ES
    &GLES_RenderDriver,
#endif
#if SDL_VIDEO_RENDER_DIRECTFB
    &DirectFB_RenderDriver,
#endif
#if SDL_VIDEO_RENDER_METAL
    &METAL_RenderDriver,
#endif
#if SDL_VIDEO_RENDER_PSP
    &PSP_RenderDriver,
#endif
    &SW_RenderDriver
};
#endif /* !SDL_RENDER_DISABLED */

static char renderer_magic;
static char texture_magic;

static int UpdateLogicalSize(SDL_Renderer *renderer);

int
SDL_GetNumRenderDrivers(void)
{
#if !SDL_RENDER_DISABLED
    return SDL_arraysize(render_drivers);
#else
    return 0;
#endif
}

int
SDL_GetRenderDriverInfo(int index, SDL_RendererInfo * info)
{
#if !SDL_RENDER_DISABLED
    if (index < 0 || index >= SDL_GetNumRenderDrivers()) {
        return SDL_SetError("index must be in the range of 0 - %d",
                            SDL_GetNumRenderDrivers() - 1);
    }
    *info = render_drivers[index]->info;
    return 0;
#else
    return SDL_SetError("SDL not built with rendering support");
#endif
}

static void GetWindowViewportValues(SDL_Renderer *renderer, int *logical_w, int *logical_h, SDL_Rect *viewport, SDL_FPoint *scale)
{
    SDL_LockMutex(renderer->target_mutex);
    *logical_w = renderer->target ? renderer->logical_w_backup : renderer->logical_w;
    *logical_h = renderer->target ? renderer->logical_h_backup : renderer->logical_h;
    *viewport = renderer->target ? renderer->viewport_backup : renderer->viewport;
    *scale = renderer->target ? renderer->scale_backup : renderer->scale;
    SDL_UnlockMutex(renderer->target_mutex);
}

static int SDLCALL
SDL_RendererEventWatch(void *userdata, SDL_Event *event)
{
    SDL_Renderer *renderer = (SDL_Renderer *)userdata;

    if (event->type == SDL_WINDOWEVENT) {
        SDL_Window *window = SDL_GetWindowFromID(event->window.windowID);
        if (window == renderer->window) {
            if (renderer->WindowEvent) {
                renderer->WindowEvent(renderer, &event->window);
            }

            if (event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                /* Make sure we're operating on the default render target */
                SDL_Texture *saved_target = SDL_GetRenderTarget(renderer);
                if (saved_target) {
                    SDL_SetRenderTarget(renderer, NULL);
                }

                if (renderer->logical_w) {
                    UpdateLogicalSize(renderer);
                } else {
                    /* Window was resized, reset viewport */
                    int w, h;

                    if (renderer->GetOutputSize) {
                        renderer->GetOutputSize(renderer, &w, &h);
                    } else {
						SDL_GetWindowSizeInside(renderer->window, &w, &h);
                    }

                    if (renderer->target) {
                        renderer->viewport_backup.x = 0;
                        renderer->viewport_backup.y = 0;
                        renderer->viewport_backup.w = w;
                        renderer->viewport_backup.h = h;
                    } else {
                        renderer->viewport.x = 0;
                        renderer->viewport.y = 0;
                        renderer->viewport.w = w;
                        renderer->viewport.h = h;
                        renderer->UpdateViewport(renderer);
                    }
                }

                if (saved_target) {
                    SDL_SetRenderTarget(renderer, saved_target);
                }
            } else if (event->window.event == SDL_WINDOWEVENT_HIDDEN) {
                renderer->hidden = SDL_TRUE;
            } else if (event->window.event == SDL_WINDOWEVENT_SHOWN) {
                if (!(SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)) {
                    renderer->hidden = SDL_FALSE;
                }
            } else if (event->window.event == SDL_WINDOWEVENT_MINIMIZED) {
                renderer->hidden = SDL_TRUE;
            } else if (event->window.event == SDL_WINDOWEVENT_RESTORED || 
                       event->window.event == SDL_WINDOWEVENT_MAXIMIZED) {
                if (!(SDL_GetWindowFlags(window) & SDL_WINDOW_HIDDEN)) {
                    renderer->hidden = SDL_FALSE;
                }
            }
        }
    } else if (event->type == SDL_MOUSEMOTION) {
        SDL_Window *window = SDL_GetWindowFromID(event->motion.windowID);
        if (window == renderer->window) {
            int logical_w, logical_h;
            SDL_Rect viewport;
            SDL_FPoint scale;
            GetWindowViewportValues(renderer, &logical_w, &logical_h, &viewport, &scale);
            if (logical_w) {
                event->motion.x -= (int)(viewport.x * renderer->dpi_scale.x);
                event->motion.y -= (int)(viewport.y * renderer->dpi_scale.y);
                event->motion.x = (int)(event->motion.x / (scale.x * renderer->dpi_scale.x));
                event->motion.y = (int)(event->motion.y / (scale.y * renderer->dpi_scale.y));
                if (event->motion.xrel > 0) {
                    event->motion.xrel = SDL_max(1, (int)(event->motion.xrel / (scale.x * renderer->dpi_scale.x)));
                } else if (event->motion.xrel < 0) {
                    event->motion.xrel = SDL_min(-1, (int)(event->motion.xrel / (scale.x * renderer->dpi_scale.x)));
                }
                if (event->motion.yrel > 0) {
                    event->motion.yrel = SDL_max(1, (int)(event->motion.yrel / (scale.y * renderer->dpi_scale.y)));
                } else if (event->motion.yrel < 0) {
                    event->motion.yrel = SDL_min(-1, (int)(event->motion.yrel / (scale.y * renderer->dpi_scale.y)));
                }
            }
        }
    } else if (event->type == SDL_MOUSEBUTTONDOWN ||
               event->type == SDL_MOUSEBUTTONUP) {
        SDL_Window *window = SDL_GetWindowFromID(event->button.windowID);
        if (window == renderer->window) {
            int logical_w, logical_h;
            SDL_Rect viewport;
            SDL_FPoint scale;
            GetWindowViewportValues(renderer, &logical_w, &logical_h, &viewport, &scale);
            if (logical_w) {
                event->button.x -= (int)(viewport.x * renderer->dpi_scale.x);
                event->button.y -= (int)(viewport.y * renderer->dpi_scale.y);
                event->button.x = (int)(event->button.x / (scale.x * renderer->dpi_scale.x));
                event->button.y = (int)(event->button.y / (scale.y * renderer->dpi_scale.y));
            }
        }
    } else if (event->type == SDL_FINGERDOWN ||
               event->type == SDL_FINGERUP ||
               event->type == SDL_FINGERMOTION) {
        int logical_w, logical_h;
        SDL_Rect viewport;
        SDL_FPoint scale;
        GetWindowViewportValues(renderer, &logical_w, &logical_h, &viewport, &scale);
        if (logical_w) {
            int w = 1;
            int h = 1;
            SDL_GetRendererOutputSize(renderer, &w, &h);

            event->tfinger.x *= (w - 1);
            event->tfinger.y *= (h - 1);

            event->tfinger.x -= (viewport.x * renderer->dpi_scale.x);
            event->tfinger.y -= (viewport.y * renderer->dpi_scale.y);
            event->tfinger.x = (event->tfinger.x / (scale.x * renderer->dpi_scale.x));
            event->tfinger.y = (event->tfinger.y / (scale.y * renderer->dpi_scale.y));

            if (logical_w > 1) {
                event->tfinger.x = event->tfinger.x / (logical_w - 1);
            } else {
                event->tfinger.x = 0.5f;
            }
            if (logical_h > 1) {
                event->tfinger.y = event->tfinger.y / (logical_h - 1);
            } else {
                event->tfinger.y = 0.5f;
            }
        }
    }

    return 0;
}

int
SDL_CreateWindowAndRenderer(int width, int height, Uint32 window_flags,
                            SDL_Window **window, SDL_Renderer **renderer)
{
    *window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED,
                                     SDL_WINDOWPOS_UNDEFINED,
                                     width, height, window_flags);
    if (!*window) {
        *renderer = NULL;
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, 0);
    if (!*renderer) {
        return -1;
    }

    return 0;
}

SDL_Renderer *
SDL_CreateRenderer(SDL_Window * window, int index, Uint32 flags)
{
#if !SDL_RENDER_DISABLED
    SDL_Renderer *renderer = NULL;
    int n = SDL_GetNumRenderDrivers();
    const char *hint;

    if (!window) {
        SDL_SetError("Invalid window");
        return NULL;
    }

    if (SDL_GetRenderer(window)) {
        SDL_SetError("Renderer already associated with window");
        return NULL;
    }

    if (SDL_GetHint(SDL_HINT_RENDER_VSYNC)) {
        if (SDL_GetHintBoolean(SDL_HINT_RENDER_VSYNC, SDL_TRUE)) {
            flags |= SDL_RENDERER_PRESENTVSYNC;
        } else {
            flags &= ~SDL_RENDERER_PRESENTVSYNC;
        }
    }

    if (index < 0) {
        hint = SDL_GetHint(SDL_HINT_RENDER_DRIVER);
        if (hint) {
            for (index = 0; index < n; ++index) {
                const SDL_RenderDriver *driver = render_drivers[index];

                if (SDL_strcasecmp(hint, driver->info.name) == 0) {
                    /* Create a new renderer instance */
                    renderer = driver->CreateRenderer(window, flags);
                    break;
                }
            }
        }

        if (!renderer) {
            for (index = 0; index < n; ++index) {
                const SDL_RenderDriver *driver = render_drivers[index];

                if ((driver->info.flags & flags) == flags) {
                    /* Create a new renderer instance */
                    renderer = driver->CreateRenderer(window, flags);
                    if (renderer) {
                        /* Yay, we got one! */
                        break;
                    }
                }
            }
        }
        if (index == n) {
            SDL_SetError("Couldn't find matching render driver");
            return NULL;
        }
    } else {
        if (index >= SDL_GetNumRenderDrivers()) {
            SDL_SetError("index must be -1 or in the range of 0 - %d",
                         SDL_GetNumRenderDrivers() - 1);
            return NULL;
        }
        /* Create a new renderer instance */
        renderer = render_drivers[index]->CreateRenderer(window, flags);
    }

    if (renderer) {
        renderer->magic = &renderer_magic;
        renderer->window = window;
        renderer->target_mutex = SDL_CreateMutex();
        renderer->scale.x = 1.0f;
        renderer->scale.y = 1.0f;
        renderer->dpi_scale.x = 1.0f;
        renderer->dpi_scale.y = 1.0f;

        if (window && renderer->GetOutputSize) {
            int window_w, window_h;
            int output_w, output_h;
            if (renderer->GetOutputSize(renderer, &output_w, &output_h) == 0) {
				SDL_GetWindowSizeInside(renderer->window, &window_w, &window_h);
                renderer->dpi_scale.x = (float)window_w / output_w;
                renderer->dpi_scale.y = (float)window_h / output_h;
            }
        }

        if (SDL_GetWindowFlags(window) & (SDL_WINDOW_HIDDEN|SDL_WINDOW_MINIMIZED)) {
            renderer->hidden = SDL_TRUE;
        } else {
            renderer->hidden = SDL_FALSE;
        }

        SDL_SetWindowData(window, SDL_WINDOWRENDERDATA, renderer);

        SDL_RenderSetViewport(renderer, NULL);

        SDL_AddEventWatch(SDL_RendererEventWatch, renderer);

        SDL_LogInfo(SDL_LOG_CATEGORY_RENDER,
                    "Created renderer: %s", renderer->info.name);
    }
    return renderer;
#else
    SDL_SetError("SDL not built with rendering support");
    return NULL;
#endif
}

SDL_Renderer *
SDL_CreateSoftwareRenderer(SDL_Surface * surface)
{
#if !SDL_RENDER_DISABLED
    SDL_Renderer *renderer;

    renderer = SW_CreateRendererForSurface(surface);

    if (renderer) {
        renderer->magic = &renderer_magic;
        renderer->target_mutex = SDL_CreateMutex();
        renderer->scale.x = 1.0f;
        renderer->scale.y = 1.0f;

        SDL_RenderSetViewport(renderer, NULL);
    }
    return renderer;
#else
    SDL_SetError("SDL not built with rendering support");
    return NULL;
#endif /* !SDL_RENDER_DISABLED */
}

SDL_Renderer *
SDL_GetRenderer(SDL_Window * window)
{
    return (SDL_Renderer *)SDL_GetWindowData(window, SDL_WINDOWRENDERDATA);
}

int
SDL_GetRendererInfo(SDL_Renderer * renderer, SDL_RendererInfo * info)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    *info = renderer->info;
    return 0;
}

int
SDL_GetRendererOutputSize(SDL_Renderer * renderer, int *w, int *h)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    if (renderer->target) {
        return SDL_QueryTexture(renderer->target, NULL, NULL, w, h);
    } else if (renderer->GetOutputSize) {
        return renderer->GetOutputSize(renderer, w, h);
    } else if (renderer->window) {
		SDL_GetWindowSizeInside(renderer->window, w, h);
        return 0;
    } else {
        SDL_assert(0 && "This should never happen");
        return SDL_SetError("Renderer doesn't support querying output size");
    }
}

static SDL_bool
IsSupportedBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode)
{
    switch (blendMode)
    {
    /* These are required to be supported by all renderers */
    case SDL_BLENDMODE_NONE:
    case SDL_BLENDMODE_BLEND:
    case SDL_BLENDMODE_ADD:
    case SDL_BLENDMODE_MOD:
        return SDL_TRUE;

    default:
        return renderer->SupportsBlendMode && renderer->SupportsBlendMode(renderer, blendMode);
    }
}

static SDL_bool
IsSupportedFormat(SDL_Renderer * renderer, Uint32 format)
{
    Uint32 i;

    for (i = 0; i < renderer->info.num_texture_formats; ++i) {
        if (renderer->info.texture_formats[i] == format) {
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

static Uint32
GetClosestSupportedFormat(SDL_Renderer * renderer, Uint32 format)
{
    Uint32 i;

    if (SDL_ISPIXELFORMAT_FOURCC(format)) {
        /* Look for an exact match */
        for (i = 0; i < renderer->info.num_texture_formats; ++i) {
            if (renderer->info.texture_formats[i] == format) {
                return renderer->info.texture_formats[i];
            }
        }
    } else {
        SDL_bool hasAlpha = SDL_ISPIXELFORMAT_ALPHA(format);

        /* We just want to match the first format that has the same channels */
        for (i = 0; i < renderer->info.num_texture_formats; ++i) {
            if (!SDL_ISPIXELFORMAT_FOURCC(renderer->info.texture_formats[i]) &&
                SDL_ISPIXELFORMAT_ALPHA(renderer->info.texture_formats[i]) == hasAlpha) {
                return renderer->info.texture_formats[i];
            }
        }
    }
    return renderer->info.texture_formats[0];
}

SDL_ScaleMode SDL_GetScaleMode(void)
{
    const char *hint = SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY);

    if (!hint || SDL_strcasecmp(hint, "nearest") == 0) {
        return SDL_ScaleModeNearest;
    } else if (SDL_strcasecmp(hint, "linear") == 0) {
        return SDL_ScaleModeLinear;
    } else if (SDL_strcasecmp(hint, "best") == 0) {
        return SDL_ScaleModeBest;
    } else {
        return (SDL_ScaleMode)SDL_atoi(hint);
    }
}

SDL_Texture *
SDL_CreateTexture(SDL_Renderer * renderer, Uint32 format, int access, int w, int h)
{
    SDL_Texture *texture;

    CHECK_RENDERER_MAGIC(renderer, NULL);

    if (!format) {
        format = renderer->info.texture_formats[0];
    }
    if (SDL_BYTESPERPIXEL(format) == 0) {
        SDL_SetError("Invalid texture format");
        return NULL;
    }
    if (SDL_ISPIXELFORMAT_INDEXED(format)) {
        SDL_SetError("Palettized textures are not supported");
        return NULL;
    }
    if (w <= 0 || h <= 0) {
        SDL_SetError("Texture dimensions can't be 0");
        return NULL;
    }
    if ((renderer->info.max_texture_width && w > renderer->info.max_texture_width) ||
        (renderer->info.max_texture_height && h > renderer->info.max_texture_height)) {
        SDL_SetError("Texture dimensions are limited to %dx%d", renderer->info.max_texture_width, renderer->info.max_texture_height);
        return NULL;
    }
    texture = (SDL_Texture *) SDL_calloc(1, sizeof(*texture));
    if (!texture) {
        SDL_OutOfMemory();
        return NULL;
    }
    texture->magic = &texture_magic;
    texture->format = format;
    texture->access = access;
    texture->w = w;
    texture->h = h;
    texture->r = 255;
    texture->g = 255;
    texture->b = 255;
    texture->a = 255;
    texture->scaleMode = SDL_GetScaleMode();
    texture->renderer = renderer;
    texture->next = renderer->textures;
    if (renderer->textures) {
        renderer->textures->prev = texture;
    }
    renderer->textures = texture;

    if (IsSupportedFormat(renderer, format)) {
        if (renderer->CreateTexture(renderer, texture) < 0) {
            SDL_DestroyTexture(texture);
            return NULL;
        }
    } else {
        texture->native = SDL_CreateTexture(renderer,
                                GetClosestSupportedFormat(renderer, format),
                                access, w, h);
        if (!texture->native) {
            SDL_DestroyTexture(texture);
            return NULL;
        }

        /* Swap textures to have texture before texture->native in the list */
        texture->native->next = texture->next;
        if (texture->native->next) {
            texture->native->next->prev = texture->native;
        }
        texture->prev = texture->native->prev;
        if (texture->prev) {
            texture->prev->next = texture;
        }
        texture->native->prev = texture;
        texture->next = texture->native;
        renderer->textures = texture;

        if (SDL_ISPIXELFORMAT_FOURCC(texture->format)) {
            texture->yuv = SDL_SW_CreateYUVTexture(format, w, h);
            if (!texture->yuv) {
                SDL_DestroyTexture(texture);
                return NULL;
            }
        } else if (access == SDL_TEXTUREACCESS_STREAMING) {
            /* The pitch is 4 byte aligned */
            texture->pitch = (((w * SDL_BYTESPERPIXEL(format)) + 3) & ~3);
            texture->pixels = SDL_calloc(1, texture->pitch * h);
            if (!texture->pixels) {
                SDL_DestroyTexture(texture);
                return NULL;
            }
        }
    }
    return texture;
}

SDL_Texture *
SDL_CreateTextureFromSurface(SDL_Renderer * renderer, SDL_Surface * surface)
{
    const SDL_PixelFormat *fmt;
    SDL_bool needAlpha;
    Uint32 i;
    Uint32 format;
    SDL_Texture *texture;

    CHECK_RENDERER_MAGIC(renderer, NULL);

    if (!surface) {
        SDL_SetError("SDL_CreateTextureFromSurface() passed NULL surface");
        return NULL;
    }

    /* See what the best texture format is */
    fmt = surface->format;
    if (fmt->Amask || SDL_HasColorKey(surface)) {
        needAlpha = SDL_TRUE;
    } else {
        needAlpha = SDL_FALSE;
    }
    format = renderer->info.texture_formats[0];
    for (i = 0; i < renderer->info.num_texture_formats; ++i) {
        if (!SDL_ISPIXELFORMAT_FOURCC(renderer->info.texture_formats[i]) &&
            SDL_ISPIXELFORMAT_ALPHA(renderer->info.texture_formats[i]) == needAlpha) {
            format = renderer->info.texture_formats[i];
            break;
        }
    }

    texture = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_STATIC,
                                surface->w, surface->h);
    if (!texture) {
        return NULL;
    }

    if (format == surface->format->format) {
        if (SDL_MUSTLOCK(surface)) {
            SDL_LockSurface(surface);
            SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
            SDL_UnlockSurface(surface);
        } else {
            SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
        }
    } else {
        SDL_PixelFormat *dst_fmt;
        SDL_Surface *temp = NULL;

        /* Set up a destination surface for the texture update */
        dst_fmt = SDL_AllocFormat(format);
        if (!dst_fmt) {
           SDL_DestroyTexture(texture);
           return NULL;
        }
        temp = SDL_ConvertSurface(surface, dst_fmt, 0);
        SDL_FreeFormat(dst_fmt);
        if (temp) {
            SDL_UpdateTexture(texture, NULL, temp->pixels, temp->pitch);
            SDL_FreeSurface(temp);
        } else {
            SDL_DestroyTexture(texture);
            return NULL;
        }
    }

    {
        Uint8 r, g, b, a;
        SDL_BlendMode blendMode;

        SDL_GetSurfaceColorMod(surface, &r, &g, &b);
        SDL_SetTextureColorMod(texture, r, g, b);

        SDL_GetSurfaceAlphaMod(surface, &a);
        SDL_SetTextureAlphaMod(texture, a);

        if (SDL_HasColorKey(surface)) {
            /* We converted to a texture with alpha format */
            SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        } else {
            SDL_GetSurfaceBlendMode(surface, &blendMode);
            SDL_SetTextureBlendMode(texture, blendMode);
        }
    }
    return texture;
}

int
SDL_QueryTexture(SDL_Texture * texture, Uint32 * format, int *access,
                 int *w, int *h)
{
    CHECK_TEXTURE_MAGIC(texture, -1);

    if (format) {
        *format = texture->format;
    }
    if (access) {
        *access = texture->access;
    }
    if (w) {
        *w = texture->w;
    }
    if (h) {
        *h = texture->h;
    }
    return 0;
}

int
SDL_SetTextureColorMod(SDL_Texture * texture, Uint8 r, Uint8 g, Uint8 b)
{
    SDL_Renderer *renderer;

    CHECK_TEXTURE_MAGIC(texture, -1);

    renderer = texture->renderer;
    if (r < 255 || g < 255 || b < 255) {
        texture->modMode |= SDL_TEXTUREMODULATE_COLOR;
    } else {
        texture->modMode &= ~SDL_TEXTUREMODULATE_COLOR;
    }
    texture->r = r;
    texture->g = g;
    texture->b = b;
    if (texture->native) {
        return SDL_SetTextureColorMod(texture->native, r, g, b);
    } else if (renderer->SetTextureColorMod) {
        return renderer->SetTextureColorMod(renderer, texture);
    } else {
        return 0;
    }
}

int
SDL_GetTextureColorMod(SDL_Texture * texture, Uint8 * r, Uint8 * g,
                       Uint8 * b)
{
    CHECK_TEXTURE_MAGIC(texture, -1);

    if (r) {
        *r = texture->r;
    }
    if (g) {
        *g = texture->g;
    }
    if (b) {
        *b = texture->b;
    }
    return 0;
}

int
SDL_SetTextureAlphaMod(SDL_Texture * texture, Uint8 alpha)
{
    SDL_Renderer *renderer;

    CHECK_TEXTURE_MAGIC(texture, -1);

    renderer = texture->renderer;
    if (alpha < 255) {
        texture->modMode |= SDL_TEXTUREMODULATE_ALPHA;
    } else {
        texture->modMode &= ~SDL_TEXTUREMODULATE_ALPHA;
    }
    texture->a = alpha;
    if (texture->native) {
        return SDL_SetTextureAlphaMod(texture->native, alpha);
    } else if (renderer->SetTextureAlphaMod) {
        return renderer->SetTextureAlphaMod(renderer, texture);
    } else {
        return 0;
    }
}

int
SDL_GetTextureAlphaMod(SDL_Texture * texture, Uint8 * alpha)
{
    CHECK_TEXTURE_MAGIC(texture, -1);

    if (alpha) {
        *alpha = texture->a;
    }
    return 0;
}

int
SDL_SetTextureBlendMode(SDL_Texture * texture, SDL_BlendMode blendMode)
{
    SDL_Renderer *renderer;

    CHECK_TEXTURE_MAGIC(texture, -1);

    renderer = texture->renderer;
    if (!IsSupportedBlendMode(renderer, blendMode)) {
        return SDL_Unsupported();
    }
    texture->blendMode = blendMode;
    if (texture->native) {
        return SDL_SetTextureBlendMode(texture->native, blendMode);
    } else if (renderer->SetTextureBlendMode) {
        return renderer->SetTextureBlendMode(renderer, texture);
    } else {
        return 0;
    }
}

int
SDL_GetTextureBlendMode(SDL_Texture * texture, SDL_BlendMode *blendMode)
{
    CHECK_TEXTURE_MAGIC(texture, -1);

    if (blendMode) {
        *blendMode = texture->blendMode;
    }
    return 0;
}

static int
SDL_UpdateTextureYUV(SDL_Texture * texture, const SDL_Rect * rect,
                     const void *pixels, int pitch)
{
    SDL_Texture *native = texture->native;
    SDL_Rect full_rect;

    if (SDL_SW_UpdateYUVTexture(texture->yuv, rect, pixels, pitch) < 0) {
        return -1;
    }

    full_rect.x = 0;
    full_rect.y = 0;
    full_rect.w = texture->w;
    full_rect.h = texture->h;
    rect = &full_rect;

    if (texture->access == SDL_TEXTUREACCESS_STREAMING) {
        /* We can lock the texture and copy to it */
        void *native_pixels = NULL;
        int native_pitch = 0;

        if (SDL_LockTexture(native, rect, &native_pixels, &native_pitch) < 0) {
            return -1;
        }
        SDL_SW_CopyYUVToRGB(texture->yuv, rect, native->format,
                            rect->w, rect->h, native_pixels, native_pitch);
        SDL_UnlockTexture(native);
    } else {
        /* Use a temporary buffer for updating */
        const int temp_pitch = (((rect->w * SDL_BYTESPERPIXEL(native->format)) + 3) & ~3);
        const size_t alloclen = rect->h * temp_pitch;
        if (alloclen > 0) {
            void *temp_pixels = SDL_malloc(alloclen);
            if (!temp_pixels) {
                return SDL_OutOfMemory();
            }
            SDL_SW_CopyYUVToRGB(texture->yuv, rect, native->format,
                                rect->w, rect->h, temp_pixels, temp_pitch);
            SDL_UpdateTexture(native, rect, temp_pixels, temp_pitch);
            SDL_free(temp_pixels);
        }
    }
    return 0;
}

static int
SDL_UpdateTextureNative(SDL_Texture * texture, const SDL_Rect * rect,
                        const void *pixels, int pitch)
{
    SDL_Texture *native = texture->native;

    if (!rect->w || !rect->h) {
        return 0;  /* nothing to do. */
    }

    if (texture->access == SDL_TEXTUREACCESS_STREAMING) {
        /* We can lock the texture and copy to it */
        void *native_pixels = NULL;
        int native_pitch = 0;

        if (SDL_LockTexture(native, rect, &native_pixels, &native_pitch) < 0) {
            return -1;
        }
        SDL_ConvertPixels(rect->w, rect->h,
                          texture->format, pixels, pitch,
                          native->format, native_pixels, native_pitch);
        SDL_UnlockTexture(native);
    } else {
        /* Use a temporary buffer for updating */
        const int temp_pitch = (((rect->w * SDL_BYTESPERPIXEL(native->format)) + 3) & ~3);
        const size_t alloclen = rect->h * temp_pitch;
        if (alloclen > 0) {
            void *temp_pixels = SDL_malloc(alloclen);
            if (!temp_pixels) {
                return SDL_OutOfMemory();
            }
            SDL_ConvertPixels(rect->w, rect->h,
                              texture->format, pixels, pitch,
                              native->format, temp_pixels, temp_pitch);
            SDL_UpdateTexture(native, rect, temp_pixels, temp_pitch);
            SDL_free(temp_pixels);
        }
    }
    return 0;
}

int
SDL_UpdateTexture(SDL_Texture * texture, const SDL_Rect * rect,
                  const void *pixels, int pitch)
{
    SDL_Renderer *renderer;
    SDL_Rect full_rect;

    CHECK_TEXTURE_MAGIC(texture, -1);

    if (!pixels) {
        return SDL_InvalidParamError("pixels");
    }
    if (!pitch) {
        return SDL_InvalidParamError("pitch");
    }

    if (!rect) {
        full_rect.x = 0;
        full_rect.y = 0;
        full_rect.w = texture->w;
        full_rect.h = texture->h;
        rect = &full_rect;
    }

    if ((rect->w == 0) || (rect->h == 0)) {
        return 0;  /* nothing to do. */
    } else if (texture->yuv) {
        return SDL_UpdateTextureYUV(texture, rect, pixels, pitch);
    } else if (texture->native) {
        return SDL_UpdateTextureNative(texture, rect, pixels, pitch);
    } else {
        renderer = texture->renderer;
        return renderer->UpdateTexture(renderer, texture, rect, pixels, pitch);
    }
}

static int
SDL_UpdateTextureYUVPlanar(SDL_Texture * texture, const SDL_Rect * rect,
                           const Uint8 *Yplane, int Ypitch,
                           const Uint8 *Uplane, int Upitch,
                           const Uint8 *Vplane, int Vpitch)
{
    SDL_Texture *native = texture->native;
    SDL_Rect full_rect;

    if (SDL_SW_UpdateYUVTexturePlanar(texture->yuv, rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch) < 0) {
        return -1;
    }

    full_rect.x = 0;
    full_rect.y = 0;
    full_rect.w = texture->w;
    full_rect.h = texture->h;
    rect = &full_rect;

    if (!rect->w || !rect->h) {
        return 0;  /* nothing to do. */
    }

    if (texture->access == SDL_TEXTUREACCESS_STREAMING) {
        /* We can lock the texture and copy to it */
        void *native_pixels = NULL;
        int native_pitch = 0;

        if (SDL_LockTexture(native, rect, &native_pixels, &native_pitch) < 0) {
            return -1;
        }
        SDL_SW_CopyYUVToRGB(texture->yuv, rect, native->format,
                            rect->w, rect->h, native_pixels, native_pitch);
        SDL_UnlockTexture(native);
    } else {
        /* Use a temporary buffer for updating */
        const int temp_pitch = (((rect->w * SDL_BYTESPERPIXEL(native->format)) + 3) & ~3);
        const size_t alloclen = rect->h * temp_pitch;
        if (alloclen > 0) {
            void *temp_pixels = SDL_malloc(alloclen);
            if (!temp_pixels) {
                return SDL_OutOfMemory();
            }
            SDL_SW_CopyYUVToRGB(texture->yuv, rect, native->format,
                                rect->w, rect->h, temp_pixels, temp_pitch);
            SDL_UpdateTexture(native, rect, temp_pixels, temp_pitch);
            SDL_free(temp_pixels);
        }
    }
    return 0;
}

int SDL_UpdateYUVTexture(SDL_Texture * texture, const SDL_Rect * rect,
                         const Uint8 *Yplane, int Ypitch,
                         const Uint8 *Uplane, int Upitch,
                         const Uint8 *Vplane, int Vpitch)
{
    SDL_Renderer *renderer;
    SDL_Rect full_rect;

    CHECK_TEXTURE_MAGIC(texture, -1);

    if (!Yplane) {
        return SDL_InvalidParamError("Yplane");
    }
    if (!Ypitch) {
        return SDL_InvalidParamError("Ypitch");
    }
    if (!Uplane) {
        return SDL_InvalidParamError("Uplane");
    }
    if (!Upitch) {
        return SDL_InvalidParamError("Upitch");
    }
    if (!Vplane) {
        return SDL_InvalidParamError("Vplane");
    }
    if (!Vpitch) {
        return SDL_InvalidParamError("Vpitch");
    }

    if (texture->format != SDL_PIXELFORMAT_YV12 &&
        texture->format != SDL_PIXELFORMAT_IYUV) {
        return SDL_SetError("Texture format must by YV12 or IYUV");
    }

    if (!rect) {
        full_rect.x = 0;
        full_rect.y = 0;
        full_rect.w = texture->w;
        full_rect.h = texture->h;
        rect = &full_rect;
    }

    if (!rect->w || !rect->h) {
        return 0;  /* nothing to do. */
    }

    if (texture->yuv) {
        return SDL_UpdateTextureYUVPlanar(texture, rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
    } else {
        SDL_assert(!texture->native);
        renderer = texture->renderer;
        SDL_assert(renderer->UpdateTextureYUV);
        if (renderer->UpdateTextureYUV) {
            return renderer->UpdateTextureYUV(renderer, texture, rect, Yplane, Ypitch, Uplane, Upitch, Vplane, Vpitch);
        } else {
            return SDL_Unsupported();
        }
    }
}

static int
SDL_LockTextureYUV(SDL_Texture * texture, const SDL_Rect * rect,
                   void **pixels, int *pitch)
{
    return SDL_SW_LockYUVTexture(texture->yuv, rect, pixels, pitch);
}

static int
SDL_LockTextureNative(SDL_Texture * texture, const SDL_Rect * rect,
                      void **pixels, int *pitch)
{
    texture->locked_rect = *rect;
    *pixels = (void *) ((Uint8 *) texture->pixels +
                        rect->y * texture->pitch +
                        rect->x * SDL_BYTESPERPIXEL(texture->format));
    *pitch = texture->pitch;
    return 0;
}

int
SDL_LockTexture(SDL_Texture * texture, const SDL_Rect * rect,
                void **pixels, int *pitch)
{
    SDL_Renderer *renderer;
    SDL_Rect full_rect;

    CHECK_TEXTURE_MAGIC(texture, -1);

    if (texture->access != SDL_TEXTUREACCESS_STREAMING) {
        return SDL_SetError("SDL_LockTexture(): texture must be streaming");
    }

    if (!rect) {
        full_rect.x = 0;
        full_rect.y = 0;
        full_rect.w = texture->w;
        full_rect.h = texture->h;
        rect = &full_rect;
    }

    if (texture->yuv) {
        return SDL_LockTextureYUV(texture, rect, pixels, pitch);
    } else if (texture->native) {
        return SDL_LockTextureNative(texture, rect, pixels, pitch);
    } else {
        renderer = texture->renderer;
        return renderer->LockTexture(renderer, texture, rect, pixels, pitch);
    }
}

static void
SDL_UnlockTextureYUV(SDL_Texture * texture)
{
    SDL_Texture *native = texture->native;
    void *native_pixels = NULL;
    int native_pitch = 0;
    SDL_Rect rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = texture->w;
    rect.h = texture->h;

    if (SDL_LockTexture(native, &rect, &native_pixels, &native_pitch) < 0) {
        return;
    }
    SDL_SW_CopyYUVToRGB(texture->yuv, &rect, native->format,
                        rect.w, rect.h, native_pixels, native_pitch);
    SDL_UnlockTexture(native);
}

static void
SDL_UnlockTextureNative(SDL_Texture * texture)
{
    SDL_Texture *native = texture->native;
    void *native_pixels = NULL;
    int native_pitch = 0;
    const SDL_Rect *rect = &texture->locked_rect;
    const void* pixels = (void *) ((Uint8 *) texture->pixels +
                        rect->y * texture->pitch +
                        rect->x * SDL_BYTESPERPIXEL(texture->format));
    int pitch = texture->pitch;

    if (SDL_LockTexture(native, rect, &native_pixels, &native_pitch) < 0) {
        return;
    }
    SDL_ConvertPixels(rect->w, rect->h,
                      texture->format, pixels, pitch,
                      native->format, native_pixels, native_pitch);
    SDL_UnlockTexture(native);
}

void
SDL_UnlockTexture(SDL_Texture * texture)
{
    SDL_Renderer *renderer;

    CHECK_TEXTURE_MAGIC(texture, );

    if (texture->access != SDL_TEXTUREACCESS_STREAMING) {
        return;
    }
    if (texture->yuv) {
        SDL_UnlockTextureYUV(texture);
    } else if (texture->native) {
        SDL_UnlockTextureNative(texture);
    } else {
        renderer = texture->renderer;
        renderer->UnlockTexture(renderer, texture);
    }
}

SDL_bool
SDL_RenderTargetSupported(SDL_Renderer *renderer)
{
    if (!renderer || !renderer->SetRenderTarget) {
        return SDL_FALSE;
    }
    return (renderer->info.flags & SDL_RENDERER_TARGETTEXTURE) != 0;
}

int
SDL_SetRenderTarget(SDL_Renderer *renderer, SDL_Texture *texture)
{
    if (!SDL_RenderTargetSupported(renderer)) {
        return SDL_Unsupported();
    }
    if (texture == renderer->target) {
        /* Nothing to do! */
        return 0;
    }

    /* texture == NULL is valid and means reset the target to the window */
    if (texture) {
        CHECK_TEXTURE_MAGIC(texture, -1);
        if (renderer != texture->renderer) {
            return SDL_SetError("Texture was not created with this renderer");
        }
        if (texture->access != SDL_TEXTUREACCESS_TARGET) {
            return SDL_SetError("Texture not created with SDL_TEXTUREACCESS_TARGET");
        }
        if (texture->native) {
            /* Always render to the native texture */
            texture = texture->native;
        }
    }

    SDL_LockMutex(renderer->target_mutex);

    if (texture && !renderer->target) {
        /* Make a backup of the viewport */
        renderer->viewport_backup = renderer->viewport;
        renderer->clip_rect_backup = renderer->clip_rect;
        renderer->clipping_enabled_backup = renderer->clipping_enabled;
        renderer->scale_backup = renderer->scale;
        renderer->logical_w_backup = renderer->logical_w;
        renderer->logical_h_backup = renderer->logical_h;
    }
    renderer->target = texture;

    if (renderer->SetRenderTarget(renderer, texture) < 0) {
        SDL_UnlockMutex(renderer->target_mutex);
        return -1;
    }

    if (texture) {
        renderer->viewport.x = 0;
        renderer->viewport.y = 0;
        renderer->viewport.w = texture->w;
        renderer->viewport.h = texture->h;
        SDL_zero(renderer->clip_rect);
        renderer->clipping_enabled = SDL_FALSE;
        renderer->scale.x = 1.0f;
        renderer->scale.y = 1.0f;
        renderer->logical_w = texture->w;
        renderer->logical_h = texture->h;
    } else {
        renderer->viewport = renderer->viewport_backup;
        renderer->clip_rect = renderer->clip_rect_backup;
        renderer->clipping_enabled = renderer->clipping_enabled_backup;
        renderer->scale = renderer->scale_backup;
        renderer->logical_w = renderer->logical_w_backup;
        renderer->logical_h = renderer->logical_h_backup;
    }

    SDL_UnlockMutex(renderer->target_mutex);

    if (renderer->UpdateViewport(renderer) < 0) {
        return -1;
    }
    if (renderer->UpdateClipRect(renderer) < 0) {
        return -1;
    }

    /* All set! */
    return 0;
}

SDL_Texture *
SDL_GetRenderTarget(SDL_Renderer *renderer)
{
    return renderer->target;
}

static int
UpdateLogicalSize(SDL_Renderer *renderer)
{
    int w = 1, h = 1;
    float want_aspect;
    float real_aspect;
    float scale;
    SDL_Rect viewport;
    /* 0 is for letterbox, 1 is for overscan */
    int scale_policy = 0;
    const char *hint;

    if (!renderer->logical_w || !renderer->logical_h) {
        return 0;
    }
    if (SDL_GetRendererOutputSize(renderer, &w, &h) < 0) {
        return -1;
    }

    hint = SDL_GetHint(SDL_HINT_RENDER_LOGICAL_SIZE_MODE);
    if (hint && (*hint == '1' || SDL_strcasecmp(hint, "overscan") == 0))  {
#if SDL_VIDEO_RENDER_D3D
        SDL_bool overscan_supported = SDL_TRUE;
        /* Unfortunately, Direct3D 9 doesn't support negative viewport numbers
           which the overscan implementation relies on.
        */
        if (SDL_strcasecmp(SDL_GetCurrentVideoDriver(), "direct3d") == 0) {
            overscan_supported = SDL_FALSE;
        }
        if (overscan_supported) {
            scale_policy = 1;
        }
#else
        scale_policy = 1;
#endif
    }

    want_aspect = (float)renderer->logical_w / renderer->logical_h;
    real_aspect = (float)w / h;

    /* Clear the scale because we're setting viewport in output coordinates */
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);

    if (renderer->integer_scale) {
        if (want_aspect > real_aspect) {
            scale = (float)(w / renderer->logical_w);
        } else {
            scale = (float)(h / renderer->logical_h);
        }
        viewport.w = (int)SDL_ceil(renderer->logical_w * scale);
        viewport.x = (w - viewport.w) / 2;
        viewport.h = (int)SDL_ceil(renderer->logical_h * scale);
        viewport.y = (h - viewport.h) / 2;

        SDL_RenderSetViewport(renderer, &viewport);
    } else if (SDL_fabs(want_aspect-real_aspect) < 0.0001) {
        /* The aspect ratios are the same, just scale appropriately */
        scale = (float)w / renderer->logical_w;
        SDL_RenderSetViewport(renderer, NULL);
    } else if (want_aspect > real_aspect) {
        if (scale_policy == 1) {
            /* We want a wider aspect ratio than is available - 
             zoom so logical height matches the real height 
             and the width will grow off the screen 
             */
            scale = (float)h / renderer->logical_h;
            viewport.y = 0;
            viewport.h = h;
            viewport.w = (int)SDL_ceil(renderer->logical_w * scale);
            viewport.x = (w - viewport.w) / 2;
            SDL_RenderSetViewport(renderer, &viewport);
        } else {
            /* We want a wider aspect ratio than is available - letterbox it */
            scale = (float)w / renderer->logical_w;
            viewport.x = 0;
            viewport.w = w;
            viewport.h = (int)SDL_ceil(renderer->logical_h * scale);
            viewport.y = (h - viewport.h) / 2;
            SDL_RenderSetViewport(renderer, &viewport);
        }
    } else {
        if (scale_policy == 1) {
            /* We want a narrower aspect ratio than is available -
             zoom so logical width matches the real width
             and the height will grow off the screen
             */
            scale = (float)w / renderer->logical_w;
            viewport.x = 0;
            viewport.w = w;
            viewport.h = (int)SDL_ceil(renderer->logical_h * scale);
            viewport.y = (h - viewport.h) / 2;
            SDL_RenderSetViewport(renderer, &viewport);
        } else {
            /* We want a narrower aspect ratio than is available - use side-bars */
             scale = (float)h / renderer->logical_h;
             viewport.y = 0;
             viewport.h = h;
             viewport.w = (int)SDL_ceil(renderer->logical_w * scale);
             viewport.x = (w - viewport.w) / 2;
             SDL_RenderSetViewport(renderer, &viewport);
        }
    }

    /* Set the new scale */
    SDL_RenderSetScale(renderer, scale, scale);

    return 0;
}

int
SDL_RenderSetLogicalSize(SDL_Renderer * renderer, int w, int h)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    if (!w || !h) {
        /* Clear any previous logical resolution */
        renderer->logical_w = 0;
        renderer->logical_h = 0;
        SDL_RenderSetViewport(renderer, NULL);
        SDL_RenderSetScale(renderer, 1.0f, 1.0f);
        return 0;
    }

    renderer->logical_w = w;
    renderer->logical_h = h;

    return UpdateLogicalSize(renderer);
}

void
SDL_RenderGetLogicalSize(SDL_Renderer * renderer, int *w, int *h)
{
    CHECK_RENDERER_MAGIC(renderer, );

    if (w) {
        *w = renderer->logical_w;
    }
    if (h) {
        *h = renderer->logical_h;
    }
}

int
SDL_RenderSetIntegerScale(SDL_Renderer * renderer, SDL_bool enable)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    renderer->integer_scale = enable;

    return UpdateLogicalSize(renderer);
}

SDL_bool
SDLCALL SDL_RenderGetIntegerScale(SDL_Renderer * renderer)
{
    CHECK_RENDERER_MAGIC(renderer, SDL_FALSE);

    return renderer->integer_scale;
}

int
SDL_RenderSetViewport(SDL_Renderer * renderer, const SDL_Rect * rect)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    if (rect) {
        renderer->viewport.x = (int)SDL_floor(rect->x * renderer->scale.x);
        renderer->viewport.y = (int)SDL_floor(rect->y * renderer->scale.y);
        renderer->viewport.w = (int)SDL_ceil(rect->w * renderer->scale.x);
        renderer->viewport.h = (int)SDL_ceil(rect->h * renderer->scale.y);
    } else {
        renderer->viewport.x = 0;
        renderer->viewport.y = 0;
        if (SDL_GetRendererOutputSize(renderer, &renderer->viewport.w, &renderer->viewport.h) < 0) {
            return -1;
        }
    }
    return renderer->UpdateViewport(renderer);
}

void
SDL_RenderGetViewport(SDL_Renderer * renderer, SDL_Rect * rect)
{
    CHECK_RENDERER_MAGIC(renderer, );

    if (rect) {
        rect->x = (int)(renderer->viewport.x / renderer->scale.x);
        rect->y = (int)(renderer->viewport.y / renderer->scale.y);
        rect->w = (int)(renderer->viewport.w / renderer->scale.x);
        rect->h = (int)(renderer->viewport.h / renderer->scale.y);
    }
}

int
SDL_RenderSetClipRect(SDL_Renderer * renderer, const SDL_Rect * rect)
{
    CHECK_RENDERER_MAGIC(renderer, -1)

    if (rect) {
        renderer->clipping_enabled = SDL_TRUE;
        renderer->clip_rect.x = (int)SDL_floor(rect->x * renderer->scale.x);
        renderer->clip_rect.y = (int)SDL_floor(rect->y * renderer->scale.y);
        renderer->clip_rect.w = (int)SDL_ceil(rect->w * renderer->scale.x);
        renderer->clip_rect.h = (int)SDL_ceil(rect->h * renderer->scale.y);
    } else {
        renderer->clipping_enabled = SDL_FALSE;
        SDL_zero(renderer->clip_rect);
    }
    return renderer->UpdateClipRect(renderer);
}

void
SDL_RenderGetClipRect(SDL_Renderer * renderer, SDL_Rect * rect)
{
    CHECK_RENDERER_MAGIC(renderer, )

    if (rect) {
        rect->x = (int)(renderer->clip_rect.x / renderer->scale.x);
        rect->y = (int)(renderer->clip_rect.y / renderer->scale.y);
        rect->w = (int)(renderer->clip_rect.w / renderer->scale.x);
        rect->h = (int)(renderer->clip_rect.h / renderer->scale.y);
    }
}

SDL_bool
SDL_RenderIsClipEnabled(SDL_Renderer * renderer)
{
    CHECK_RENDERER_MAGIC(renderer, SDL_FALSE)
    return renderer->clipping_enabled;
}

int
SDL_RenderSetScale(SDL_Renderer * renderer, float scaleX, float scaleY)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    renderer->scale.x = scaleX;
    renderer->scale.y = scaleY;
    return 0;
}

void
SDL_RenderGetScale(SDL_Renderer * renderer, float *scaleX, float *scaleY)
{
    CHECK_RENDERER_MAGIC(renderer, );

    if (scaleX) {
        *scaleX = renderer->scale.x;
    }
    if (scaleY) {
        *scaleY = renderer->scale.y;
    }
}

int
SDL_SetRenderDrawColor(SDL_Renderer * renderer,
                       Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    renderer->r = r;
    renderer->g = g;
    renderer->b = b;
    renderer->a = a;
    return 0;
}

int
SDL_GetRenderDrawColor(SDL_Renderer * renderer,
                       Uint8 * r, Uint8 * g, Uint8 * b, Uint8 * a)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    if (r) {
        *r = renderer->r;
    }
    if (g) {
        *g = renderer->g;
    }
    if (b) {
        *b = renderer->b;
    }
    if (a) {
        *a = renderer->a;
    }
    return 0;
}

int
SDL_SetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode blendMode)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    if (!IsSupportedBlendMode(renderer, blendMode)) {
        return SDL_Unsupported();
    }
    renderer->blendMode = blendMode;
    return 0;
}

int
SDL_GetRenderDrawBlendMode(SDL_Renderer * renderer, SDL_BlendMode *blendMode)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    *blendMode = renderer->blendMode;
    return 0;
}

int
SDL_RenderClear(SDL_Renderer * renderer)
{
    CHECK_RENDERER_MAGIC(renderer, -1);

    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return 0;
    }
    return renderer->RenderClear(renderer);
}

int
SDL_RenderDrawPoint(SDL_Renderer * renderer, int x, int y)
{
    SDL_Point point;

    point.x = x;
    point.y = y;
    return SDL_RenderDrawPoints(renderer, &point, 1);
}

static int
RenderDrawPointsWithRects(SDL_Renderer * renderer,
                     const SDL_Point * points, int count)
{
    SDL_FRect *frects;
    int i;
    int status;

    frects = SDL_stack_alloc(SDL_FRect, count);
    if (!frects) {
        return SDL_OutOfMemory();
    }
    for (i = 0; i < count; ++i) {
        frects[i].x = points[i].x * renderer->scale.x;
        frects[i].y = points[i].y * renderer->scale.y;
        frects[i].w = renderer->scale.x;
        frects[i].h = renderer->scale.y;
    }

    status = renderer->RenderFillRects(renderer, frects, count);

    SDL_stack_free(frects);

    return status;
}

int
SDL_RenderDrawPoints(SDL_Renderer * renderer,
                     const SDL_Point * points, int count)
{
    SDL_FPoint *fpoints;
    int i;
    int status;

    CHECK_RENDERER_MAGIC(renderer, -1);

    if (!points) {
        return SDL_SetError("SDL_RenderDrawPoints(): Passed NULL points");
    }
    if (count < 1) {
        return 0;
    }

    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return 0;
    }

    if (renderer->scale.x != 1.0f || renderer->scale.y != 1.0f) {
        return RenderDrawPointsWithRects(renderer, points, count);
    }

    fpoints = SDL_stack_alloc(SDL_FPoint, count);
    if (!fpoints) {
        return SDL_OutOfMemory();
    }
    for (i = 0; i < count; ++i) {
        fpoints[i].x = points[i].x * renderer->scale.x;
        fpoints[i].y = points[i].y * renderer->scale.y;
    }

    status = renderer->RenderDrawPoints(renderer, fpoints, count);

    SDL_stack_free(fpoints);

    return status;
}

int
SDL_RenderDrawLine(SDL_Renderer * renderer, int x1, int y1, int x2, int y2)
{
    SDL_Point points[2];

    points[0].x = x1;
    points[0].y = y1;
    points[1].x = x2;
    points[1].y = y2;
    return SDL_RenderDrawLines(renderer, points, 2);
}

static int
RenderDrawLinesWithRects(SDL_Renderer * renderer,
                     const SDL_Point * points, int count)
{
    SDL_FRect *frect;
    SDL_FRect *frects;
    SDL_FPoint fpoints[2];
    int i, nrects;
    int status;

    frects = SDL_stack_alloc(SDL_FRect, count-1);
    if (!frects) {
        return SDL_OutOfMemory();
    }

    status = 0;
    nrects = 0;
    for (i = 0; i < count-1; ++i) {
        if (points[i].x == points[i+1].x) {
            int minY = SDL_min(points[i].y, points[i+1].y);
            int maxY = SDL_max(points[i].y, points[i+1].y);

            frect = &frects[nrects++];
            frect->x = points[i].x * renderer->scale.x;
            frect->y = minY * renderer->scale.y;
            frect->w = renderer->scale.x;
            frect->h = (maxY - minY + 1) * renderer->scale.y;
        } else if (points[i].y == points[i+1].y) {
            int minX = SDL_min(points[i].x, points[i+1].x);
            int maxX = SDL_max(points[i].x, points[i+1].x);

            frect = &frects[nrects++];
            frect->x = minX * renderer->scale.x;
            frect->y = points[i].y * renderer->scale.y;
            frect->w = (maxX - minX + 1) * renderer->scale.x;
            frect->h = renderer->scale.y;
        } else {
            /* FIXME: We can't use a rect for this line... */
            fpoints[0].x = points[i].x * renderer->scale.x;
            fpoints[0].y = points[i].y * renderer->scale.y;
            fpoints[1].x = points[i+1].x * renderer->scale.x;
            fpoints[1].y = points[i+1].y * renderer->scale.y;
            status += renderer->RenderDrawLines(renderer, fpoints, 2);
        }
    }

    status += renderer->RenderFillRects(renderer, frects, nrects);

    SDL_stack_free(frects);

    if (status < 0) {
        status = -1;
    }
    return status;
}

int
SDL_RenderDrawLines(SDL_Renderer * renderer,
                    const SDL_Point * points, int count)
{
    SDL_FPoint *fpoints;
    int i;
    int status;

    CHECK_RENDERER_MAGIC(renderer, -1);

    if (!points) {
        return SDL_SetError("SDL_RenderDrawLines(): Passed NULL points");
    }
    if (count < 2) {
        return 0;
    }

    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return 0;
    }

    if (renderer->scale.x != 1.0f || renderer->scale.y != 1.0f) {
        return RenderDrawLinesWithRects(renderer, points, count);
    }

    fpoints = SDL_stack_alloc(SDL_FPoint, count);
    if (!fpoints) {
        return SDL_OutOfMemory();
    }
    for (i = 0; i < count; ++i) {
        fpoints[i].x = points[i].x * renderer->scale.x;
        fpoints[i].y = points[i].y * renderer->scale.y;
    }

    status = renderer->RenderDrawLines(renderer, fpoints, count);

    SDL_stack_free(fpoints);

    return status;
}

int
SDL_RenderDrawRect(SDL_Renderer * renderer, const SDL_Rect * rect)
{
    SDL_Rect full_rect;
    SDL_Point points[5];

    CHECK_RENDERER_MAGIC(renderer, -1);

    /* If 'rect' == NULL, then outline the whole surface */
    if (!rect) {
        SDL_RenderGetViewport(renderer, &full_rect);
        full_rect.x = 0;
        full_rect.y = 0;
        rect = &full_rect;
    }

    points[0].x = rect->x;
    points[0].y = rect->y;
    points[1].x = rect->x+rect->w-1;
    points[1].y = rect->y;
    points[2].x = rect->x+rect->w-1;
    points[2].y = rect->y+rect->h-1;
    points[3].x = rect->x;
    points[3].y = rect->y+rect->h-1;
    points[4].x = rect->x;
    points[4].y = rect->y;
    return SDL_RenderDrawLines(renderer, points, 5);
}

int
SDL_RenderDrawRects(SDL_Renderer * renderer,
                    const SDL_Rect * rects, int count)
{
    int i;

    CHECK_RENDERER_MAGIC(renderer, -1);

    if (!rects) {
        return SDL_SetError("SDL_RenderDrawRects(): Passed NULL rects");
    }
    if (count < 1) {
        return 0;
    }

    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return 0;
    }

    for (i = 0; i < count; ++i) {
        if (SDL_RenderDrawRect(renderer, &rects[i]) < 0) {
            return -1;
        }
    }
    return 0;
}

int
SDL_RenderFillRect(SDL_Renderer * renderer, const SDL_Rect * rect)
{
    SDL_Rect full_rect = { 0, 0, 0, 0 };

    CHECK_RENDERER_MAGIC(renderer, -1);

    /* If 'rect' == NULL, then outline the whole surface */
    if (!rect) {
        SDL_RenderGetViewport(renderer, &full_rect);
        full_rect.x = 0;
        full_rect.y = 0;
        rect = &full_rect;
    }
    return SDL_RenderFillRects(renderer, rect, 1);
}

int
SDL_RenderFillRects(SDL_Renderer * renderer,
                    const SDL_Rect * rects, int count)
{
    SDL_FRect *frects;
    int i;
    int status;

    CHECK_RENDERER_MAGIC(renderer, -1);

    if (!rects) {
        return SDL_SetError("SDL_RenderFillRects(): Passed NULL rects");
    }
    if (count < 1) {
        return 0;
    }

    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return 0;
    }

    frects = SDL_stack_alloc(SDL_FRect, count);
    if (!frects) {
        return SDL_OutOfMemory();
    }
    for (i = 0; i < count; ++i) {
        frects[i].x = rects[i].x * renderer->scale.x;
        frects[i].y = rects[i].y * renderer->scale.y;
        frects[i].w = rects[i].w * renderer->scale.x;
        frects[i].h = rects[i].h * renderer->scale.y;
    }

    status = renderer->RenderFillRects(renderer, frects, count);

    SDL_stack_free(frects);

    return status;
}

int
SDL_RenderCopy(SDL_Renderer * renderer, SDL_Texture * texture,
               const SDL_Rect * srcrect, const SDL_Rect * dstrect)
{
    SDL_Rect real_srcrect = { 0, 0, 0, 0 };
    SDL_Rect real_dstrect = { 0, 0, 0, 0 };
    SDL_FRect frect;

    CHECK_RENDERER_MAGIC(renderer, -1);
    CHECK_TEXTURE_MAGIC(texture, -1);

    if (renderer != texture->renderer) {
        return SDL_SetError("Texture was not created with this renderer");
    }

    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return 0;
    }

    real_srcrect.x = 0;
    real_srcrect.y = 0;
    real_srcrect.w = texture->w;
    real_srcrect.h = texture->h;
    if (srcrect) {
        if (!SDL_IntersectRect(srcrect, &real_srcrect, &real_srcrect)) {
            return 0;
        }
    }

    SDL_RenderGetViewport(renderer, &real_dstrect);
    real_dstrect.x = 0;
    real_dstrect.y = 0;
    if (dstrect) {
        if (!SDL_HasIntersection(dstrect, &real_dstrect)) {
            return 0;
        }
        real_dstrect = *dstrect;
    }

    if (texture->native) {
        texture = texture->native;
    }

    frect.x = real_dstrect.x * renderer->scale.x;
    frect.y = real_dstrect.y * renderer->scale.y;
    frect.w = real_dstrect.w * renderer->scale.x;
    frect.h = real_dstrect.h * renderer->scale.y;

    return renderer->RenderCopy(renderer, texture, &real_srcrect, &frect);
}


int
SDL_RenderCopyEx(SDL_Renderer * renderer, SDL_Texture * texture,
               const SDL_Rect * srcrect, const SDL_Rect * dstrect,
               const double angle, const SDL_Point *center, const SDL_RendererFlip flip)
{
    SDL_Rect real_srcrect = { 0, 0, 0, 0 };
    SDL_Rect real_dstrect = { 0, 0, 0, 0 };
    SDL_Point real_center;
    SDL_FRect frect;
    SDL_FPoint fcenter;

    if (flip == SDL_FLIP_NONE && (int)(angle/360) == angle/360) { /* fast path when we don't need rotation or flipping */
        return SDL_RenderCopy(renderer, texture, srcrect, dstrect);
    }

    CHECK_RENDERER_MAGIC(renderer, -1);
    CHECK_TEXTURE_MAGIC(texture, -1);

    if (renderer != texture->renderer) {
        return SDL_SetError("Texture was not created with this renderer");
    }
    if (!renderer->RenderCopyEx) {
        return SDL_SetError("Renderer does not support RenderCopyEx");
    }

    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return 0;
    }

    real_srcrect.x = 0;
    real_srcrect.y = 0;
    real_srcrect.w = texture->w;
    real_srcrect.h = texture->h;
    if (srcrect) {
        if (!SDL_IntersectRect(srcrect, &real_srcrect, &real_srcrect)) {
            return 0;
        }
    }

    /* We don't intersect the dstrect with the viewport as RenderCopy does because of potential rotation clipping issues... TODO: should we? */
    if (dstrect) {
        real_dstrect = *dstrect;
    } else {
        SDL_RenderGetViewport(renderer, &real_dstrect);
        real_dstrect.x = 0;
        real_dstrect.y = 0;
    }

    if (texture->native) {
        texture = texture->native;
    }

    if (center) {
        real_center = *center;
    } else {
        real_center.x = real_dstrect.w/2;
        real_center.y = real_dstrect.h/2;
    }

    frect.x = real_dstrect.x * renderer->scale.x;
    frect.y = real_dstrect.y * renderer->scale.y;
    frect.w = real_dstrect.w * renderer->scale.x;
    frect.h = real_dstrect.h * renderer->scale.y;

    fcenter.x = real_center.x * renderer->scale.x;
    fcenter.y = real_center.y * renderer->scale.y;

    return renderer->RenderCopyEx(renderer, texture, &real_srcrect, &frect, angle, &fcenter, flip);
}

int
SDL_RenderReadPixels(SDL_Renderer * renderer, const SDL_Rect * rect,
                     Uint32 format, void * pixels, int pitch)
{
    SDL_Rect real_rect;

    CHECK_RENDERER_MAGIC(renderer, -1);

    if (!renderer->RenderReadPixels) {
        return SDL_Unsupported();
    }

    if (!format) {
        format = SDL_GetWindowPixelFormat(renderer->window);
    }

    real_rect.x = renderer->viewport.x;
    real_rect.y = renderer->viewport.y;
    real_rect.w = renderer->viewport.w;
    real_rect.h = renderer->viewport.h;
    if (rect) {
        if (!SDL_IntersectRect(rect, &real_rect, &real_rect)) {
            return 0;
        }
        if (real_rect.y > rect->y) {
            pixels = (Uint8 *)pixels + pitch * (real_rect.y - rect->y);
        }
        if (real_rect.x > rect->x) {
            int bpp = SDL_BYTESPERPIXEL(format);
            pixels = (Uint8 *)pixels + bpp * (real_rect.x - rect->x);
        }
    }

    return renderer->RenderReadPixels(renderer, &real_rect,
                                      format, pixels, pitch);
}

void
SDL_RenderPresent(SDL_Renderer * renderer)
{
    CHECK_RENDERER_MAGIC(renderer, );

    /* Don't draw while we're hidden */
    if (renderer->hidden) {
        return;
    }
    renderer->RenderPresent(renderer);
}

void
SDL_DestroyTexture(SDL_Texture * texture)
{
    SDL_Renderer *renderer;

    CHECK_TEXTURE_MAGIC(texture, );

    renderer = texture->renderer;
    if (texture == renderer->target) {
        SDL_SetRenderTarget(renderer, NULL);
    }

    texture->magic = NULL;

    if (texture->next) {
        texture->next->prev = texture->prev;
    }
    if (texture->prev) {
        texture->prev->next = texture->next;
    } else {
        renderer->textures = texture->next;
    }

    if (texture->native) {
        SDL_DestroyTexture(texture->native);
    }
    if (texture->yuv) {
        SDL_SW_DestroyYUVTexture(texture->yuv);
    }
    SDL_free(texture->pixels);

    renderer->DestroyTexture(renderer, texture);
    SDL_free(texture);
}

void
SDL_DestroyRenderer(SDL_Renderer * renderer)
{
    CHECK_RENDERER_MAGIC(renderer, );

    SDL_DelEventWatch(SDL_RendererEventWatch, renderer);

    /* Free existing textures for this renderer */
    while (renderer->textures) {
        SDL_Texture *tex = renderer->textures; (void) tex;
        SDL_DestroyTexture(renderer->textures);
        SDL_assert(tex != renderer->textures);  /* satisfy static analysis. */
    }

    if (renderer->window) {
        SDL_SetWindowData(renderer->window, SDL_WINDOWRENDERDATA, NULL);
    }

    /* It's no longer magical... */
    renderer->magic = NULL;

    /* Free the target mutex */
    SDL_DestroyMutex(renderer->target_mutex);
    renderer->target_mutex = NULL;

    /* Free the renderer instance */
    renderer->DestroyRenderer(renderer);
}

int SDL_GL_BindTexture(SDL_Texture *texture, float *texw, float *texh)
{
    SDL_Renderer *renderer;

    CHECK_TEXTURE_MAGIC(texture, -1);
    renderer = texture->renderer;
    if (texture->native) {
        return SDL_GL_BindTexture(texture->native, texw, texh);
    } else if (renderer && renderer->GL_BindTexture) {
        return renderer->GL_BindTexture(renderer, texture, texw, texh);
    } else {
        return SDL_Unsupported();
    }
}

int SDL_GL_UnbindTexture(SDL_Texture *texture)
{
    SDL_Renderer *renderer;

    CHECK_TEXTURE_MAGIC(texture, -1);
    renderer = texture->renderer;
    if (texture->native) {
        return SDL_GL_UnbindTexture(texture->native);
    } else if (renderer && renderer->GL_UnbindTexture) {
        return renderer->GL_UnbindTexture(renderer, texture);
    }

    return SDL_Unsupported();
}

void *
SDL_RenderGetMetalLayer(SDL_Renderer * renderer)
{
    CHECK_RENDERER_MAGIC(renderer, NULL);

    if (renderer->GetMetalLayer) {
        return renderer->GetMetalLayer(renderer);
    }
    return NULL;
}

void *
SDL_RenderGetMetalCommandEncoder(SDL_Renderer * renderer)
{
    CHECK_RENDERER_MAGIC(renderer, NULL);

    if (renderer->GetMetalCommandEncoder) {
        return renderer->GetMetalCommandEncoder(renderer);
    }
    return NULL;
}

static SDL_BlendMode
SDL_GetShortBlendMode(SDL_BlendMode blendMode)
{
    if (blendMode == SDL_BLENDMODE_NONE_FULL) {
        return SDL_BLENDMODE_NONE;
    }
    if (blendMode == SDL_BLENDMODE_BLEND_FULL) {
        return SDL_BLENDMODE_BLEND;
    }
    if (blendMode == SDL_BLENDMODE_ADD_FULL) {
        return SDL_BLENDMODE_ADD;
    }
    if (blendMode == SDL_BLENDMODE_MOD_FULL) {
        return SDL_BLENDMODE_MOD;
    }
    return blendMode;
}

static SDL_BlendMode
SDL_GetLongBlendMode(SDL_BlendMode blendMode)
{
    if (blendMode == SDL_BLENDMODE_NONE) {
        return SDL_BLENDMODE_NONE_FULL;
    }
    if (blendMode == SDL_BLENDMODE_BLEND) {
        return SDL_BLENDMODE_BLEND_FULL;
    }
    if (blendMode == SDL_BLENDMODE_ADD) {
        return SDL_BLENDMODE_ADD_FULL;
    }
    if (blendMode == SDL_BLENDMODE_MOD) {
        return SDL_BLENDMODE_MOD_FULL;
    }
    return blendMode;
}

SDL_BlendMode
SDL_ComposeCustomBlendMode(SDL_BlendFactor srcColorFactor, SDL_BlendFactor dstColorFactor,
                           SDL_BlendOperation colorOperation,
                           SDL_BlendFactor srcAlphaFactor, SDL_BlendFactor dstAlphaFactor,
                           SDL_BlendOperation alphaOperation)
{
    SDL_BlendMode blendMode = SDL_COMPOSE_BLENDMODE(srcColorFactor, dstColorFactor, colorOperation,
                                                    srcAlphaFactor, dstAlphaFactor, alphaOperation);
    return SDL_GetShortBlendMode(blendMode);
}

SDL_BlendFactor
SDL_GetBlendModeSrcColorFactor(SDL_BlendMode blendMode)
{
    blendMode = SDL_GetLongBlendMode(blendMode);
    return (SDL_BlendFactor)(((Uint32)blendMode >> 4) & 0xF);
}

SDL_BlendFactor
SDL_GetBlendModeDstColorFactor(SDL_BlendMode blendMode)
{
    blendMode = SDL_GetLongBlendMode(blendMode);
    return (SDL_BlendFactor)(((Uint32)blendMode >> 8) & 0xF);
}

SDL_BlendOperation
SDL_GetBlendModeColorOperation(SDL_BlendMode blendMode)
{
    blendMode = SDL_GetLongBlendMode(blendMode);
    return (SDL_BlendOperation)(((Uint32)blendMode >> 0) & 0xF);
}

SDL_BlendFactor
SDL_GetBlendModeSrcAlphaFactor(SDL_BlendMode blendMode)
{
    blendMode = SDL_GetLongBlendMode(blendMode);
    return (SDL_BlendFactor)(((Uint32)blendMode >> 20) & 0xF);
}

SDL_BlendFactor
SDL_GetBlendModeDstAlphaFactor(SDL_BlendMode blendMode)
{
    blendMode = SDL_GetLongBlendMode(blendMode);
    return (SDL_BlendFactor)(((Uint32)blendMode >> 24) & 0xF);
}

SDL_BlendOperation
SDL_GetBlendModeAlphaOperation(SDL_BlendMode blendMode)
{
    blendMode = SDL_GetLongBlendMode(blendMode);
    return (SDL_BlendOperation)(((Uint32)blendMode >> 16) & 0xF);
}

/* vi: set ts=4 sw=4 expandtab: */

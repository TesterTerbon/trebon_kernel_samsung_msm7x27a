/*
 * Copyright (C) 2008 Maarten Maathuis.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __NOUVEAU_CRTC_H__
#define __NOUVEAU_CRTC_H__

struct nouveau_crtc {
	struct drm_crtc base;

	int index;

<<<<<<< HEAD
<<<<<<< HEAD
	struct drm_display_mode *mode;

	uint32_t dpms_saved_fp_control;
	uint32_t fp_users;
	int saturation;
=======
=======
>>>>>>> refs/remotes/origin/master
	uint32_t dpms_saved_fp_control;
	uint32_t fp_users;
	int saturation;
	int color_vibrance;
	int vibrant_hue;
<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
>>>>>>> refs/remotes/origin/master
	int sharpness;
	int last_dpms;

	int cursor_saved_x, cursor_saved_y;

	struct {
		int cpp;
		bool blanked;
		uint32_t offset;
		uint32_t tile_flags;
	} fb;

	struct {
		struct nouveau_bo *nvbo;
		bool visible;
		uint32_t offset;
		void (*set_offset)(struct nouveau_crtc *, uint32_t offset);
		void (*set_pos)(struct nouveau_crtc *, int x, int y);
		void (*hide)(struct nouveau_crtc *, bool update);
		void (*show)(struct nouveau_crtc *, bool update);
	} cursor;

	struct {
		struct nouveau_bo *nvbo;
		uint16_t r[256];
		uint16_t g[256];
		uint16_t b[256];
		int depth;
	} lut;

<<<<<<< HEAD
<<<<<<< HEAD
	int (*set_dither)(struct nouveau_crtc *crtc, bool on, bool update);
	int (*set_scale)(struct nouveau_crtc *crtc, int mode, bool update);
=======
	int (*set_dither)(struct nouveau_crtc *crtc, bool update);
	int (*set_scale)(struct nouveau_crtc *crtc, bool update);
	int (*set_color_vibrance)(struct nouveau_crtc *crtc, bool update);
>>>>>>> refs/remotes/origin/cm-10.0
=======
	int (*set_dither)(struct nouveau_crtc *crtc, bool update);
	int (*set_scale)(struct nouveau_crtc *crtc, bool update);
	int (*set_color_vibrance)(struct nouveau_crtc *crtc, bool update);
>>>>>>> refs/remotes/origin/master
};

static inline struct nouveau_crtc *nouveau_crtc(struct drm_crtc *crtc)
{
	return container_of(crtc, struct nouveau_crtc, base);
}

static inline struct drm_crtc *to_drm_crtc(struct nouveau_crtc *crtc)
{
	return &crtc->base;
}

<<<<<<< HEAD
int nv50_crtc_create(struct drm_device *dev, int index);
<<<<<<< HEAD
int nv50_cursor_init(struct nouveau_crtc *);
void nv50_cursor_fini(struct nouveau_crtc *);
=======
>>>>>>> refs/remotes/origin/cm-10.0
int nv50_crtc_cursor_set(struct drm_crtc *drm_crtc, struct drm_file *file_priv,
			 uint32_t buffer_handle, uint32_t width,
			 uint32_t height);
int nv50_crtc_cursor_move(struct drm_crtc *drm_crtc, int x, int y);

int nv04_cursor_init(struct nouveau_crtc *);
<<<<<<< HEAD
=======
int nv50_cursor_init(struct nouveau_crtc *);
>>>>>>> refs/remotes/origin/cm-10.0

struct nouveau_connector *
nouveau_crtc_connector_get(struct nouveau_crtc *crtc);
=======
int nv04_cursor_init(struct nouveau_crtc *);
>>>>>>> refs/remotes/origin/master

#endif /* __NOUVEAU_CRTC_H__ */

/**
 * File:   native_window_ogre.pp
 * Author: AWTK Develop Team
 * Brief:  native window ogre
 *
 * Copyright (c) 2024 - 2024  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2024-04-13 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "base/widget.h"
#include "lcd_ogre.inc"
#include "base/window_manager.h"
#include "native_window_ogre.hpp"

typedef struct _native_window_ogre_t {
  native_window_t native_window;

  canvas_t canvas;
  RenderWindow* render_win;
} native_window_ogre_t;

static native_window_t* s_shared_win = NULL;

#define NATIVE_WINDOW_OGRE(win) ((native_window_ogre_t*)(win))

static ret_t native_window_ogre_move(native_window_t* win, xy_t x, xy_t y) {
  return RET_OK;
}

static ret_t native_window_ogre_on_resized_timer(const timer_info_t* info) {
  widget_t* wm = window_manager();
  widget_set_need_relayout_children(wm);
  widget_invalidate_force(wm, NULL);

  log_debug("on_resized_idle\n");
  return RET_REMOVE;
}

static ret_t native_window_ogre_resize(native_window_t* win, wh_t w, wh_t h) {
  ret_t ret = RET_OK;
  widget_t* wm = window_manager();
  native_window_ogre_t* ogre = NATIVE_WINDOW_OGRE(win);
  event_t e = event_init(EVT_NATIVE_WINDOW_RESIZED, NULL);
  return_value_if_fail(ogre != NULL, RET_BAD_PARAMS);

  if (win->rect.w != w || win->rect.h != h) {
    win->rect.w = w;
    win->rect.h = h;

    ret = lcd_resize(ogre->canvas.lcd, w, h, 0);
    system_info_set_lcd_w(system_info(), w);
    system_info_set_lcd_h(system_info(), h);

    window_manager_dispatch_native_window_event(wm, &e, win);
    timer_add(native_window_ogre_on_resized_timer, win, 100);
  }

  return RET_OK;
}

static ret_t native_window_ogre_set_orientation(native_window_t* win,
                                               lcd_orientation_t old_orientation,
                                               lcd_orientation_t new_orientation) {
  return RET_OK;
}

static canvas_t* native_window_ogre_get_canvas(native_window_t* win) {
  native_window_ogre_t* ogre = NATIVE_WINDOW_OGRE(win);

  return &(ogre->canvas);
}

static ret_t native_window_ogre_get_info(native_window_t* win, native_window_info_t* info) {
  system_info_t* s_info = system_info();

  info->x = 0;
  info->y = 0;
  info->w = s_info->lcd_w;
  info->h = s_info->lcd_h;
  info->ratio = s_info->device_pixel_ratio;

  log_debug("ratio=%f %d %d\n", info->ratio, info->w, info->h);

  return RET_OK;
}

static ret_t native_window_ogre_swap_buffer(native_window_t* win) {
  return RET_OK;
}

static ret_t native_window_ogre_gl_make_current(native_window_t* win) {
  return RET_OK;
}

static const native_window_vtable_t s_native_window_vtable = {
    .type = "native_window_ogre",
    .move = native_window_ogre_move,
    .resize = native_window_ogre_resize,
    .get_info = native_window_ogre_get_info,
    .get_canvas = native_window_ogre_get_canvas,
    .swap_buffer = native_window_ogre_swap_buffer,
    .gl_make_current = native_window_ogre_gl_make_current,
    .set_orientation = native_window_ogre_set_orientation
  };
static ret_t native_window_ogre_set_prop(tk_object_t* obj, const char* name, const value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t native_window_ogre_get_prop(tk_object_t* obj, const char* name, value_t* v) {
  return RET_NOT_FOUND;
}

static ret_t native_window_ogre_on_destroy(tk_object_t* obj) {
  native_window_ogre_t* ogre = NATIVE_WINDOW_OGRE(obj);
  lcd_t* lcd = ogre->canvas.lcd;

  canvas_reset(&(ogre->canvas));
  lcd_destroy(lcd);

  return RET_OK;
}

static const object_vtable_t s_native_window_ogre_vtable = {
    .type = "native_window_ogre",
    .desc = "native_window_ogre",
    .size = sizeof(native_window_ogre_t),
    .on_destroy = native_window_ogre_on_destroy,
    .get_prop = native_window_ogre_get_prop,
    .set_prop = native_window_ogre_set_prop,
    };

static native_window_t* native_window_ogre_create_internal(RenderWindow* render_win) {
  int top = 0;
  int left = 0;
  unsigned int width = 0;
  unsigned int height = 0;
  float devicePixelRatio = 1;

  if (s_shared_win != NULL) {
    TK_OBJECT_REF(s_shared_win);
    return s_shared_win;
  }

  tk_object_t* obj = tk_object_create(&s_native_window_ogre_vtable);
  native_window_t* win = NATIVE_WINDOW(obj);
  native_window_ogre_t* ogre = NATIVE_WINDOW_OGRE(win);
  canvas_t* c = &(ogre->canvas);
  return_value_if_fail(ogre != NULL, NULL);

  win->shared = TRUE;
  win->vt = &s_native_window_vtable;
  ogre->render_win = render_win;
  render_win->getMetrics(width, height, left, top);
  devicePixelRatio = render_win->getViewPointToPixelScale();

  width = width / devicePixelRatio;
  height = height / devicePixelRatio;
  
  system_info_set_lcd_w(system_info(), width);
  system_info_set_lcd_h(system_info(), height);
  system_info_set_device_pixel_ratio(system_info(), devicePixelRatio);
  win->rect = rect_init(left, top, width, height);

  lcd_t* lcd = lcd_ogre_init(win);
  canvas_init(c, lcd, font_manager());
  
  return win;
}

native_window_t* native_window_ogre_create(widget_t* widget) {
  native_window_t* nw = s_shared_win;
  return_value_if_fail(nw != NULL, NULL);

  widget_set_prop_pointer(widget, WIDGET_PROP_NATIVE_WINDOW, nw);

  return nw;
}

ret_t native_window_ogre_init(RenderWindow* render_win) {
  window_manager_t* wm = WINDOW_MANAGER(window_manager());

  return_value_if_fail(render_win != NULL, RET_BAD_PARAMS);

  wm->create_native_window = native_window_ogre_create;
  s_shared_win = native_window_ogre_create_internal(render_win);
  return_value_if_fail(s_shared_win != NULL, RET_OOM);

  return RET_OK;
}

ret_t native_window_ogre_deinit(void) {
  return RET_OK;
}

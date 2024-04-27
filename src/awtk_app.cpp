/**
 * File:   awtk_ogre_application.cpp
 * Author: AWTK Develop Team
 * Brief:  awtk_ogre_application
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

#include "ogre_app.hpp"
#include "awtk_app.hpp"

BEGIN_C_DECLS
#include "../res/assets_all.inc"
END_C_DECLS

#ifndef APP_DEFAULT_FONT
#define APP_DEFAULT_FONT "default"
#endif /*APP_DEFAULT_FONT*/

static ret_t on_wheel_event(void* ctx, event_t* e) {
  widget_t* win = WIDGET(e->target);
  wheel_event_t* evt = (wheel_event_t*)e;
  AwtkApp* app = (AwtkApp*)ctx;
  CameraHelper& camera_helper = app->getApp()->getCameraHelper();

  if (win->target != NULL) {
    return RET_OK;
  }

  if (evt->dy > 0) {
    camera_helper.increaseDistance(-1);
  } else {
    camera_helper.increaseDistance(1);
  }

  return RET_OK;
}

static ret_t on_camera_func_clicked(void* ctx, event_t* e) {
  widget_t* widget = WIDGET(e->target);
  AwtkApp* app = (AwtkApp*)ctx;
  const char* name = widget->name;
  CameraHelper& camera_helper = app->getApp()->getCameraHelper();

  if (strstr(name, "left")) {
    camera_helper.increaseAlpha(0.1);
  } else if (strstr(name, "right")) {
    camera_helper.increaseAlpha(-0.1);
  } else if (strstr(name, "up")) {
    camera_helper.increaseHeight(0.1);
  } else if (strstr(name, "down")) {
    camera_helper.increaseHeight(-0.1);
  } else if (strstr(name, "zoom_in")) {
    camera_helper.increaseDistance(-1);
  } else if (strstr(name, "zoom_out")) {
    camera_helper.increaseDistance(1);
  } else if (strstr(name, "zoom_0")) {
    camera_helper.reset();
  }

  return RET_OK;
}

static ret_t awtk_hook_camera_buttons(AwtkApp* app, widget_t* win) {  
  widget_on(win, EVT_WHEEL, on_wheel_event, app);

  widget_child_on(win, "camera_up", EVT_CLICK, on_camera_func_clicked, app);
  widget_child_on(win, "camera_down", EVT_CLICK, on_camera_func_clicked, app);
  widget_child_on(win, "camera_left", EVT_CLICK, on_camera_func_clicked, app);
  widget_child_on(win, "camera_right", EVT_CLICK, on_camera_func_clicked, app);
  widget_child_on(win, "camera_zoom_out", EVT_CLICK, on_camera_func_clicked, app);
  widget_child_on(win, "camera_zoom_0", EVT_CLICK, on_camera_func_clicked, app);
  widget_child_on(win, "camera_zoom_in", EVT_CLICK, on_camera_func_clicked, app);

  return RET_OK;
}

AwtkApp::AwtkApp(OgreApp* app) : m_app(app) {
}

AwtkApp::~AwtkApp() {
}

ret_t AwtkApp::hookCameraButtons(widget_t* win) {
  return awtk_hook_camera_buttons(this, win);
}

ret_t AwtkApp::init(const char* app_root) {
  const char* app_name = "";
  return_value_if_fail(tk_pre_init() == RET_OK, RET_FAIL);
  ENSURE(system_info_init(APP_DESKTOP, app_name, app_root) == RET_OK);
  return_value_if_fail(tk_init_internal() == RET_OK, RET_FAIL);

  m_app->initApp();
  assets_init();
  tk_ext_widgets_init();
  main_loop_ogre_init(m_app);
  system_info_set_default_font(system_info(), APP_DEFAULT_FONT);

  m_app->createScene();
  this->createUI();

  return RET_OK;
}

ret_t AwtkApp::run(void) {
  tk_run();
  tk_exit();

  return RET_OK;
}

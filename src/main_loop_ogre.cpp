/**
 * File:   main_loop_ogre.cpp
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

#include "main_loop/main_loop_simple.h"
#include "main_loop_ogre.hpp"
#include "native_window_ogre.hpp"

static ret_t main_loop_ogre_step(main_loop_t* l) {
  ApplicationContext* app = NULL;
  main_loop_simple_t* loop = (main_loop_simple_t*)l;
  return_value_if_fail(loop != NULL, RET_BAD_PARAMS);
  app = (ApplicationContext*)(loop->user1);
  return_value_if_fail(app != NULL, RET_BAD_PARAMS);

  Root* root = app->getRoot();
  event_source_manager_dispatch(loop->event_source_manager);
  window_manager_check_and_layout(loop->base.wm);

  if (root->endRenderingQueued()) {
    main_loop_quit((main_loop_t*)loop);
  } else {
    root->renderOneFrame();
    sleep_ms(16);
  }

  return RET_OK;
}

static ret_t main_loop_ogre_destroy(main_loop_t* l) {
  main_loop_simple_t* loop = (main_loop_simple_t*)l;
  main_loop_simple_reset(loop);
  native_window_ogre_deinit();

  return RET_OK;
}

main_loop_t* main_loop_ogre_init(ApplicationContext* app) {
  int top = 0;
  int left = 0;
  unsigned int width = 0;
  unsigned int height = 0;
  main_loop_simple_t* loop = NULL;
  RenderWindow* render_win = NULL;
  return_value_if_fail(app != NULL, NULL);
  render_win = app->getRenderWindow();
  return_value_if_fail(render_win != nullptr, NULL);
  return_value_if_fail(native_window_ogre_init(render_win) == RET_OK, NULL);

  render_win->getMetrics(width, height, left, top);
  loop = main_loop_simple_init(width, height, NULL, NULL);
  return_value_if_fail(loop != NULL, NULL);

  loop->user1 = app;
  loop->base.destroy = main_loop_ogre_destroy;
  loop->base.step = main_loop_ogre_step;

  return (main_loop_t*)loop;
}

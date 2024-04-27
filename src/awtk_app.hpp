/**
 * File:   awtk_ogre_application.hpp
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
#ifndef AWTK_OGRE_APPLICATION_H
#define AWTK_OGRE_APPLICATION_H

#include "awtk.h"
#include "main_loop_ogre.hpp"

class AwtkApp {
 public:
  AwtkApp(OgreApp* app);

  virtual ~AwtkApp();

  ret_t init(const char* app_root); 
  ret_t run(void);

  OgreApp* getApp(void) {
    return m_app;
  }
protected:
  virtual ret_t createUI(void) = 0; 
  ret_t hookCameraButtons(widget_t* win);
  
 private:
  OgreApp* m_app;
};
#endif  // AWTK_OGRE_APPLICATION_H

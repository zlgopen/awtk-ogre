/**
 * File:   ogre_app.hpp
 * Author: AWTK Develop Team
 * Brief:  ogre_app
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
#ifndef OGRE_APP_HPP
#define OGRE_APP_HPP

#include "awtk.h"
#include "light_helper.hpp"
#include "camera_helper.hpp"
#include "scene_manager_helper.hpp"

class OgreApp : public ApplicationContext, public InputListener, public RenderTargetListener {
 public:
  OgreApp(const char* app_name);

  virtual ~OgreApp();

  virtual void createScene() = 0;

 public:
  CameraHelper& getCameraHelper() {
    return mCameraHelper;
  }

 protected:
  void setup() override;

  bool mouseMoved(const MouseMotionEvent& evt) override;
  bool mouseWheelRolled(const MouseWheelEvent& evt) override;
  bool mousePressed(const MouseButtonEvent& evt) override;
  bool mouseReleased(const MouseButtonEvent& evt) override;
  bool dispatchKeyEvent(const KeyboardEvent& evt, int type);

  bool keyPressed(const KeyboardEvent& evt) override;

  bool keyReleased(const KeyboardEvent& evt) override;

  void frameRendered(const Ogre::FrameEvent& evt) override;
  bool touchMoved(const TouchFingerEvent& evt) override;
  bool touchPressed(const TouchFingerEvent& evt) override;
  bool touchReleased(const TouchFingerEvent& evt) override;
  bool textInput(const TextInputEvent& evt) override;
  bool axisMoved(const AxisEvent& evt) override;
  bool buttonPressed(const ButtonEvent& evt) override;
  bool buttonReleased(const ButtonEvent& evt) override;

  void windowResized(Ogre::RenderWindow* rw) override;
  bool frameRenderingQueued(const FrameEvent& evt) override;
  bool createAxis(float length, const Vector3& position = Vector3::ZERO);
  SceneNode* createLocalAxes(SceneManager* sceneMgr, SceneNode* parent, const Vector3& size);

 protected:
  int mMousePressX;
  int mMousePressY;
  int mMouseLastX;
  int mMouseLastY;
  bool mMouseIsPressed;

  SceneManager* mSceneMgr;
  CameraHelper mCameraHelper;
  LightHelper mLightHelper;
  SceneManagerHelper mSceneManagerHelper;
};

#endif  // OGRE_APP_HPP

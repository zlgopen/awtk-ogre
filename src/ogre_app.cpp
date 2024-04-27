/**
 * File:   ogre_app.cpp
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

#include <string>
#include "ogre_app.hpp"

static ret_t native_window_on_resized_timer(const timer_info_t* info);

OgreApp::OgreApp(const char* app_name, int w, int h)
    : ApplicationContext(std::string(app_name)),
      mMouseIsPressed(false),
      mMousePressX(0),
      mMousePressY(0),
      mMouseLastX(0),
      mMouseLastY(0),
      mSceneMgr(nullptr),
      mCameraHelper(),
      mLightHelper(),
      mWidth(w),
      mHeight(h) {
}

OgreApp::~OgreApp() {
  this->closeApp();
}

void OgreApp::setup() {
  ApplicationContext::setup();
  addInputListener(this);

  Root* root = getRoot();
  mSceneMgr = root->createSceneManager();

  // register our scene with the RTSS
  RTShader::ShaderGenerator* shadergen = RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(mSceneMgr);

  mSceneManagerHelper.init(mSceneMgr);
  mCameraHelper.init(mSceneMgr, getRenderWindow(), Vector3(0, -1, 0), Vector3(0, 0, 0));
  mLightHelper.init(mSceneMgr, 3000, Vector3(0.6, 0.6, 0.6));

  root->addFrameListener(this);
}

bool OgreApp::mouseMoved(const MouseMotionEvent& evt) {
  pointer_event_t event;
  widget_t* widget = window_manager();
  float device_pixel_ratio = system_info()->device_pixel_ratio;

  pointer_event_init(&event, EVT_POINTER_MOVE, widget, evt.x / device_pixel_ratio,
                     evt.y / device_pixel_ratio);

  event.button = 0;
  event.pressed = this->mMouseIsPressed;
  event.e.native_window_handle = tk_pointer_from_int(evt.windowID);

  window_manager_dispatch_input_event(widget, (event_t*)&event);
  return true;
}

bool OgreApp::mouseWheelRolled(const MouseWheelEvent& evt) {
  wheel_event_t event;
  widget_t* widget = window_manager();
  event_t* e = wheel_event_init(&event, EVT_WHEEL, widget, evt.y);
  window_manager_dispatch_input_event(widget, e);

  return false;
}

bool OgreApp::mousePressed(const MouseButtonEvent& evt) {
  this->mMouseIsPressed = true;

  pointer_event_t event;
  widget_t* widget = window_manager();
  float device_pixel_ratio = system_info()->device_pixel_ratio;

  pointer_event_init(&event, EVT_POINTER_DOWN, widget, evt.x / device_pixel_ratio,
                     evt.y / device_pixel_ratio);

  event.button = evt.button;
  event.e.native_window_handle = NULL;

  window_manager_dispatch_input_event(widget, (event_t*)&event);

  return true;
}

bool OgreApp::mouseReleased(const MouseButtonEvent& evt) {
  this->mMouseIsPressed = false;

  pointer_event_t event;
  widget_t* widget = window_manager();
  float device_pixel_ratio = system_info()->device_pixel_ratio;

  pointer_event_init(&event, EVT_POINTER_UP, widget, evt.x / device_pixel_ratio,
                     evt.y / device_pixel_ratio);

  event.button = evt.button;
  event.e.native_window_handle = NULL;

  window_manager_dispatch_input_event(widget, (event_t*)&event);

  return true;
}

bool OgreApp::dispatchKeyEvent(const KeyboardEvent& evt, int type) {
  key_event_t event;
  widget_t* widget = window_manager();
  key_event_init(&event, type, widget, evt.keysym.sym);
  //event.capslock = (evt.keysym.mod & OgreBites::KMOD_CAPS) != 0;
  //event.numlock = (evt.keysym.mod & OgreBites::KMOD_NUM) != 0;
  window_manager_dispatch_input_event(widget, (event_t*)&event);

  return true;
}

bool OgreApp::keyPressed(const KeyboardEvent& evt) {
  if (evt.keysym.sym == SDLK_ESCAPE) {
    getRoot()->queueEndRendering();
  }

  return this->dispatchKeyEvent(evt, EVT_KEY_DOWN);
}

bool OgreApp::keyReleased(const KeyboardEvent& evt) {
  return this->dispatchKeyEvent(evt, EVT_KEY_UP);
}

void OgreApp::frameRendered(const Ogre::FrameEvent& evt) {
}
bool OgreApp::touchMoved(const TouchFingerEvent& evt) {
  return false;
}
bool OgreApp::touchPressed(const TouchFingerEvent& evt) {
  return false;
}
bool OgreApp::touchReleased(const TouchFingerEvent& evt) {
  return false;
}
bool OgreApp::textInput(const TextInputEvent& evt) {
  im_commit_event_t event;

  im_commit_event_init(&event, evt.chars, FALSE);

  input_method_dispatch_to_widget(input_method(), &(event.e));

  return false;
}

bool OgreApp::axisMoved(const AxisEvent& evt) {
  return false;
}
bool OgreApp::buttonPressed(const ButtonEvent& evt) {
  return false;
}
bool OgreApp::buttonReleased(const ButtonEvent& evt) {
  return false;
}

void OgreApp::windowResized(Ogre::RenderWindow* rw) {
  int top = 0;
  int left = 0;
  unsigned int width = 0;
  unsigned int height = 0;
  float device_pixel_ratio = 1.0;
  event_t e = event_init(EVT_NATIVE_WINDOW_RESIZED, NULL);
  widget_t* wm = window_manager();
  rw->getMetrics(width, height, top, left);
  device_pixel_ratio = rw->getViewPointToPixelScale();

  width = width / device_pixel_ratio;
  height = height / device_pixel_ratio;
  system_info_set_lcd_w(system_info(), width);
  system_info_set_lcd_h(system_info(), height);
  window_manager_dispatch_native_window_event(wm, &e, rw);
  timer_add(native_window_on_resized_timer, wm, 100);
}

bool OgreApp::frameRenderingQueued(const FrameEvent& evt) {
  widget_t* wm = window_manager();
  widget_invalidate_force(widget_get_child(wm, 0), NULL);
  window_manager_paint(wm);

  return true;
}

// 创建表示局部坐标系的辅助对象
SceneNode* OgreApp::createLocalAxes(SceneManager* sceneMgr, SceneNode* parent,
                                    const Vector3& size) {
  ManualObject* manualObject = sceneMgr->createManualObject("LocalAxes");
  // 创建材质
  MaterialPtr redMaterial = MaterialManager::getSingleton().create(
      "RedMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  redMaterial->setDiffuse(ColourValue::Red);
  redMaterial->setSelfIllumination(ColourValue::Red);  // 可选，使轴更加明亮

  MaterialPtr greenMaterial = MaterialManager::getSingleton().create(
      "GreenMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  greenMaterial->setDiffuse(ColourValue::Green);
  greenMaterial->setSelfIllumination(ColourValue::Green);

  MaterialPtr blueMaterial = MaterialManager::getSingleton().create(
      "BlueMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  blueMaterial->setDiffuse(ColourValue::Blue);
  blueMaterial->setSelfIllumination(ColourValue::Blue);

  // X 轴
  manualObject->begin("RedMaterial", RenderOperation::OT_LINE_LIST);
  manualObject->position(Vector3::ZERO);
  manualObject->position(size.x, 0, 0);
  manualObject->end();

  // Y 轴
  manualObject->begin("GreenMaterial", RenderOperation::OT_LINE_LIST);
  manualObject->position(Vector3::ZERO);
  manualObject->position(0, size.y, 0);
  manualObject->end();

  // Z 轴
  manualObject->begin("BlueMaterial", RenderOperation::OT_LINE_LIST);
  manualObject->position(Vector3::ZERO);
  manualObject->position(0, 0, size.z);
  manualObject->end();

  SceneNode* axesNode = parent->createChildSceneNode("LocalAxesNode");
  axesNode->attachObject(manualObject);
  return axesNode;
}

static ret_t native_window_on_resized_timer(const timer_info_t* info) {
  widget_t* wm = WIDGET(info->ctx);
  widget_set_need_relayout_children(wm);
  widget_invalidate_force(wm, NULL);

  log_debug("on_resized_timer\n");
  return RET_REMOVE;
}

NativeWindowPair OgreApp::createWindow(const Ogre::String& name, uint32_t w, uint32_t h,
                                       Ogre::NameValuePairList miscParams) {
  miscParams["FSAA"] = 4;                                       
  return ApplicationContextSDL::createWindow(name, mWidth, mHeight, miscParams);
}


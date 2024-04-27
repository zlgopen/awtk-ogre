#ifndef CAMERA_HELPER_HPP
#define CAMERA_HELPER_HPP

#include "ogre_types_def.hpp"

class CameraHelper {
 public:
  CameraHelper() {
  }

  void init(SceneManager* sceneMgr, RenderWindow* renderWindow, const Vector3& direction,
            const Vector3& lookAt) {
    mHeight = 0;
    mAlpha = 0;
    mDistance = 10;
    mSceneMgr = sceneMgr;

    SceneNode* cameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    cameraNode->lookAt(lookAt, Node::TS_WORLD);
    cameraNode->setDirection(direction, Node::TS_WORLD);

    Camera* cam = mSceneMgr->createCamera("MainCamera");
    cam->setNearClipDistance(1);  // specific to this sample
    cam->setAutoAspectRatio(true);
    cameraNode->attachObject(cam);

    renderWindow->addViewport(cam);
    mCameraNode = cameraNode;

    this->updateCamera();
  }

  void updateCamera() {
    float z = mHeight;
    float x = mDistance * sin(mAlpha);
    float y = mDistance * cos(mAlpha);
    auto direction = Vector3(0, 0, -1);

    mCameraNode->resetOrientation();
    mCameraNode->setPosition(x, y, z);
    mCameraNode->roll(Radian(-mAlpha));
    mCameraNode->setDirection(direction, Node::TS_WORLD);
    mCameraNode->lookAt(Vector3(0, 0, z), Node::TS_WORLD);
  }

  void increaseHeight(float delta) {
    mHeight += delta;
    updateCamera();
  }

  void increaseAlpha(float delta) {
    mAlpha += delta;
    updateCamera();
  }

  void increaseDistance(float delta) {
    if ((mDistance + delta) < 2) {
      return;
    }
    mDistance += delta;
    updateCamera();
  }
  
  void reset(void) {
    this->mAlpha = 0;
    this->mHeight = 0;
    this->mDistance = 10;
    this->updateCamera();
  }

  bool handleMouseMoveEvent(int dx, int dy) {
    this->increaseHeight(-dy * 0.01);
    this->increaseAlpha(-dx * 0.01);

    return true;
  }

  bool handleMouseWheelEvent(int delta) {
    this->increaseDistance(delta);
    return true;
  }

  bool handleKeyEvent(const KeyboardEvent& evt) {
    if (evt.keysym.sym == SDLK_LEFT) {
      this->increaseAlpha(-0.1);
      return true;
    } else if (evt.keysym.sym == SDLK_RIGHT) {
      this->increaseAlpha(0.1);
      return true;
    } else if (evt.keysym.sym == SDLK_UP) {
      this->increaseHeight(0.1);
      return true;
    } else if (evt.keysym.sym == SDLK_DOWN) {
      this->increaseHeight(-0.1);
      return true;
    } else if (evt.keysym.sym == SDLK_PAGEUP) {
      this->increaseDistance(5);
      return true;
    } else if (evt.keysym.sym == SDLK_PAGEDOWN) {
      this->increaseDistance(-5);
      return true;
    } else if (evt.keysym.sym == SDLK_HOME) {
      this->reset();  

      return true;
    }

    return false;
  }

 private:
  SceneManager* mSceneMgr;
  SceneNode* mCameraNode;
  float mHeight; //高度
  float mAlpha; //角度
  float mDistance; //距离
};

#endif  // CAMERA_HELPER_HPP

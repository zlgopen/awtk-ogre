#ifndef LIGHT_HELPER_HPP
#define LIGHT_HELPER_HPP

#include "ogre_types_def.hpp"

class LightHelper {
 public:
  LightHelper() {
  }

  void init(SceneManager* sceneMgr, int32_t distance, const Vector3& color) {
    this->createLight(sceneMgr, "1", Vector3f(distance, 0, 0), color);
    this->createLight(sceneMgr, "2", Vector3f(-distance, 0, 0), color);
    
    this->createLight(sceneMgr, "3", Vector3f(0, distance, 0), color);
    this->createLight(sceneMgr, "4", Vector3f(0, -distance, 0), color);

    this->createLight(sceneMgr, "5", Vector3f(0, 0, distance), color);
    this->createLight(sceneMgr, "6", Vector3f(0, 0, -distance), color);
  }

  void createLight(SceneManager* sceneMgr, const char* name, const Vector3f& position,
                   const Vector3f& color) {
    Light* light = sceneMgr->createLight(name);
    SceneNode* lightNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);

    lightNode->setPosition(position.x, position.y, position.z);
    light->setDiffuseColour(color.x, color.y, color.z);
    light->setSpecularColour(color.x, color.y, color.z);
  }
};

#endif // LIGHT_HELPER_HPP
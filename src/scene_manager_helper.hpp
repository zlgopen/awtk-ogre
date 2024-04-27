#ifndef SCENE_MANAGER_HELPER_HPP
#define SCENE_MANAGER_HELPER_HPP

#include "ogre_types_def.hpp"

class SceneManagerHelper {
 public:
  SceneManagerHelper() {
  }

  void init(SceneManager* sceneMgr) {
    mSceneMgr = sceneMgr;
  }

  bool createAxis(float length, const Vector3& position = Vector3::ZERO) {
    SceneManager* sceneMgr = mSceneMgr;
    ManualObject* axis = sceneMgr->createManualObject("Axis");

    axis->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
    drawLine(axis, Vector3(-length, 0, 0), Vector3(length, 0, 0), ColourValue(1, 0, 0));
    drawLine(axis, Vector3(0, -length, 0), Vector3(0, length, 0), ColourValue(0, 1, 0));
    drawLine(axis, Vector3(0, 0, -length), Vector3(0, 0, length), ColourValue(0, 0, 1));
    axis->end();

    SceneNode* axisNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
    axisNode->attachObject(axis);

    axisNode->setPosition(position);
    axisNode->setDirection(Vector3::UNIT_Z, Node::TS_WORLD);

    return true;
  }

  SceneNode*  createEntity(const char* mesh, const char* name, const Vector3& position,
                    const Vector3& scale) {
    SceneManager* sceneMgr = mSceneMgr;
    Entity* ogreEntity = sceneMgr->createEntity(mesh);
    SceneNode* ogreNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name, position);
    ogreNode->setScale(scale);
    ogreNode->attachObject(ogreEntity);

    return ogreNode;
  }

  void drawLine(ManualObject* obj, const Vector3& start, const Vector3& end,
                const ColourValue& color) {
    obj->position(start);
    obj->colour(color);
    obj->position(end);
    obj->colour(color);
  }

 private:
  SceneManager* mSceneMgr;
};

#endif  // SCENE_MANAGER_HELPER_HPP
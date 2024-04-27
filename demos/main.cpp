
#include "awtk_ogre_app.hpp"

static ret_t on_dialog_clicked(void* ctx, event_t* e) {
  dialog_info("Hello", "Hello Awtk Ogre");
  return RET_OK;
}

class DemoApp : public AwtkOgreApp {
 public:
  DemoApp(const char* app_name) : AwtkOgreApp(app_name) {
  }

  ret_t createUI(void) override {
    widget_t* win = window_open("main");

    this->hookCameraButtons(win);
    widget_child_on(win, "dialog", EVT_CLICK, on_dialog_clicked, NULL);

    return RET_OK;
  }

  void createScene() override {
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Ogre::Entity* ent = mSceneMgr->createEntity("test.mesh");
    Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject(ent);
    node->setPosition(0, 0, 0);
  }
};


int main(int argc, char** argv) {
  DemoApp app("AwtkOgreApp");

  app.init(NULL);
  app.run();

  return 0;
}

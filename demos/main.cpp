
#include "awtk_ogre_app.hpp"

static ret_t on_dialog_clicked(void* ctx, event_t* e) {
  return dialog_info("awtk", "hello awtk ogre demo.");
}

static ret_t on_window_clicked(void* ctx, event_t* e) {
  file_chooser_t* chooser = file_chooser_create();
  return_value_if_fail(chooser != NULL, RET_OOM);
  file_chooser_set_init_dir(chooser, "./");
  return file_chooser_choose_file_for_open(chooser);
}

class DemoApp : public AwtkOgreApp {
 public:
  DemoApp(const char* app_name, int w, int h) : AwtkOgreApp(app_name, w, h) {
  }

  ret_t createUI(void) override {
    widget_t* win = window_open("main");

    this->hookCameraButtons(win);
    widget_child_on(win, "dialog", EVT_CLICK, on_dialog_clicked, NULL);
    widget_child_on(win, "window", EVT_CLICK, on_window_clicked, NULL);

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
  DemoApp app("AwtkOgreApp", 800, 600);

  app.init(NULL);
  app.run();

  return 0;
}

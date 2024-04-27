#ifndef AWTK_OGRE_APP_HPP
#define AWTK_OGRE_APP_HPP

#include "awtk_app.hpp"
#include "ogre_app.hpp"

class AwtkOgreApp : public AwtkApp, public OgreApp {
 public:
  AwtkOgreApp(const char* app_name, int w, int h) : AwtkApp(this), OgreApp(app_name, w, h) {
  }

  virtual ~AwtkOgreApp() {
    
  }
};

#endif/*AWTK_OGRE_APP_HPP*/

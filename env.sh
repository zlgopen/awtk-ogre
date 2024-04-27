export awtk_DIR=~/work/awtk-root/awtk
export awtk_modbus_DIR=$awtk_DIR/../awtk-modbus
export awtk_mvvm_DIR=$awtk_DIR/../awtk-mvvm

export AWTK_OGRE_ROOT=$PWD
export awtk_ogre_DIR=$PWD
export assimp_DIR=$AWTK_OGRE_ROOT/lib/cmake/assimp-5.3
export CMAKE_PREFIX_PATH=$AWTK_OGRE_ROOT/lib/cmake/assimp-5.3:$AWTK_OGRE_ROOT/CMake

if [[ "$(uname)" == "MINGW"* ]]; then
  export CONFIG='--config Debug'
elif [[ "$(uname)" == "Windows"* ]]; then
  export CONFIG='--config Debug'
else
  export CONFIG=
fi


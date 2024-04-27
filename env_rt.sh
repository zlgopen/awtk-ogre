export OGRE_CONFIG_DIR=$PWD
export PATH=$PATH:$PWD/lib:$PWD/bin
export LD_LIBRARY_PATH=$PWD/lib:$PWD/bin
export DYLD_LIBRARY_PATH=$PWD/lib:$PWD/bin

if [[ "$(uname)" == "MINGW"* ]]; then
  export OGRE_PLUGIN_DIR=$PWD/bin
elif [[ "$(uname)" == "Windows"* ]]; then
  export OGRE_PLUGIN_DIR=$PWD/bin
else
  export OGRE_PLUGIN_DIR=$PWD/lib/OGRE
fi

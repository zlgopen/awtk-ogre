cmake -S assimp -B build_assimp 

cmake --build build_assimp

echo cmake --install build_assimp --prefix .. $CONFIG
cmake --install build_assimp --prefix .. $CONFIG


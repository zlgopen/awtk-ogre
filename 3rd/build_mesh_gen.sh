cmake -S mesh_gen -B build_mesh_gen 

cmake --build build_mesh_gen 
echo cmake --install build_mesh_gen --prefix .. $CONFIG
cmake --install build_mesh_gen --prefix .. $CONFIG


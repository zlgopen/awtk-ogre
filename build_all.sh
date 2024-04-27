source env.sh
source env_rt.sh
source clean.sh

cd 3rd
./build_assimp.sh	
./build_ogre.sh
./build_mesh_gen.sh
cd ..

./build.sh


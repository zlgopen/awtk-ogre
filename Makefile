all:
	cmake --build build --config Debug
	cmake --install build --config Debug --prefix .

clean:
	cd build && make clean


cmake -H. -Bcmake-build-debug
cd cmake-build-debug
cmake --build . --config Debug
ctest
cd ..

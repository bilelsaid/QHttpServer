rm -rf build
mkdir -p build
cd build
cmake -DCMAKE_PREFIX_PATH=~/Qt/6.8.1/gcc_64/lib/cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)

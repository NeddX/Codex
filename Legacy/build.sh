#!/bin/bash
# Auto-generated by ccpinit

if [ -z "$CC" ]; then
	export CC=$(which clang)
	export CXX=$(which clang++)
fi

kernel=$(uname -s | tr '[:upper:]' '[:lower:]')
arch=$(uname -m | tr '[:upper:]' '[:lower:]')
platform="$kernel-$arch"
build_conf="Debug"
build_dir="$platform-$build_conf"
build_dir=$(echo "${build_dir}" | tr '[:upper:]' '[:lower:]')
cmake_build_generator="Unix Makefiles"

if [ -z "$1" ]; then
    build_conf="Debug"
elif [[ "$1" == "clean" ]]; then
    if [ -d "builds/$build_dir" ]; then
        cmake --build -B "builds/$build_dir" --target clean --parallel
    fi
elif [[ "$1" == "vs2022" ]]; then
	mkdir -p "builds/vs2022"
    cmake_build_generator="Visual Studio 17"
	cmake ./ -G "$cmake_build_generator" -B "builds/vs2022"
	cmake --build "builds/vs2022" --parallel
	exit
else
    build_conf="$1"
	build_dir="$platform-$build_conf"
	build_dir="${build_dir,,}"
fi

clear
echo ":: Current working directory: $PWD"
echo ":: Build started"
echo ":: OS: $kernel"
echo ":: Architecture: $arch"
echo ":: Configuration: $build_conf"
echo ":: Build directory: $build_dir"
echo ":: C Compiler: $CC"
echo ":: CXX Compiler: $CXX"
echo ""

if [ ! -d "builds" ]; then
    echo ":: Configuring CMake..."
    mkdir -p "builds/$build_dir"
elif [ ! -d "builds/$build_dir" ]; then
    echo ":: Configuring CMake..."
    mkdir -p "builds/$build_dir"
fi

cmake -G "$cmake_build_generator" -DCMAKE_BUILD_TYPE="$build_conf" -B "builds/$build_dir"

echo ":: Building..."
cmake --build "builds/$build_dir" --parallel
cp "builds/$build_dir/compile_commands.json" ./

if [ ! -d "./bin" ]; then
    mkdir -p "./bin"
fi

# Export few envrionment variables
echo ":: Exported Codex development envrionment variables." 
export CODEX_DEV="yes"
export CODEX_BUILD_GENERATOR="$cmake_build_generator"
export CODEX_BUILD_DIR="$PWD"/builds/$build_dir
export CODEX_ROOT="$PWD"
export CODEX_CONFIG="$build_dir"

echo ":: Build finished."

clear
echo ":: Building 'Codex Template Project' (Debug)..."

if [ -z "$CODEX_DEV" ]; then
    echo ":: Codex development envrionment variables are not exported."
    echo ":: Run 'build.sh' using source or the dot space operator if on bash."
    echo ":: E.g.: source build.sh"
    exit -1
fi

#dependencies=("libCodexEngine-static.a" "libMGL.a" "libfmtd.a")
dependencies=()

# Get all the static libraries.
while IFS= read -r -d '' f; do
    dependencies+=("$f")
done < <(find "$CODEX_BUILD_DIR/Codex" -type f \( -name "*.a" -o -name "*.lib" \) -print0)

if [ ! -d "lib" ]; then
    mkdir lib
fi
if [ ! -d "builds" ]; then
    mkdir -p builds/"$CODEX_CONFIG"
fi
if [ ! -d "builds/$CODEX_CONFIG" ]; then
    mkdir builds/"$CODEX_CONFIG"
fi

echo ""
echo ":: Copying neccessary dependencies..."
for f in "${dependencies[@]}"; do
    fp="$CODEX_BUILD_DIR/Codex/$f"
    cp "$fp" ./
    echo ":: Copied $fp"
done

echo ""
echo ":: Running CMake"
echo ""
cmake ./ -G "$CODEX_CMAKE_GENERATOR" -B "builds/$CODEX_CONFIG"
if [ ! $? -eq 0 ]; then
    echo ":: CMake build generation failed. Build aborted."
    exit -2
fi

cmake --build "builds/$CODEX_CONFIG" --parallel
if [ ! $? -eq 0 ]; then
    echo ":: CMake compilation failed. Build aborted."
    exit -3
fi

echo ""
echo ":: Build finished"

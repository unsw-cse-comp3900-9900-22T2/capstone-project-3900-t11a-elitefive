cmake -B /tmp/build -S . "-DCMAKE_TOOLCHAIN_FILE=vcpkg/scripts/buildsystems/vcpkg.cmake" && \
cd /tmp/build && make && ./server

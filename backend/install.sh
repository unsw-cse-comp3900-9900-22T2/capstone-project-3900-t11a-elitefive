#!/bin/sh
# only run this in the very beginning 
if [ -n "vcpkg" ]; then
  echo "-=-=-=-=-=setting up=-=-=-=-=-=-"
  git clone https://github.com/Microsoft/vcpkg.git
  cd vcpkg
  ./bootstrap-vcpkg.sh
  ./vcpkg integrate install
else
  cd vcpkg
fi
echo "-=-=-=-=-=installing dependencies=-=-=-=-=-"
./vcpkg install uwebsockets
./vcpkg install nlohmann-json
apt-get update
apt install -y libpq-dev
apt install -y libpqxx-dev
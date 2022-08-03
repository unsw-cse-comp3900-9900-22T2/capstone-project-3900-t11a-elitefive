#!/bin/sh
 
wd=$(pwd)
fedir="${wd}/frontend"
bedir="${wd}/backend"
dbdir="${wd}/database"
 
apt-get update
 
sh database/pg_setup.sh
 
apt install -y cmake
apt install -y libpq-dev
apt install -y libpqxx-dev
apt install zlib1g
apt install zlib1g-dev
 
apt install -y g++
 
# Set up database
cd "$dbdir"
sh db_setup.sh
 
# Set up frontend
cd "$fedir"
apt install curl
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.1/install.sh | bash
source ~/.bashrc
nvm install node
npm install
# npm start
 
# Set up backend
cd "$bedir"
cmake CMakeLists.txt
make
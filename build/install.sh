#!/usr/bin/bash
 
wd=$(pwd)
fedir="${wd}/frontend"
bedir="${wd}/backend"
dbdir="${wd}/database"
 
apt-get -y update

apt install -y cmake
apt install -y libpq-dev
apt install -y libpqxx-dev
apt install -y zlib1g
apt install -y zlib1g-dev
apt install -y libssl-dev
apt install -y g++
 
# Set up database
cd "$dbdir"
sh pg_setup.sh

# Set up backend
cd "$bedir"
apt-get install -y mutt
apt-get install -y sendmail
mkdir /home/lubuntu/.mutt
cp "${bedir}/mail/muttrc" /home/lubuntu/.mutt/muttrc
chmod +x "${bedir}/mail/send_email.sh"
cd "$bedir"
cmake CMakeLists.txt
make
 
# Set up frontend
cd "$fedir"
apt install curl
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.39.1/install.sh | bash
export NVM_DIR="$HOME/.nvm"
[ -s "$NVM_DIR/nvm.sh" ] && \. "$NVM_DIR/nvm.sh"  # This loads nvm
[ -s "$NVM_DIR/bash_completion" ] && \. "$NVM_DIR/bash_completion"  # This loads nvm bash_completion
nvm install node
npm install
sh qterminal -e npm start

# Run backend 
cd "$bedir"
qterminal -e ./server &

# Run frontend
cd "$fedir"
# has bug atm, idk
# qterminal -e npm start &

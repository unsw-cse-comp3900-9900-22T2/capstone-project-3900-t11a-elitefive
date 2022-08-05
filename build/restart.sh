#!/usr/bin/bash
 
wd=$(pwd)
fedir="${wd}/frontend"
bedir="${wd}/backend"
dbdir="${wd}/database"

# Run frontend
cd "$fedir"
qterminal -e npm start &

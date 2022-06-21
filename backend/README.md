## How to Run Backend Container
```
make 
docker exec -it cpp-backend /bin/bash -i
```
this will spin up a shell into the backend docker container

## First time running
`./install.sh`

this installs all the c++ dev dependencies via vcpkg

## Building the app
`./build.sh`

this runs the cmake and make commands to recompile the binary


## How to close the container
if the shell is still open, run `exit`
then run `make clean` to clean up the containers
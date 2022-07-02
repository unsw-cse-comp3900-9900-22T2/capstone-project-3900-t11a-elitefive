## How to run?

normal running
`make`

when you need to rebuild
`make build`

run this after your are done developing
`make clean`

if make clean doesn't work to remove containers 
`docker stop $(docker ps -q)`
`docker rm $(docker ps -qa)`
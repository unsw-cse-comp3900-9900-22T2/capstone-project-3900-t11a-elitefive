
## Building the container 

From root directory:
`docker compose up`

## Debuging the databas

Shell into the container:

`sudo docker exec -it pg-database bash`

Open postgres

`psql -U yavalath`

Check relations

`\d`

## Docker Cheat Sheet

Show running containers:
`docker ps`

Show stopped containers:
`sudo docker ps -a`

Show images:
`docker images`

Delete container:
`docker rm <container-id>`

Delete image:
`docker image rm <image-id>`

Shell into container:
`docker exec -it <container-name>`

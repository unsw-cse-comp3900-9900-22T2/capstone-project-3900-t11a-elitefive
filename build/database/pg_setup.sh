#!/usr/bin/sh

sudo apt update

dpkg --configure -a

sudo apt install -y curl 
sudo apt install -y gpg 
sudo apt install -y gnupg2 
sudo apt install -y software-properties-common 
sudo apt install -y apt-transport-https 
sudo apt install -y lsb-release 
sudo apt install -y ca-certificates

curl -fsSL https://www.postgresql.org/media/keys/ACCC4CF8.asc|sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/postgresql.gpg

echo "deb http://apt.postgresql.org/pub/repos/apt/ `lsb_release -cs`-pgdg main" |sudo tee  /etc/apt/sources.list.d/pgdg.list

sudo apt -y update

sudo dpkg --configure -a

apt install -y postgresql-13 postgresql-client-13

timeout 2 systemctl status postgresql@13-main.service

cp pg_hba.conf /etc/postgresql/13/main/pg_hba.conf

service postgresql restart

psql -U postgres -c 'create role lubuntu with superuser createdb login createrole'

createdb yavalath

psql yavalath -f schema.sql

#!/usr/bin/sh

apt update

dpkg --configure -a

apt install curl gpg gnupg2 software-properties-common apt-transport-https lsb-release ca-certificates

curl -fsSL https://www.postgresql.org/media/keys/ACCC4CF8.asc|sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/postgresql.gpg

echo "deb http://apt.postgresql.org/pub/repos/apt/ `lsb_release -cs`-pgdg main" |sudo tee  /etc/apt/sources.list.d/pgdg.list

apt update

sudo dpkg --configure -a

apt install postgresql-13 postgresql-client-13

cp pg_hba.conf /etc/postgresql/13/main/pg_hba.conf

service postgresql restart

psql -U postgres -c 'create role lubuntu with superuser createdb login createrole'

createdb yavalath




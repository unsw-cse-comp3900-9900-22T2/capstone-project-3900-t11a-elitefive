FROM postgres:13.0-alpine

ENV POSTGRES_PASSWORD=yavalath
ENV POSTGRES_USER=yavalath
ENV POSTGRES_DB=yavalath

COPY schema.sql /docker-entrypoint-initdb.d/schema.sql


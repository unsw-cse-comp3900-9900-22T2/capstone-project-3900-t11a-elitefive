FROM ubuntu:latest

WORKDIR /app

RUN apt update -y && \
    apt upgrade -y && \
    apt install -y \
      vim \
      g++ \
      bash \
      git \
      curl \
      zip \
      unzip \
      tar \
      libstdc++6
RUN apt install -y \
      cmake \
      pkg-config
RUN apt install -y libpq-dev
RUN apt install -y libpqxx-dev

EXPOSE 8080

WORKDIR /app
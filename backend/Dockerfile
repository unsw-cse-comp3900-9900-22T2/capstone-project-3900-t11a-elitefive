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
RUN apt update -y
RUN apt install -y libpq-dev
RUN apt install -y libpqxx-dev

RUN apt install -y mutt
RUN mkdir /root/.config 
RUN mkdir /root/.config/mutt
COPY mail/muttrc /root/.config/mutt

EXPOSE 8080

WORKDIR /app


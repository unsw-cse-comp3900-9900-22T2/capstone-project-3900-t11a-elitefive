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

EXPOSE 8080

COPY install.sh /app/install.sh
COPY vcpkg /app/vcpkg

WORKDIR /app

RUN ./install.sh
ENTRYPOINT ["./build.sh"]

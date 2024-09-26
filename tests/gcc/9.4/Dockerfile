FROM --platform=linux/amd64 ubuntu:20.04

RUN apt update -y
RUN DEBIAN_FRONTEND=noninteractive apt install -y build-essential
RUN DEBIAN_FRONTEND=noninteractive apt install -y wget
WORKDIR /tmp/
RUN wget https://github.com/Kitware/CMake/releases/download/v3.28.4/cmake-3.28.4-linux-x86_64.tar.gz
RUN tar zxvf cmake-3.28.4-linux-x86_64.tar.gz
RUN DEBIAN_FRONTEND=noninteractive apt install -y git
RUN DEBIAN_FRONTEND=noninteractive apt install -y libwayland-dev
RUN DEBIAN_FRONTEND=noninteractive apt install -y pkg-config
RUN DEBIAN_FRONTEND=noninteractive apt install -y libxkbcommon-dev
RUN DEBIAN_FRONTEND=noninteractive apt install -y xorg-dev

ADD . /tmp/clay

WORKDIR /tmp/clay

CMD /tmp/cmake-3.28.4-linux-x86_64/bin/cmake . && /tmp/cmake-3.28.4-linux-x86_64/bin/cmake --build .
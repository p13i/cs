FROM ubuntu:22.04

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && \
    apt-get -y install sudo && \
    sudo apt-get -y install make

WORKDIR /app

COPY . .

RUN sudo make setup

EXPOSE 8080

CMD ["make", "http"]

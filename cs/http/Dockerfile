FROM ubuntu:22.04

RUN apt install sudo

RUN DEBIAN_FRONTEND=noninteractive sudo apt-get update

RUN useradd -ms /bin/bash app

USER app

RUN make setup

EXPOSE 8080

CMD ["make", "http"]

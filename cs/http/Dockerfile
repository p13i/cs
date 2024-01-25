FROM ubuntu:22.04

RUN DEBIAN_FRONTEND=noninteractive sudo apt-get update

RUN useradd -ms /bin/bash app

USER app

RUN make setup

CMD ["make", "http"]

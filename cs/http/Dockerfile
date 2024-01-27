FROM ubuntu:22.04

RUN DEBIAN_FRONTEND=noninteractive \
    apt-get update && \
    apt-get -y install sudo

RUN useradd -ms /bin/bash app

USER app

RUN make setup

EXPOSE 8080

CMD ["make", "http"]

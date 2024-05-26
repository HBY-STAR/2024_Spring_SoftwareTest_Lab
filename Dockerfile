FROM klee/klee:2.3

RUN sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1A127079A92F09ED
RUN sudo apt-get update
RUN sudo apt-get install -y \
    openssh-server \
    openssh-client

ENV PATH=/home/klee/klee_build/bin:$PATH
COPY demo demo

RUN sudo service ssh start
EXPOSE 22

ENTRYPOINT ["/bin/bash"]

FROM gcc:10.2
RUN apt upgrade -y
RUN apt update -y
RUN apt install gdbserver -y
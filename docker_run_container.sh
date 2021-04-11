#!/bin/sh
docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined --rm -it -v $(pwd):/home/algorithms -w="/home/algorithms" -p 2000:2000 random_algorithms /bin/bash
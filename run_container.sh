#!/bin/sh
docker run --rm -it -v $(pwd):/home/algorithms -w="/home/algorithms" gcc:10.2 /bin/bash
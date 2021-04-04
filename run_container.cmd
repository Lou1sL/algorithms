@echo off
docker run --rm -it -v %cd%:/home/algorithms -w="/home/algorithms" gcc:10.2 /bin/bash
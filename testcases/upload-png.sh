#!/bin/bash

curl -X POST -F "file=@./test.png" "localhost:80/upload"

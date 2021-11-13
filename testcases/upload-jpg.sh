#!/bin/bash

curl -X POST -F "file=@./test.jpg" "localhost:80/upload"

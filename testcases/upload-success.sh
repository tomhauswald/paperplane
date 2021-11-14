#!/bin/bash

curl -X POST -F "file=@./supported.jpg" "localhost:80/upload"

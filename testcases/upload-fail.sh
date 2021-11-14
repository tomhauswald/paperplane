#!/bin/bash

curl -X POST -F "file=@./unsupported.png" "localhost:80/upload"

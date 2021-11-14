#!/bin/bash

curl -X POST -F "file=@$(dirname ${0})/supported.jpg" "localhost:80/upload"

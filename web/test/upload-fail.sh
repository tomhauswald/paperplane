#!/bin/bash

curl -X POST -F "file=@$(dirname ${0})/unsupported.png" "localhost:80/upload"

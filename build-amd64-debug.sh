#!/bin/bash

set -o pipefail
set -x

docker buildx build --tag paperplane:amd64-debug --platform linux/amd64 --file amd64-debug.docker .

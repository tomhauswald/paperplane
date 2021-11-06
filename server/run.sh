#!/bin/bash

docker run \
  -p 80:80 \
  -v $(realpath ./server.py):/mnt/paperplane/server.py:ro \
  -v $(realpath ../db):/mnt/paperplane/db:rw \
  paperplane-web:${arch-amd64}

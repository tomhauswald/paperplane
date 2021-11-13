#!/bin/bash

docker run \
  --rm --init --privileged \
  -p 80:80 \
  -v /opt/paperplane/db:/opt/paperplane/db:rw \
  -v /var/run/docker.sock:/var/run/docker.sock:rw \
  -v $(realpath ./web/server.py):/bin/server.py:ro \
 paperplane:amd64-debug

#!/bin/bash

for arch in amd64 arm64; do
  docker buildx build \
    --tag paperplane-web:${arch} \
    --platform linux/${arch} . 
done

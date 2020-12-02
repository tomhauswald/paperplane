#!/bin/bash

image_sha1="$(./util/_sha1.sh ${1})"
mkdir -p db/${image_sha1} 

docker run \
 --rm -i \
 -v $(realpath .):/mnt/workspace:rw \
 -v $(realpath ${1}):$(realpath ${1}):ro \
 -w /mnt/workspace \
 jbarlow83/ocrmypdf \
 --output-type pdfa-3 \
 --language ${lang} \
 --rotate-pages \
 --deskew \
 --clean \
 --optimize 1 \
 --force-ocr \
 --oversample 800 \
 --sidecar db/${image_sha1}/TEXT \
 $(realpath ${1}) \
 db/${image_sha1}/PDF

pdf_sha1="$(./util/_sha1.sh db/${image_sha1}/PDF)"

cp ${1} db/${image_sha1}/IMAGE
echo "${image_sha1}" > db/${image_sha1}/IMAGE_SHA1
echo "${pdf_sha1}" > db/${image_sha1}/PDF_SHA1

./util/_tokenize.sh db/${image_sha1}/TEXT > db/${image_sha1}/TOKENS

sudo chown -R $(id -u):$(id -g) db/${image_sha1}
chmod 0644 db/${image_sha1}/*


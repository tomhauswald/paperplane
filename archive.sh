#!/bin/bash

set -o pipefail

if [ ! -f "${1}" ]; then
  echo "Usage: [lang=] ./archive.sh /path/to/image"
  exit 1
fi

src_image_path="$(realpath ${1})"
id="$(uuidgen)"
echo "Generated document id ${id} for file ${src_image_path}."

set -x

mkdir -p ./db/${id} 
install -m 0644 ${src_image_path} ./db/${id}/image

docker run \
 --rm -i \
 -v $(realpath ./db/${id}):/mnt/doc:rw \
 -w /mnt/workspace \
 jbarlow83/ocrmypdf \
 --output-type pdfa-3 \
 --language ${lang-"deu+eng"} \
 $(false && echo --rotate-pages) \
 $(false && echo --deskew) \
 $(false && echo --clean) \
 --optimize 1 \
 --force-ocr \
 $(false && echo --oversample 800) \
 --sidecar /mnt/doc/text \
 /mnt/doc/image \
 /mnt/doc/pdf

./util/_sha1.sh     ./db/${id}/pdf   > ./db/${id}/pdf-checksum
./util/_sha1.sh     ./db/${id}/image > ./db/${id}/image-checksum
./util/_tokenize.sh ./db/${id}/text  > ./db/${id}/tokens


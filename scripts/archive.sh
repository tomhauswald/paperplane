set -o pipefail

if [ ! -f "${1}" ]; then
  echo "Usage: /bin/sh archive.sh /path/to/image1 ... /path/to/imageN"
  exit 1
fi

for input in "${@}"; do
  
  src_image_path="$(realpath ${input})"

  image_hash=$(/bin/sh "$(dirname ${0})/hash.sh" ${src_image_path})
  doc_dir="/opt/paperplane/db/${image_hash}"

  install -Dm 0644 ${src_image_path} ${doc_dir}/image

  docker run \
   --rm --init --privileged \
   -v $(realpath ${doc_dir}):/mnt/doc:rw \
   -w /mnt/workspace \
   jbarlow83/ocrmypdf \
   --output-type="pdfa-3" \
   --language="deu+eng" \
   --rotate-pages \
   --deskew \
   --clean \
   --optimize=1 \
   --force-ocr \
   --image-dpi=100 \
   --sidecar=/mnt/doc/text \
   /mnt/doc/image \
   /mnt/doc/pdf

  if [ -f ${doc_dir}/pdf ]; then
    /bin/sh "$(dirname ${0})/hash.sh"     "${doc_dir}/pdf"   > "${doc_dir}/pdf-hash"
    /bin/sh "$(dirname ${0})/tokenize.sh" "${doc_dir}/text"  > "${doc_dir}/tokens"
  else
    echo "Failed to convert image to PDF."
    rm -rf ${doc_dir}
  fi

done

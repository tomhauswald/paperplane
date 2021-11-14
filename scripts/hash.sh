set -e
set -u
set -o pipefail

sha256sum --binary ${1} | awk '{print $1}'

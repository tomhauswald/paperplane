#!/bin/bash
sha1sum --binary ${1} | awk '{print $1}'

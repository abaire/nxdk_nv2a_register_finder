#!/usr/bin/env bash
set -eu
set -o pipefail

readonly test_output_dir='e:\nv2a_register_finder'

if [[ $# -gt 0 ]]; then
  xbox=$1
else
  if [[ -z "${XBOX:-}" ]]; then
    xbox="192.168.80.87"
  else
    xbox="${XBOX}"
  fi
fi


echo "Collecting output of nxdk_nv2a_register_finder from ${xbox}..."
xbdm "${xbox}" getfile "${test_output_dir}" ./results


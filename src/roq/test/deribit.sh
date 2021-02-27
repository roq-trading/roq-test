#!/usr/bin/env bash

CWD="$(realpath "$(dirname "${BASH_SOURCE[0]}")")"

if [ "$1" == "debug" ]; then
  PREFIX="gdb --args"
  shift 1
else
  PREFIX=
fi

NAME="trader"

$PREFIX ./roq-test \
  --name "$NAME" \
  --exchange "deribit" \
  --symbol "BTC-PERPETUAL" \
  --enable_modify_order \
  $@

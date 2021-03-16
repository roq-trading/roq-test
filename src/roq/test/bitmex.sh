#!/usr/bin/env bash

NAME="trader"

# debug?

if [ "$1" == "debug" ]; then
  KERNEL="$(uname -a)"
  case "$KERNEL" in
    Linux*)
      PREFIX="gdb --args"
      ;;
    Darwin*)
      PREFIX="lldb --"
      ;;
  esac
  shift 1
else
  PREFIX=
fi

# launch

$PREFIX "./roq-test" \
  --name "$NAME" \
  --exchange "bitmex" \
  --symbol "XBTUSD" \
  --enable_modify_order \
  $@

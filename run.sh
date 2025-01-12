#!/usr/bin/env bash

set -x

[ "$1" == "server" ] && ./build.sh && ./build/server

[ "$1" == "client" ] && ./build.sh && ./build/client

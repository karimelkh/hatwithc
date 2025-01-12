#!/usr/bin/env bash

set -x

CC=gcc
CFLAGS=-Wall

[ ! -d "build" ] && mkdir build

$CC $CFLAGS -o ./build/server ./src/server.c
$CC $CFLAGS -o ./build/client ./src/client.c

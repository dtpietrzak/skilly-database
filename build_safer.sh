#!/bin/bash

lsof -i :7777 | grep LISTEN | awk '{print $2}' | xargs kill -9

gcc -fsanitize=address -g -o build/database main.c $(find settings -name "*.c") $(find utils -name "*.c") $(find requests -name "*.c") $(find server -name "*.c") \
    -I./utils -I./requests -I./server \
    -I/opt/homebrew/opt/openssl/include \
    -L/opt/homebrew/opt/openssl/lib -lssl -lcrypto \
    -I/opt/homebrew/opt/libuv/include \
    /opt/homebrew/opt/libuv/lib/libuv.a \
    -framework CoreFoundation -framework CoreServices

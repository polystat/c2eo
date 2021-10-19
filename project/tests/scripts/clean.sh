#!/bin/bash

for fn in `find ../tests -name "*.c"`; do
    eval " rm -rfv ${fn%%.c}"
done

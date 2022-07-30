#!/bin/bash

java -Xmx256m -cp target/classes:target/eo-runtime.jar org.eolang.Main c2eo.src.union.some_104.global "$@"

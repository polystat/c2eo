#!/bin/bash

java -cp target/classes:target/eo-runtime.jar org.eolang.Main c2eo.examples.fibonacci5.global "$@"

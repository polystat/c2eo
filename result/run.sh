#!/bin/bash

java -Xmx256m -cp target/classes:target/eo-runtime.jar org.eolang.Main c2eo.src.bug02.init01.global "$@"

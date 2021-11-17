#!/bin/bash

java -cp target/classes:target/eo-runtime.jar org.eolang.Main c2eo.src.swapBCA.app "$@"
#java -cp target/classes:target/eo-runtime.jar org.eolang.Main c2eo.app "$@"
# java -cp "target/classes;target/eo-runtime.jar" org.eolang.Mainc2eo.src.app.%*

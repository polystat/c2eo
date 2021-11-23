#!/bin/bash

#java -cp target/classes org.eolang.Main --verbose c2eo.app $@
java -cp target/classes:target/eo-runtime.jar org.eolang.Main c2eo.app "$@"

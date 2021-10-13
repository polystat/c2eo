#!/bin/bash
object_to_run=$1
shift
java -cp target/classes:target/eo-runtime.jar org.eolang.phi.Main c2eo.${object_to_run:-app} "$@"

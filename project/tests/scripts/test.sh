#!/bin/bash -e

bash ./clean.sh
for fn in `find ../tests -name "*.c"`; do
    CTOEO=$(eval " realpath $1")
    FILE=$(eval " realpath ${fn}")
    NAME="${fn##*/}"
    mkdir ${FILE%%.c}
    unzip env.zip -d ${FILE%%.c}
    gcc ${FILE} -o ${FILE%%.c}/${NAME%%.c}.out -w
    eval " ${FILE%%.c}/${NAME%%.c}.out > ${FILE%%.c}/${NAME%%.c}.res_c"
    eval " ${CTOEO} ${FILE} -- -d ${FILE%%.c}/env/eo/ > ${FILE%%.c}/${NAME%%.c}.details 2>${FILE%%.c}/${NAME%%.c}.log"
    mvn -f ${FILE%%.c}/env/ clean compile
    eval " ${FILE%%.c}/env/run.sh > ${FILE%%.c}/${NAME%%.c}.res_eo"
    diff -a ${FILE%%.c}/${NAME%%.c}.res_c ${FILE%%.c}/${NAME%%.c}.res_eo > ${FILE%%.c}/${NAME%%.c}.diff
done

for diff in `find ../tests -name "*.diff"`; do
    TEST=$(eval " realpath ${diff}")
    if [ -s $diff ]; then
            # The file is not-empty.
            printf "\033[0;31m${TEST}"
    else
            # The file is empty.
            printf "\033[0;32m${TEST}"
    fi
done

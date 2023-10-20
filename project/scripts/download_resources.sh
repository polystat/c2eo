#wget "https://github.com/polystat/c2eo/tree/master/project/eo-lib"
echo copy eo-lib to "$1"
cp -r ./../eo-lib/ "$1"

# @todo #1049:30min In this implementation, we simply
#  copy the current implementation, which lies at the
#  relative address locally. We have to download the
#  current version from the repository. The address
#  is given above in the comment.
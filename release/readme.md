To use the script, make sure that you have installed the following packages:
```bash
$ pip3 install git_config
$ sudo apt install md5deep reprepro
```

```bash
$ python3 ./release/update-release.py -h
usage: update-release.py [-h] [--branch BRANCH] [--version VERSION]

Release maker

optional arguments:
  -h, --help         show this help message and exit
  --branch BRANCH    name of the branch to check out (default is heap)
  --version VERSION  specify the new version
```


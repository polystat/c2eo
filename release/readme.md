To use the script, make sure that you have installed the following packages:
```bash
$ pip3 install git_config
$ sudo apt install md5deep reprepro
```

```bash
$ python3 ./release/update-release.py -h
usage: update-release.py [-h] [--branch BRANCH] [--version VERSION] [--date DATE] [--user USER]

Release maker

optional arguments:
  -h, --help         show this help message and exit
  --branch BRANCH    name of the branch to check out (default is heap)
  --version VERSION  specify the new version (by default, the last number increases)
  --date DATE        use the date and time in the format %a, %d%b %Y %H:%M:%S%z, example: Sun, 13 Feb 2022 23:35:51 +0300 (the current time is used by default)
  --user USER        use the parameter to specify the full name and email address, example: Yaroslav Ryabtsev <yairyabtsev@edu.hse.ru> (data from git_config is used by default)
```


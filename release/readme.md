To use this script, make sure you have the following packages installed:
```bash
(venv) $ pip3 install git_config
$ sudo apt install md5deep reprepro
```
Also, check that in `CMakeLists.txt` the correct path to the libraries is specified: 
```cmake
link_directories("/usr/lib")
```

### How to use:
```bash
(venv) $ python3 update-release.py -h
usage: update-release.py [-h] [--branch BRANCH] [--version VERSION]

Release maker

optional arguments:
  -h, --help         show this help message and exit
  --branch BRANCH    name of the branch to check out (default is heap)
  --version VERSION  specify the new version
```
Example
```bash
(venv) $ python3 update-release.py --version=0.1.1 --branch="yar"
```
Notes: 
* Use `.` as a version delimiter.
* This script uses the current date, time, and time zone. Make sure they are configured correctly.
* This script extracts your name and email from `git config`. Make sure you have them.

This script will write automatically generated merges to the changelog file.
You can view an approximate list of changes by running the following command in the terminal:

```bash
$ git log $(git describe --tags --abbrev=0)..HEAD --merges --oneline --format="  * %h %s by %an <%aE>"
```

### Algorithm:
* Build the executable file. 
* Create a deb file (basic: https://habr.com/ru/post/78094/)
* Create a repository (basic: https://unixforum.org/viewtopic.php?t=79513 )

The following files will be generated:
```bash 
$ tree
.
├── c2eo-X.X.X
│   ├── DEBIAN
│   │   ├── changelog
│   │   ├── control
│   │   ├── copyright
│   │   └── md5sums
│   └── usr
│       ├── bin
│       │   └── c2eo
│       └── lib
│           ├── libclangAnalysis.so
│           ├── libclangAnalysis.so.12
│           ├── ...
│           └── libLLVMTransformUtils.so.12
├── c2eo-X.X.X.deb
├── readme.md
├── repository
│   ├── conf
│   │   └── distributions
│   ├── db
│   │   ├── checksums.db
│   │   ├── contents.cache.db
│   │   ├── packages.db
│   │   ├── references.db
│   │   ├── release.caches.db
│   │   └── version
│   ├── dists
│   │   └── c2eo-rep
│   │       ├── contrib
│   │       │   ├── binary-amd64
│   │       │   │   ├── Packages
│   │       │   │   ├── Packages.gz
│   │       │   │   └── Release
│   │       │   ├── binary-i386
│   │       │   │   ├── Packages
│   │       │   │   ├── Packages.gz
│   │       │   │   └── Release
│   │       │   ├── debian-installer
│   │       │   │   ├── binary-amd64
│   │       │   │   │   ├── Packages
│   │       │   │   │   └── Packages.gz
│   │       │   │   └── binary-i386
│   │       │   │       ├── Packages
│   │       │   │       └── Packages.gz
│   │       │   └── source
│   │       │       ├── Release
│   │       │       └── Sources.gz
│   │       ├── InRelease
│   │       ├── main
│   │       │   ├── binary-amd64
│   │       │   │   ├── Packages
│   │       │   │   ├── Packages.gz
│   │       │   │   └── Release
│   │       │   ├── binary-i386
│   │       │   │   ├── Packages
│   │       │   │   ├── Packages.gz
│   │       │   │   └── Release
│   │       │   ├── debian-installer
│   │       │   │   ├── binary-amd64
│   │       │   │   │   ├── Packages
│   │       │   │   │   └── Packages.gz
│   │       │   │   └── binary-i386
│   │       │   │       ├── Packages
│   │       │   │       └── Packages.gz
│   │       │   └── source
│   │       │       ├── Release
│   │       │       └── Sources.gz
│   │       ├── non-free
│   │       │   ├── binary-amd64
│   │       │   │   ├── Packages
│   │       │   │   ├── Packages.gz
│   │       │   │   └── Release
│   │       │   ├── binary-i386
│   │       │   │   ├── Packages
│   │       │   │   ├── Packages.gz
│   │       │   │   └── Release
│   │       │   ├── debian-installer
│   │       │   │   ├── binary-amd64
│   │       │   │   │   ├── Packages
│   │       │   │   │   └── Packages.gz
│   │       │   │   └── binary-i386
│   │       │   │       ├── Packages
│   │       │   │       └── Packages.gz
│   │       │   └── source
│   │       │       ├── Release
│   │       │       └── Sources.gz
│   │       ├── Release
│   │       └── Release.gpg
│   └── pool
│       └── main
│           └── c
│               └── c2eo
│                   └── c2eo_X.X.X_all.deb
├── todo.sh
└── update-release.py

35 directories, 120 files
```

Then you have to upload `./repository/dists` and `./repository/pool` to ... `TODO: Amazon S3`

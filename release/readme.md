To use this script, make sure you have the following packages installed:
```bash
(venv) $ pip3 install git_config pgpy s3cmd
$ sudo apt install md5deep reprepro gcc cmake dpkg wget tar s3cmd -y
# for the latest version of the cmake package, try:
$ sudo pip3 install cmake
```

### How to use:
```bash
(venv) $ python3 update-release.py -h
usage: update-release.py [-h] [--branch BRANCH] [--version VERSION]

Release maker

optional arguments:
  -h, --help         show this help message and exit
  --version VERSION  specify the new version
```
Example
```bash
(venv) $ python3 update-release.py --version=0.1.1
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
* Create a deb file (basic: [HABR](https://habr.com/ru/post/78094/))
* Create a repository (basic: [UNIXFORUM](https://unixforum.org/viewtopic.php?t=79513))
* Upload a repository tree into the bucket's virtual 'directory'.

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

Then you have to upload `./repository/dists` and `./repository/pool` to [c2eo.polystat.org/](http://c2eo.polystat.org)debian/.

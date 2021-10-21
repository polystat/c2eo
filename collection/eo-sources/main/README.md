<img src="https://www.yegor256.com/images/books/elegant-objects/cactus.svg" height="100px" />

You can run this C2EO here, in a few simple steps:

First, clone this repo to your local machine and go
to the `main` directory (you will need
[Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
installed):

```bash
$ git clone https://github.com/polystat/c2eo.git
$ cd c2eo/collection/eo-sources/main
```

Then, compile the code (you will need [Maven 3.3+](https://maven.apache.org/)
and [Java SDK 8+](https://www.java.com/en/download/) installed):

```bash
$ mvn clean compile
```

Intermediary `*.xml` files will be generated in the `target` directory (it will
be created). Also, there will be `*.java` and `*.class` files. Feel free to analyze
them: EO is parsed into XML, then translated to Java, and then compiled
by Java SDK to Java bytecode. Finally, just run the bytecode program through JRE:

```bash
$ ./run.sh 5
... many lines of code result
```

```cmd
run.cmd 5
... many lines of code result
```

Should work. If it doesn't, [submit an issue](https://github.com/polystat/c2eo/issues),
we will fix it.
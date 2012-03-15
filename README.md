# jnibind
jnibind generates c++ & java jni binding class for your own c structure.

c structure parse tool : flex, bison

## Usage

`jnibind` \<input header file\> \<bind name\>

If you have some user defined type(of typedef) in input header file, you must add your new type in jnibind.l file.

jnibind's output files are 

* Bind<name>.cpp
* Bind<name>.h
* Bind<name>.java
* Bind<name>_test.cpp

you use thease file to your androind project.

Also `jnibind` perform making test program and running it to verify that output files are generated correctly.

# cDBC library
A simple library for reading files with the .dbc extension. Files with the extension .dbc are also known as CAN database files. In the src directory of the project there is an example.c file with examples of using functions, and also in the root of the project there is a tesla_can.dbc file of the Tesla car can matrix. tesla_can.dbc is used for in the example program. You can simply connect cDBC.c and cDBC.h files to your project directly or build a shared library and use it.
# Building
To build an example program, simply type:
``` Shell
make
```
To start the program, type:
``` Shell
make run
```
or
``` Shell
./example
```
To build a cdbc library type:
``` Shell
make lib
```
To clean the project, type:
```Shell
make clean
```
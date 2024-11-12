# cDBC library
A simple library for reading files with the .dbc extension. Files with the extension .dbc are also known as CAN database files. In the examples directory of the project there are .c files with examples of using functions, and also in the root of the project there is a tesla_can.dbc file of the Tesla car CAN matrix. tesla_can.dbc is used for in the example programs. You can simply connect cDBC.c and cDBC.h files to your project directly or build a shared library and use it.
# Building
To build an example programs, simply type:
``` Shell
make
```
To start the program, type:
``` Shell
make run-<program_name>
```
or
``` Shell
./bin/<program_name>
```
To build a cdbc library, type:
``` Shell
make lib
```
To clean the project, type:
```Shell
make clean
```

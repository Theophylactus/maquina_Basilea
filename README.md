# maquina_Basilea
A fast C math parser I made back in 2019. Two years later I refactored the program, making it faster, more robust and non-C++ dependent.
Like most of the stuff I make, this didn't follow any guide. I came up with the algorithm myself. All I know about its quality is that it outputted the right answer in all the tests I put it through; besides, it's *very* fast.

The program will interpret any arithmetic expression built with the most fundamental operators (+, -, ^, *, /), most mathematical functions included in `math.h`, nested parentheses and the mathematical constants `PI` and `E`.
No error handling was implemented. Bad expressions will simply give a result of 0.

## Usage
Compiling: run `./build.sh` to generate library and executable files.  
Running: `./Basilea <expression>` or `./Basilea` to enter REPL mode.

A silly graph made with a program built with libbasilea.so
![alt text](https://raw.githubusercontent.com/Theophylactus/maquina_Basilea/main/sine.png)

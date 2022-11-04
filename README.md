# maquina_Basilea
This is a C math parser I made back in 2019. Two years later I refactored the program, making it faster, more robust and non-C++ dependent.
The program was made with little to no outer influence, so reliability and efficiency are not guaranteed, even though it was made from start to finish with high performance as a top priority. I haven't even done any proper, comparative benchmarking. 

The program will interpret any arithmetic expression built with the most fundamental operators (+, -, ^, *, /), most mathematical functions included in `math.h`, nested parentheses and the mathematical constants `PI` and `E`.
No error handling was implemented. Bad expressions will simply give a result of 0.

## Usage
Compiling: run `./build.sh` to generate library and executable files.  
Running: `./Basilea <expression>` or `./Basilea` to enter REPL mode.

A silly graph made with a program built with libbasilea.so
![](https://raw.githubusercontent.com/Theophylactus/maquina_Basilea/main/sine.png)

# This program simulates the work of simple made-up language.

Structure of the program:
1. The compiler generates optimized machine code.
2. The interpreter reads machine code and takes action.

We have 26 variables named as letters (a-z). At first, every variable is set to 0.

Variables have a special built-in type that can contain non-negative numbers up to INT_MAX^INT_MAX - 1.

## LANGUAGE

* 'a' - increases the value of a by 1
```
  ++a;
```
  for example - 'aabbb' sets value a to 2 and value b to 3
* '(ab...)' - it works as function:
```
  while (a > 0) {
     --a;
     ++b; 
     ...
  }
```
* '=a' - writes value of variable a

## MACHINE CODE INSTRUCTIONS

* INC 'variable' (increment)  
  increase value of variable by 1
* ADD 'variable0' 'variable1' (add)  
  add value of variable1 to variable0
* CLR 'variable' (clear)  
  set variable 'variable' to 0
* JMP 'adress' (jump)  
  jump to adress 'adress'
* DJZ 'variable' 'adress' (decrement or jump if 0)  
  if value of 'variable' is 0 - jump to adress 'adress'  
  else - decrease value of 'variable' by 1
* HLT (halt)  
  end progam
  
If in instruction (...) there is no another instruction (...) and if no variable increasing is the same as variable decreasing, then the compiler generates optimized code.
For example:
(ab)
generates:

ADD b a  
CLR a

instead of:

DJZ a 'adress of instruction after JMP'  
INC b  
JMP 'adress of instruction DJZ'


If you want to see machine code, uncomment function write_code in function compile.

## EXAMPLE

***input:***

aaaaaaaaaa  
=a  
(ab)a(b(ac)(caa))  
=a  
(ab)a(b(ac)(caa))  
=a

***output:***

10  
1024  
179769313486231590772930519078902473361797697894230657273430081157732675805500963132708477322407536021120113879871393357658789768814416622492847430639474124377767893424865485276302219601246094119453082952085005768838150682342462881473913110540827237163350510684586298239947245938479716304835356329624224137216

## It was a graded project for the university. I scored max points in both quality and efficiency of code.

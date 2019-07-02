# LPP (front-end) Compiler

This proyect consists of a [LPP](https://github.com/mvillatoro/CompiladoresEjemplosLpp) compiler. Being able to run simple LPP programs like:

```
Entero x, _y, z

Inicio
    x <- 65
    _y <- 27
    z <- x + _y
    Escriba z
Fin
```

The example above would print `92` in the terminal.

### Files Description

There are 4 main folders in this project:

- **fa**:
    In this folder you will find the Finite Automaton used by the lexer to create the tokens. The files can be opened with [JFLAP](http://www.jflap.org/).

- **lexer**:
    Source files for the lexer. In summary the lexer is a tremendous _switch_ which goes letter by letter until it finds a token.

- **parser**:
    LL(1) parser, using recursivity.

- **ast**:
    From the parser, the ast is built. Here is were semantic errors are found. Unfortunately I didn't finish this part to handle arrays nor functions (help would be grateful).

### How to run

Some prerequisites to the lexer are:

1. A linux distro (preferable ubuntu)
2. cmake          (preferable 3.10.2)
3. GNU Make       (preferable 4.1)

Inside "lexer" and "ast" folders, there is a _CMakeList_ file; which creates a _Makefile_, which will create the executable.
I will assume you have a LPP file in the same folder of the source files.

```
/ast$ mkdir build
/ast$ cd build
/ast/build$ cmake ..
/ast/build$ make
/ast/build$ ./Lex ../boolTest.txt
VerdaderoFalso
/ast/build$ 
```

assuming boolTest.txt contains:
```
Booleano x

Inicio
    x <- Verdadero
    Escriba x
    x <- Falso
    Escriba x
Fin
```

### Other comments

you could also add the executable writing down `$ export LPP=<absolute route>` in the console, the problem with this is that the variable will be deleted when the instance of the console is destroyed.

Hope you have fun! :octocat:
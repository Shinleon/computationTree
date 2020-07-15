# computationTree

This program is made to digest and evaluate simple expressions composed
of operators[```"^", "*", "/", "+", "-"```], floats, 
variables["a-zA-Z\_"], and parenthesis (to adjust operator precedence).

Certain variables are forbiden such as "exit", "quit", and "env";
the first two because they're how you quit the program, and the last
because inputting "env" [without the quotes], will print to the terminal
all declared variables and their values.

A Makefile is provided so you can just type ```make``` if you're using Linux and
have ```gcc ``` available.  
Thanks! And if you have any recommendations (even if it's formatting my readMe.md,
maybe how to organize my files, or coding style suggestions) please e-mail me: 
wombatinkombat@protonmail.com.
## Implementation
---

The interface (in interface.c) digests input as a string. It then splits the string
if it finds an equal sign so that there are two strings, before and after the equal sign.
The left strings goes into the storage, while the right string is evaluated.
If there is no equal sign, the right string will be characterwise equal to the 
string provided to the splitting function, while the left string will be empty.

For example, inputting "3\*4.5" will evaluate to 13.5 where as inputting
"VAR = 3\*4.5" will store ```13.5``` in "VAR". Spaces are ignored in input unless
they're in the middle of a float or variable name in which case they cause an error.

#### Digestion of Inputs

The digestion of inputs happens using repeated calls to ```getc(stdin)``` in
charnode.c . Each character is added to a linked list of a struct that holds
a char and a pointer to the next charnode.
When it reaches the newline character, it stops and points the last node to NULL;

e.g. "32 - (3-4)\*5^2/(VAR + 2)\n" would be digested into

```
'3' -> '2' -> '-' -> '(' -> '3' -> '-' -> '4' -> ')' -> '*' -> '5' -> '^' -> '2' 
-> '/' -> '(' -> 'V' -> 'A' -> 'R' -> '+' -> '2' -> ')' ->\ 
```

This then is used to make a parseList stuct defined in parserDef.h,
that converts each item into a compNode struct (to be explained further 
down) and holds them in sequence.

#### How the sequence of characters becomes a computable tree

The parseList for the example above would look like (using ' to surround
variable name, nothing to surround floats, " to surround operators, and 
a downward arrow to represent holding no computationNode but something in the 
subparen field of the parseList struct)

```
32 -> "-" ->  | -> "*" -> 5 -> "^" -> 2 -> "/" -> | ->\  
              V                                   V  
              3 -> "-" -> 4                     'VAR' -> "+" -> 2

```

An evaluator then goes through the parseList, first evaluting the parenthesis
statements recursively, then compressing around the exponentiation symbol ["^"]
then multiplication and division, then addition and subtraction

e.g. first pass (parenthesis compaction)
```
32 -> "-" -> "-" -> "*" -> 5 -> "^" -> 2 -> "/" -> "+" ->\  
             / \                                  /   \  
            3   4                              'VAR'   2  
```

e.g. second pass (exponentiation)  
```
32 -> "-" -> "-" -> "*" -> "^" -> "/" -> "+" ->\  
             / \           / \          /   \  
            3   4         5   2       'VAR'   2  
```

e.g. third pass (multiplication and division)
```
32 -> "-" -> "*" -> "/" -> "+" ->\  
           /     \         / \  
         "-"     "^"    'VAR' 2  
         / \     / \  
        3   4   5   2  

32 -> "-" -> "/" ->\  
          /       \
        "*"        "+"
      /     \      / \  
    "-"     "^"  'VAR' 2  
    / \     / \  
   3   4   5   2  
```

e.g. fourth pass (addition and subtraction)
```
         "-" ->\  
        /    \  
       /      \  
     32       "/"  
            /     \
           /       \ 
          /         \  
        "*"         "+"  
       /   \        / \  
     "-"   "^"   'VAR' 2  
     / \   / \  
    3   4 5   2  
```

This parseList is then free'd while the compNode underneath is returned to 
the interface. 

#### What the interface does with the computable tree

The interface then looks back and if there was an "=" in the original
input, the output is assigned to a variable of name that was user provided, and the
environment is printed.

Otherwise, if no "=" was provided, the computation node is evaluated using computer.c
who looks through the tree solving from leaves all the way up to the root; if a variable is
used in the evaluation, it records it in dependencies and looks for it in the "env" variable
which stores all the previously declared variables. If a necessary variable isn't declared, then
an error message is provided to the terminal, or if a variable has a recursive definiton
an error message is again, output to the terminal.

## Future Additions
--- 

In no particular order

1. variables will eventually be able to be deleted using "rm={variable_vame}"
and rm will have to become a forbidden variable name. 

2. being able to store multiple values in a float 
variable, so one can hold a log scale/linear list of items and then when
asked for the value of variables that depend on the float variable, a list 
will be returned.

3. simplifications, so that 2^2^x can be compacted to 4^x and 2*(3+4)* (x - 3)
will become 14*x - 42

4. functions like variables, so that one can declare a variable as "f(x)" and 
eventually introduce a derivative/integral like computation.

5. graphing. requiring some sort of pop up image output.

6. sin, cos, cosh, tan, ... other trig functions.

7. handwriting? NMIST database and ML, somehow sync to a GUI writing surface
 so that you could write out instead of typing.

8. negatives. so you can type -VAR instead of (0-VAR) for the negative of a variable



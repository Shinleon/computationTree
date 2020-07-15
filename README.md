# computationTree
computationTree

This program is make to digest and evaluate simple expressions composed
of the operators["^", "*", "/", "+", "-"], floats, 
variables["a-zA-Z\_"], and parenthesis to adjust operator
precedence.

Inputting "3*4.5" will evaluate to 13.5 and these values can be stored in 
variables using the notation "{variable_name} = {expression}". Spaces
are ignored in input.

The digestion of inputs happens using repeated calls to getc(stdin) in
charnode.c . Each character is added to a linked list of a struct that holds
a char and a pointer to the next charnode.
When it reaches the newline character, it stops and points the last node to NULL;

e.g. 32 - (3-4)\*5^2/VAR + 2 would be digested into
'''
'3' -> '2' -> '-' -> '(' -> '3' -> '-' -> '4' -> ')' -> '*' -> '5' -> '^' -> '2' 
-> '/' -> 'V' -> 'A' -> 'R' -> '+' -> '2' ->\ 
'''

#include "compNodeDef.h"
#include "environment.h"
#include "wordnode.h"

//Evaluates the provided compNode "node"
// returns a float that equals the value of the node 
// dependencies is recursively called inside, and should initially
// be passed in as NULL, unless there are variable names you want to ban
// the int* errorCode should be passed in as a pointer to a zero, and
// will return as zero if there were no errors in computing
// otherwise it will be a value determined in computer.c's define statements
// The parameter env lists the current environment. It it made of the variables
// currently active in the program.
float evalCompNode( struct compNode* node, 
                    struct environmentNode* env, 
                    struct wordnode* dependencies, 
                    int* errorCode);
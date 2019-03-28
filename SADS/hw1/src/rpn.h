#ifndef _RPN_H
#define _RPN_H

#define RPN_OK                  0
#define RPN_INVALID_TOKEN      -1
#define RPN_MISSING_OPERAND    -2
#define RPN_MISSING_OPERATOR   -3
#define RPN_ARITHMETIC_ERROR   -4

/**
 *  \brief Evaluate an expression in reverse polish notation
 *
 *  \param  token Array of tokens making up the expression
 *  \param result Pointer to the string which whill hold the result (malloced)
 *  \result One of the error codes defined above
***/

int rpn_eval_token(char *token[], char **result);

/**
 *  \bried Evaluate an expression in reverse polish notation
 *
 *  \param expr The expression (whitespace separated by numbers and operators)
 *  \paran result Pointer to the string which will hold the result (malloced)
 *  \result One of the error codes defined above
**/

int rpn_eval_expr (const char *expr, char **result);




#endif
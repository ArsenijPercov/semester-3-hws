/**
 * hw1/src/rpn.c
 * 
 * Author: Dushan Terzikj
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rpn.h"
#include "rpn-stack.h"

/* region GLOBALS */

int num_of_tokens = 0;

/* endregion */

/**
 * \brief   Checks whether the token is a sign.
 * \result  1 if it the token is a sign, 0 otherwise.
*/
int token_sign(char *token) {
    if(token != NULL 
        && (strcmp(token, "+") == 0 
        || strcmp(token, "-") == 0 
        || strcmp(token, "*") == 0 
        || strcmp(token, "/") == 0)) {
            return 1;
    }
    return 0;
}

/**
 * \brief   Calculates the numerical value of the operation.
 * \result  The numberical value of an operation (+ -> 0, * -> 1, - -> 2, / -> 3).
*/
int op(char *token) {
    if(strcmp(token, "+") == 0) {
        return 0;
    } else if(strcmp(token, "*") == 0) {
        return 1;
    } else if(strcmp(token, "-") == 0) {
        return 2;
    } else {
        return 3;
    }
}

int rpn_eval_token(char *token[], char **result) {

    int i, fres_i;
    char buf[255];
    void *fres;
    rpn_stack_t *s = rpn_stack_new();

    /* Check for invalid tokens */
    for(i = 0; i < num_of_tokens; i++) {
        if(token_sign(token[i])) {
            continue;
        }
        if(atoi(token[i]) == 0 && strcmp(token[i], "0") != 0){
            return RPN_INVALID_TOKEN;
        }
    }

    for(i = 0; i < num_of_tokens; i++){
        if(token_sign(token[i])) {

            void *op1, *op2;
            int op1_i, op2_i;
            int *res;
            if(rpn_stack_empty(s)){
                return RPN_MISSING_OPERAND;
            }
            op1 = rpn_stack_pop(s);
            if(rpn_stack_empty(s)){
                return RPN_MISSING_OPERAND;
            }
            op2 = rpn_stack_pop(s);
            op1_i = *((int*)op1);
            op2_i = *((int*)op2);
            res = (int*) malloc(sizeof(int));
            if(!res) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }

            int op_type = op(token[i]);
            switch(op_type) {
                case 0:
                *res = op2_i + op1_i;
                break;
                case 1:
                *res = op2_i * op1_i;
                break;
                case 2:
                *res = op2_i - op1_i;
                break;
                default:
                if(op1_i == 0) {
                    free(res);
                    return RPN_ARITHMETIC_ERROR;
                }
                *res = op2_i/op1_i;
            }

            rpn_stack_push(s, res);
        } else {
            int *tmp = (int*) malloc(sizeof(int));
            *tmp = atoi(token[i]);
            rpn_stack_push(s, tmp); 
        }
    }

    fres = rpn_stack_pop(s);
    if(!rpn_stack_empty(s)) {
        return RPN_MISSING_OPERATOR;
    }
    fres_i = *((int*)fres);
    sprintf(buf, "%d", fres_i);
    strcpy(*result, buf);
    printf("Hello\n");
    return RPN_OK;
}

int rpn_eval_expr (const char *expr, char **result) {
   
    num_of_tokens = 0;

    int rc;
    char *tokens[255];
    char *expr_val;
    expr_val = (char*) malloc(sizeof(char)*strlen(expr)+1);
    if(expr_val == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(expr_val, expr);
    char *tmp;
    tmp = strtok(expr_val, " ");
    if(tmp && (tokens[num_of_tokens] = malloc(sizeof(char)*strlen(tmp)+1)) != NULL) {
        strcpy(tokens[num_of_tokens++], tmp);
    } 

    while(tmp != NULL){
        tmp = strtok(NULL, " ");
        if(tmp && (tokens[num_of_tokens] = malloc(sizeof(char)*strlen(tmp)+1)) != NULL) {
            strcpy(tokens[num_of_tokens++], tmp);      
        }
    }

    char *nresult = malloc(sizeof(char)*255);
    if(nresult == NULL){
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    rc = rpn_eval_token(tokens, &nresult);
    switch(rc) {
        case RPN_OK:
        strcpy(*result, nresult);
        break;
        case RPN_INVALID_TOKEN:
        strcpy(*result, "invalid token");
        break;
        case RPN_MISSING_OPERAND:
        strcpy(*result, "missing operand");
        break;
        case RPN_MISSING_OPERATOR:
        strcpy(*result, "missing operator");
        break;
        default:
        strcpy(*result, "arithmetic error");
    }
    free(nresult);
    free(expr_val);
    return rc;
}
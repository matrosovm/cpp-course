#ifndef _CALC_ERROR_H_INCLUDED__
#define _CALC_ERROR_H_INCLUDED__

#define EMPTY_STRING 1
#define ERR_BR_NUM 2 // wrong number of brackets
#define ERR_BR_CLOSE_OPEN 3 // if open bracket goes after close bracket
#define ERR_OP_NEAR 4 // two operands are neighbours
#define ERR_FU_UNDEF 5 // undefined function
#define ERR_FU_WRONG_ARG 6 // bad function arguments (e.g. sqrt(-1))
#define ERR_BR_OPER_CLOSE 7
#define ERR_CHAR_UNDEF 8
#define ERR_CONST_UNDEF 9
#define ERR_NO_OPERAND 10
#define ERR_NO_OPERATOR 11
#define ERR_CONST_NAME 12
#define ERR_CONST_RECURSION 13
#define ERR_BR_OPEN_CLOSE 14
#define ERR_NUMBER_NOTATION 15

#define ERR_DIV_ZERO 20 // division by zero
#define ERR_OVERFLOW 21 // too big number (1.#INF)
#define ERR_NAN 22

#define ERR_MEMORY_LACK 30

#define INFINITY 0x7FFFFFFFFFFFFFFF
#define ELDER_BYTE 0x8000000000000000

typedef double (*elementaryFunction)(double x);

int BadString(char *s);
int DeleteSpaces(char *s);
int IsBinOperator(char const c);
int IsLetter(char const c);
int IsDigit(char const c);
int IsOverFlow(double const x);
int IsDivisionZero(double const x);
int IsNan(double const x);
void SetError(int error);

int g_error; // here we will store errors we get on each string.

#endif // CALC_ERROR_H
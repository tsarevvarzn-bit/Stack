#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#define DEFAULT "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define VIOLET  "\033[1;35m"
#define CYAN    "\033[1;36m"

#define ISDEBUGMODE 1

//typedef double stack_elem_t;

const unsigned int MAX_COMMAND_LEN = 100;
const size_t       DEFAULT_STACK_SIZE = 5;

enum code_errors{
    correct,
    stack_data_is_NULL,
    stack_size_larger_than_capacity,
    stack_elem_is_poison,
    stack_unuse_elem_is_not_a_poison,
    stack_overflow,
    stack_underflow
};

struct stack_t{
    size_t  size;
    size_t  capacity;
    double* data;
};

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

//#define ISDEBUGMODE 1

//typedef double stack_elem_t;

const unsigned int MAX_COMMAND_LEN = 100;
const size_t       DEFAULT_STACK_SIZE = 5;

enum code_errors{

    correct,

    stack_data_is_NULL,             //Фатальные ошибки программиста
    stack_size_larger_than_capacity,
    stack_capacity_is_zero,

    memory_cannot_be_allocated_for_stack_expansion, //Не фатальные, должна быть обработана + взаимодействие с пользователем
    memory_cannot_be_allocated_for_stack_reducing,
    memory_cannot_be_allocated_to_create_a_stack    //Фатально для конкретного стэка
};

struct stack_t{

    size_t  size;
    size_t  capacity;
    double* data;
};

/*!SECTION

    safe_call(fuction)

    int error_code = correct;
    if(error_code = fuction) != correct){

        strerrno(errno);
        return error_code;
    }
*/

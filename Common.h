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

const unsigned int MAX_COMMAND_LEN    = 100;
const size_t       DEFAULT_STACK_SIZE = 5;
const size_t       CANARY         = 0xC000FEEE;

enum code_errors{

    correct,

    stack_data_is_NULL,             //Фатальные ошибки программиста
    stack_first_elem_p_is_null,
    stack_first_elem_p_is_incorrect,
    stack_size_larger_than_capacity,
    stack_capacity_is_zero,

    left_canary_in_stack_is_dead,
    right_canary_in_stack_is_dead,
    left_canary_in_data_is_dead,
    right_canary_in_data_is_dead,
    hash_is_incorrect,

    memory_cannot_be_allocated_for_stack_expansion, //Не фатальные, должна быть обработана + взаимодействие с пользователем
    memory_cannot_be_allocated_for_stack_reducing,
    memory_cannot_be_allocated_to_create_a_stack    //Фатально для конкретного стэка
};

#define IS_DEBUG 1
//TODO защита от взлома стэка канарейками и хэшом (два хэша: на данные и на сам стэк)

#define MY_ASSERT(str)                                                                                          \
                                                                                                                \
if(!(str)){                                                                                                     \
    printf("\nMy assertion failed: " #str ", file %s:%d, function: %s", __FILE_NAME__, __LINE__, __func__);     \
    abort();                                                                                                    \
}                                                                                                                               //TODO ending with ;


#if IS_DEBUG

    #define ON_DEBUG(...) __VA_ARGS__

#else

    #define ON_DEBUG(...)

#endif


#define CHECK_STACK(stack_p, comment) checkStack(stack_p, __FILE_NAME__, __LINE__, __func__, comment)


#define CHAR_TYPE 0
#define DOUBLE_TYPE 1

#define ELEM_TYPE CHAR_TYPE

#if ELEM_TYPE == CHAR_TYPE

    #define POISON '@'

    #define IS_POISON(elem) ((elem) == POISON)

    typedef char elem_type;

    #define PRINT_ELEM(x) printf("%c", x)

    #define SCAN_ELEM(x) getchar(); scanf("%c", x)

#elif ELEM_TYPE == DOUBLE_TYPE

    #define POISON NAN

    #define IS_POISON(elem) isnan(elem)

    typedef double elem_type;

    #define PRINT_ELEM(x) printf("%lg", x)

    #define SCAN_ELEM(x) scanf("%lg", x)

#endif


struct stack_t{

    size_t     canary_left;
    size_t     size;
    size_t     capacity;
    char*      data;
    elem_type* first_elem_p;
    size_t     canary_right;
};

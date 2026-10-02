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

#define IS_DEBUG 1
//TODO защита от взлома стэка канарейками и хэшом (два хэша: на данные и на сам стэк)
#if IS_DEBUG

    #define ON_DEBUG(...) __VA_ARGS__

    #define MY_ASSERT(str)                                                                                          \
                                                                                                                    \
    if(!(str)){                                                                                                     \
        printf("\nMy assertion failed: " #str ", file %s:%d, function: %s", __FILE_NAME__, __LINE__, __func__);     \
        abort();                                                                                                    \
    }                                                                                                                               //TODO ending with ;

#else

    #define ON_DEBUG(...)

    #define MY_ASSERT(str)

#endif


#define CHECK_STACK(stack_p, comment) checkStack(stack_p, __FILE_NAME__, __LINE__, __func__, comment)

enum elem_types{

    char_type = 0,
    double_type = 1
};

#define ELEM_TYPE 1

#if ELEM_TYPE == 0

    #define POISON '@'

    #define IS_POISON(elem) ((elem) == POISON)

    typedef char elem_type;

    #define PRINT_ELEM(x) printf("%c", x)

    #define SCAN_ELEM(x) getchar(); scanf("%c", x)

#elif ELEM_TYPE == 1

    #define POISON NAN

    #define IS_POISON(elem) isnan(elem)

    typedef double elem_type;

    #define PRINT_ELEM(x) printf("%lg", x)

    #define SCAN_ELEM(x) scanf("%lg", x)

#endif
struct stack_t{

    size_t  size;
    size_t  capacity;
    elem_type* data;
};


// enum PolicyType {
//     IntegerType,
//     DoubleType,
//     //...
// }

// #define STACK_ELEMENT_TYPE_POLICY IntegerType
//
// #if STACK_ELEMENT_TYPE == IntegerType
//     typedef int stack_elem_t
//     int PoisonValue = 0x666;
// #elif

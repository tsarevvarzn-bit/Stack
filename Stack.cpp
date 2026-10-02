#include "Common.h"

code_errors createStack(       stack_t** const stack_p, const size_t capacity);

void        printStack (const  stack_t* const stack_p);
code_errors stackPush  (       stack_t* const stack_p, const elem_type new_elem);
code_errors stackPop   (       stack_t* const stack_p, elem_type* const last_elem_p);
void        stackDelete(       stack_t* const stack_p);
code_errors verifyStack (const stack_t* const stack_p, const char* const call_info);

code_errors stackResizeUp(     stack_t* const stack_p);
code_errors stackResizeDown(   stack_t* const stack_p);

void        printBeautifulSpaces(size_t elem_index, size_t max_index);

void        checkStack(const stack_t* const stack_p, const char* const file_name, unsigned int line, const char* const func_name, const char* const comment);

int main(){

    stack_t* stack_p = NULL;

    int error_code = correct;

    if((error_code = createStack(&stack_p, DEFAULT_STACK_SIZE)) != correct){

        printf(RED "Can't create a stack, error:\n" DEFAULT);
        perror(RED "Error is fatal, \n Errno print: " DEFAULT);
        return errno;
    }

    printStack(stack_p);

    char command[MAX_COMMAND_LEN] = "";
    elem_type buffer = 0;

    while(strcmp(command, "end") != 0){

        scanf("%s", command);

        if(strcmp(command,"push") == 0){

            SCAN_ELEM(&buffer);
            stackPush(stack_p, buffer);
            printStack(stack_p);

        }else if(strcmp(command,"pop") == 0){

            stackPop(stack_p, &buffer);
            printf("Last element: " VIOLET);
            PRINT_ELEM(buffer);
            printf("\n" DEFAULT);
            printStack(stack_p);

        }
    }

    stackDelete(stack_p);
}

//TODO вопрос: если realloc() выдал указатель null, то мы можем пользоваться старым указателем, сохранились ли наши данные? - ДА, НУЖНО СОХРАНЯТЬ СТАРЫЙ УКАЗАТЕЛЬ
//TODO вопрос: если мы урезаем память, гарантируется ли что realloc не выдаст ноль - ДА, НО ПРОВЕРЯЕМ

/*
Ошибки, программиста, можем MY_ASSERT'ить:
Фатальные:
stack_data_is_NULL,
stack_size_larger_than_capacity,
stack_capacity_is_zero
Не фатальные:
stack_elem_is_poison,
stack_unuse_elem_is_not_a_poison,
Ошибки, которые могут вылезти при работе правильной программы (обрабатываем, не MY_ASSERT'им):
memory_cannot_be_allocated_for_stack_expansion,
memory_cannot_be_allocated_for_stack_reducing,
memory_cannot_be_allocated_to_create_a_stack

*/

code_errors createStack(stack_t** const stack_p_p, const size_t capacity){

    MY_ASSERT(stack_p_p)
    MY_ASSERT(capacity > 0)

    *stack_p_p = (stack_t*) calloc(1, sizeof(stack_t));

    if(*stack_p_p == NULL){

        return memory_cannot_be_allocated_to_create_a_stack;
    }


    (*stack_p_p)->data = (elem_type*) calloc(capacity, sizeof(double));

    if((*stack_p_p)->data == NULL){

        free(*stack_p_p);
        return memory_cannot_be_allocated_to_create_a_stack;
    }

    (*stack_p_p)->size = 0;
    (*stack_p_p)->capacity = capacity;

    for(size_t i = 0; i < capacity; i++){

        (*stack_p_p)->data[i] = POISON;
    }

    (*stack_p_p)->data[2] = 2;
    //(*stack_p_p)->size = 20;
    //(*stack_p_p)->capacity = 50;
    //(*stack_p_p)->data = NULL;


    CHECK_STACK((*stack_p_p), "check on exit");

    return correct;
}

void        printStack(const stack_t* const stack_p){

    MY_ASSERT(stack_p)
    MY_ASSERT(stack_p->data)

    printf("Printing of stack" CYAN ON_DEBUG(" [%p]") DEFAULT ":\n\n" DEFAULT ON_DEBUG(, stack_p));
    printf("\tsize      " CYAN ON_DEBUG("[%p]") DEFAULT ":" GREEN "%llu\n" DEFAULT, ON_DEBUG(&stack_p->size,) stack_p->size);
    printf("\tcapacity  " CYAN ON_DEBUG("[%p]") DEFAULT ":" YELLOW "%llu\n" DEFAULT, ON_DEBUG(&stack_p->capacity,) stack_p->capacity);
    printf("\tdata" CYAN ON_DEBUG("      [%p]") DEFAULT ":\n" DEFAULT ON_DEBUG(,stack_p->data));

    for(size_t i = 0; i < stack_p->capacity; i++){

        printf("\t");
        if(i < stack_p->size){
            printf(GREEN "*"  DEFAULT "[" YELLOW "%llu" DEFAULT "]" DEFAULT, i);
            printBeautifulSpaces(i, stack_p->capacity - 1);
            printf(" = " VIOLET);
            PRINT_ELEM(stack_p->data[i]);
            printf(DEFAULT);

            ON_DEBUG(
            if(IS_POISON(stack_p->data[i]))
                printf(YELLOW " - POISON" DEFAULT);
            )

            printf("\n");

        }else{
            printf(DEFAULT " [" DEFAULT "%llu" DEFAULT "]" DEFAULT, i);
            printBeautifulSpaces(i, stack_p->capacity - 1);
            printf(" = ");
            PRINT_ELEM(stack_p->data[i]);

            ON_DEBUG(
            if(!IS_POISON(stack_p->data[i]))
                printf(YELLOW " - NOT A POISON" DEFAULT);
            )

            printf("\n");
        }
    }

    MY_ASSERT(stack_p->data)
}

code_errors        stackPush(stack_t* const stack_p, const elem_type new_elem){

    MY_ASSERT(stack_p)
    CHECK_STACK(stack_p, "check on input");

    code_errors error_code = correct;

    if(stack_p->size == stack_p->capacity && (error_code = stackResizeUp(stack_p)) != correct){

        return error_code;
    }

    stack_p->data[stack_p->size++] = new_elem;

    CHECK_STACK(stack_p, "check on exit");

    return correct;
}

code_errors     stackPop(stack_t* const stack_p, elem_type* const last_elem_p){

    MY_ASSERT(stack_p)
    MY_ASSERT(last_elem_p)
    CHECK_STACK(stack_p, "check on input");

    if(stack_p->size != 0){

        *last_elem_p = stack_p->data[stack_p->size - 1];
        stack_p->data[stack_p->size - 1] = POISON;
        stack_p->size--;

        code_errors error_code = correct;

        if(stack_p->size * 4 <= stack_p->capacity && stack_p->capacity != 1 && (error_code = stackResizeDown(stack_p)) != correct){

            return error_code;
        }

    }else{

        printf("Stack " CYAN ON_DEBUG("[%p]") DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity error:\n" YELLOW
               "Stack underflow, there is no elements in stack\n" DEFAULT,
                ON_DEBUG(stack_p,) stack_p->size, stack_p->capacity);

        *last_elem_p = POISON;
    }

    CHECK_STACK(stack_p, "check on exit");

    return correct;
}

void        stackDelete(stack_t* const stack_p){

    MY_ASSERT(stack_p)
    CHECK_STACK(stack_p, "check on input");

    free(stack_p->data);
    free(stack_p);
}

code_errors verifyStack(const stack_t* const stack_p, const char* const call_info){ //Проверяет ошибки программиста, в билде не должен вызываться

    MY_ASSERT(stack_p)

    if(stack_p->data != NULL){//TODO канарейки и хэши

        if(stack_p->size <= stack_p->capacity){

            return correct;

        }else{

            printf(VIOLET "%s" DEFAULT ", stack" CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" RED
                   "size > capacity" YELLOW ", stack dump:\n" DEFAULT,
                    call_info, stack_p, stack_p->size, stack_p->capacity);

            printStack(stack_p);

            return stack_size_larger_than_capacity;
        }

    }else{

        printf(VIOLET "%s" DEFAULT ", stack" CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" RED
               "stack_p->data == NULL" YELLOW ", stack dump:\n" DEFAULT,
                call_info, stack_p, stack_p->size, stack_p->capacity);

        return stack_data_is_NULL;
    }
}

void        printBeautifulSpaces(size_t elem_index, size_t max_index){

    MY_ASSERT(elem_index <= max_index)

    int size_log = 0;

    while(elem_index >= 10){

        elem_index /= 10;
        size_log++;
    }

    while(max_index >= 10){

        max_index /= 10;
        size_log--;

        if(size_log < 0){
            putchar(' ');
        }
    }
}

code_errors stackResizeUp(stack_t* const stack_p){

    MY_ASSERT(stack_p)
    CHECK_STACK(stack_p, "check on input");

    void* new_pointer = realloc(stack_p->data, stack_p->capacity * sizeof(elem_type) * 2);

    if(new_pointer == NULL){

        printf("Stack " CYAN ON_DEBUG("[%p]") DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity error:\n" YELLOW
                "Stack overflow, memory cannot be allocated for stack expansion\n"
                ON_DEBUG("Errno output: %s"),
                ON_DEBUG(stack_p,) stack_p->size, stack_p->capacity ON_DEBUG(, strerror(errno)));

        CHECK_STACK(stack_p, "check on exit");

        return memory_cannot_be_allocated_for_stack_expansion;
    }

    stack_p->data = (elem_type*) new_pointer;
    stack_p->capacity *= 2;

    for(size_t i = stack_p->capacity / 2; i < stack_p->capacity; i++){

        stack_p->data[i] = POISON;
    }

    ON_DEBUG(printf("Stack " VIOLET "reallocate" DEFAULT ", new stack capacity: " YELLOW "%llu\n" DEFAULT, stack_p->capacity);)

    CHECK_STACK(stack_p, "check on exit");

    return correct;
}

code_errors stackResizeDown(stack_t* const stack_p){

    MY_ASSERT(stack_p)
    CHECK_STACK(stack_p, "check on input");


    void* new_pointer = realloc(stack_p->data, stack_p->capacity * sizeof(elem_type) / 2);

    if(new_pointer == NULL){

        printf("Stack " CYAN ON_DEBUG("[%p]") DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity error:\n" YELLOW
                "Memory cannot be allocated for stack reducing\n"
                ON_DEBUG("Errno output: %s"),
                ON_DEBUG(stack_p,) stack_p->size, stack_p->capacity ON_DEBUG(, strerror(errno)));

        CHECK_STACK(stack_p, "check on exit");

        return memory_cannot_be_allocated_for_stack_expansion;
    }

    stack_p->data = (elem_type*) new_pointer;
    stack_p->capacity /= 2;

    ON_DEBUG(printf("Stack " VIOLET "reallocate" DEFAULT ", new stack capacity: " YELLOW "%llu\n" DEFAULT, stack_p->capacity);)

    CHECK_STACK(stack_p, "check on exit");

    return correct;
}

void checkStack(const stack_t* const stack_p, const char* const file_name, unsigned int line, const char* const func_name, const char* const comment){

    MY_ASSERT(file_name)
    MY_ASSERT(func_name)
    MY_ASSERT(comment)

    char str[100] = "";
    sprintf(str, "Check of stack was called in %s:%d in function %s, comment: %s", file_name, line, func_name, comment);

    if(verifyStack(stack_p, str) != correct){

        abort();
    }
}

//TODO в коммандной строке можем работать сразу с несколькими стэками: создать стэк, удалить стэк, добавить элемент/убрать

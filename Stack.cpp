#include "Common.h"
//#include "Secure_shells.cpp"

code_errors createStack(      stack_t** const stack_p, const size_t capacity);

void        printStack (const stack_t* const stack_p);
code_errors        stackPush  (      stack_t* const stack_p, const double new_elem);
code_errors        stackPop   (      stack_t* const stack_p, double* const last_elem_p);
void        stackDelete(      stack_t* const stack_p);
code_errors checkStack (const stack_t* const stack_p);

void        printBeautifulSpaces(size_t elem_index, size_t max_index);

int main(){

    stack_t* stack_p = NULL;

    int error_code = correct;

    if((error_code = createStack(&stack_p, DEFAULT_STACK_SIZE)) != correct){

        printf(RED "Can't create a stack, error:\n" DEFAULT);
        perror(RED "Error is fatal, \n Errno print: " DEFAULT);
        return errno;
    }

    stack_p->data[2] = 2;

    printStack(stack_p);

    char command[MAX_COMMAND_LEN] = "";
    double buffer = 0;

    while(strcmp(command, "end") != 0){

        scanf("%s", command); //TODO safe input

        if(strcmp(command,"push") == 0){

            scanf("%lg", &buffer);
            stackPush(stack_p, buffer);
            printStack(stack_p);

        }else if(strcmp(command,"pop") == 0){

            stackPop(stack_p, &buffer);
            printf("Last element: " VIOLET "%lg\n" DEFAULT, buffer);
            printStack(stack_p);

        }
    }

    stackDelete(stack_p);
}

//TODO вопрос: если realloc() выдал указатель null, то мы можем пользоваться старым указателем, сохранились ли наши данные?
//TODO вопрос: если мы урезаем память, гарантируется ли что realloc не выдаст ноль

/*
Ошибки, программиста, можем assert'ить:
Фатальные:
stack_data_is_NULL,
stack_size_larger_than_capacity,
stack_capacity_is_zero
Не фатальные:
stack_elem_is_poison,
stack_unuse_elem_is_not_a_poison,
Ошибки, которые могут вылезти при работе правильной программы (обрабатываем, не assert'им):
memory_cannot_be_allocated_for_stack_expansion,
memory_cannot_be_allocated_for_stack_reducing,
memory_cannot_be_allocated_to_create_a_stack

*/

code_errors createStack(stack_t** const stack_p_p, const size_t capacity){

    assert(stack_p_p);
    assert(capacity > 0);

    *stack_p_p = (stack_t*) calloc(1, sizeof(stack_t));

    if(*stack_p_p == NULL){

        return memory_cannot_be_allocated_to_create_a_stack;
    }


    (*stack_p_p)->data = (double*) calloc(capacity, sizeof(double));

    if((*stack_p_p)->data == NULL){

        free(*stack_p_p);
        return memory_cannot_be_allocated_to_create_a_stack;
    }

    (*stack_p_p)->size = 0;
    (*stack_p_p)->capacity = capacity;

    for(size_t i = 0; i < capacity; i++){

        (*stack_p_p)->data[i] = NAN;
    }


    assert(checkStack(*stack_p_p) == correct);

    return correct;
}

void        printStack(const stack_t* const stack_p){

    assert(stack_p);
    assert(checkStack(stack_p) == correct);

    printf("Printing of stack " CYAN "[%p]\n\n" DEFAULT, stack_p);
    printf("\tsize      " CYAN "[%p]: " GREEN "%llu\n" DEFAULT, &stack_p->size, stack_p->size);
    printf("\tcapacity  " CYAN "[%p]: " YELLOW "%llu\n" DEFAULT, &stack_p->capacity, stack_p->capacity);
    printf("\tdata      " CYAN "[%p]:\n" DEFAULT, stack_p->data);

    for(size_t i = 0; i < stack_p->capacity; i++){

        printf("\t");
        if(i < stack_p->size){
            printf(GREEN "*"  DEFAULT "[" YELLOW "%llu" DEFAULT "]" DEFAULT, i);
            printBeautifulSpaces(i, stack_p->capacity - 1);
            printf(" = " VIOLET "%lg\n" DEFAULT, stack_p->data[i]);
        }else{
            printf(DEFAULT " [" DEFAULT "%llu" DEFAULT "]" DEFAULT, i);
            printBeautifulSpaces(i, stack_p->capacity - 1);
            printf(" = %lg\n", stack_p->data[i]);
        }
    }

    assert(checkStack(stack_p) == correct);
}

code_errors        stackPush(stack_t* const stack_p, const double new_elem){

    assert(stack_p);
    assert(checkStack(stack_p) == correct);

    if(stack_p->size == stack_p->capacity){

        void* new_pointer = realloc(stack_p->data, stack_p->capacity * sizeof(double) * 2);//TODO не падать, обрабатывать не расширение как warning

        if(new_pointer == NULL){

            printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity error:\n" YELLOW
                   "Stack overflow, memory cannot be allocated for stack expansion\n"
                   "Errno output: %s",
                    stack_p, stack_p->size, stack_p->capacity, strerror(errno));

            assert(checkStack(stack_p) == correct);

            return memory_cannot_be_allocated_for_stack_expansion;
        }

        stack_p->data = (double*) new_pointer;
        stack_p->capacity *= 2;

        for(size_t i = stack_p->capacity / 2; i < stack_p->capacity; i++){

            stack_p->data[i] = NAN;
        }

        printf("Stack " VIOLET "reallocate" DEFAULT ", new stack capacity: " YELLOW "%llu\n" DEFAULT, stack_p->capacity);
    }

    stack_p->data[stack_p->size++] = new_elem;

    assert(checkStack(stack_p) == correct);

    return correct;
}

code_errors     stackPop(stack_t* const stack_p, double* const last_elem_p){

    assert(stack_p);
    assert(last_elem_p);
    assert(checkStack(stack_p) == correct);

    if(stack_p->size != 0){

        *last_elem_p = stack_p->data[stack_p->size - 1];

        stack_p->data[stack_p->size - 1] = NAN;

        stack_p->size--;

        if(stack_p->size*4 <= stack_p->capacity && stack_p->capacity != 1){

            void* new_pointer = realloc(stack_p->data, stack_p->capacity * sizeof(double) / 2);

            if(new_pointer == NULL){

                printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity error:\n" YELLOW
                       "Memory cannot be allocated for stack reducing\n"
                       "Errno output: %s",
                        stack_p, stack_p->size, stack_p->capacity, strerror(errno));

                assert(checkStack(stack_p) == correct);

                return memory_cannot_be_allocated_for_stack_expansion;
            }

            stack_p->data = (double*) new_pointer;
            stack_p->capacity /= 2;

            printf("Stack " VIOLET "reallocate" DEFAULT ", new stack capacity: " YELLOW "%llu\n" DEFAULT, stack_p->capacity);
        }

    }else{

        printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity error:\n" YELLOW
               "Stack underflow, there is no elements in stack\n" DEFAULT,
                stack_p, stack_p->size, stack_p->capacity);

        *last_elem_p = NAN;
    }

    return correct;
}

void        stackDelete(stack_t* const stack_p){

    assert(stack_p);
    assert(checkStack(stack_p) == correct);

    free(stack_p->data);
    free(stack_p);
}

code_errors checkStack(const stack_t* const stack_p){

    assert(stack_p);

    if(stack_p->data != NULL){

        if(stack_p->size <= stack_p->capacity){

            for(size_t i = 0; i < stack_p->capacity; i++){

                if(i < stack_p->size && isnan(stack_p->data[i])){

                    printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" YELLOW
                           "Element %llu: %lg is NAN\n" DEFAULT,
                            stack_p, stack_p->size, stack_p->capacity, i, stack_p->data[i]);

                }else if(i >= stack_p->size && ! isnan(stack_p->data[i])){

                    printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" YELLOW
                           "Element %llu: %lg is not a nan\n" DEFAULT,
                            stack_p, stack_p->size, stack_p->capacity, i, stack_p->data[i]);

                }
            }

            return correct;

        }else{

            printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" YELLOW
                   "size > capacity\n" DEFAULT,
                    stack_p, stack_p->size, stack_p->capacity);

            return stack_size_larger_than_capacity;
        }

    }else{

        printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" YELLOW
           "stack_p->data == NULL\n" DEFAULT,
            stack_p, stack_p->size, stack_p->capacity);

        return stack_data_is_NULL;
    }
}

void        printBeautifulSpaces(size_t elem_index, size_t max_index){

    assert(elem_index <= max_index);

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


//TODO в коммандной строке можем работать сразу с несколькими стэками: создать стэк, удалить стэк, добавить элемент/убрать

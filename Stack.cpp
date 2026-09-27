#include "Common.h"
#include "Secure_shells.cpp"

stack_t*    createStack(const size_t capacity);
void        printStack(const stack_t* const stack_p);
void        stackPush(stack_t* const stack_p, const double new_elem);
void        stackPop(stack_t* const stack_p, double* const last_elem_p);
void        stackDelete(stack_t* const stack_p);
code_errors checkStack(const stack_t* const stack_p);

void        printBeautifulSpaces(size_t elem_index, size_t max_index);

int main(){

    stack_t* stack_p = createStack(DEFAULT_STACK_SIZE);
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

stack_t*    createStack(const size_t capacity){

    stack_t* stack_p = (stack_t*) safeCalloc(1, sizeof(stack_t));
    stack_p->data = (double*) safeCalloc(capacity, sizeof(double));
    stack_p->size = 0;
    stack_p->capacity = capacity;

    for(size_t i = 0; i < capacity; i++){

        stack_p->data[i] = NAN;
    }

    assert(checkStack(stack_p) == correct);

    return stack_p;
}

void        printStack(const stack_t* const stack_p){

    assert(stack_p);

    assert(checkStack(stack_p) == correct);

    printf("Printing of stack " CYAN "[%p]\n\n" DEFAULT, stack_p);
    printf("\tSize      " CYAN "[%p]: " GREEN "%llu\n" DEFAULT, &stack_p->size, stack_p->size);
    printf("\tCapacity  " CYAN "[%p]: " YELLOW "%llu\n" DEFAULT, &stack_p->capacity, stack_p->capacity);
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
}

void        stackPush(stack_t* const stack_p, const double new_elem){

    assert(stack_p);

    assert(checkStack(stack_p) == correct);

    if(stack_p->size == stack_p->capacity){

        stack_p->capacity *= 2;
        stack_p->data = (double*) safeRealloc(stack_p->data, stack_p->capacity * sizeof(double));//TODO не падать, обрабатывать не расширение как warning

        for(size_t i = stack_p->capacity / 2; i < stack_p->capacity; i++){

            stack_p->data[i] = NAN;
        }

        printf("Stack " VIOLET "reallocate" DEFAULT ", new stack capacity: " YELLOW "%llu\n" DEFAULT, stack_p->capacity);
    }

    stack_p->data[stack_p->size++] = new_elem;
}

void        stackPop(stack_t* const stack_p, double* const last_elem_p){

    assert(stack_p);
    assert(last_elem_p);

    assert(checkStack(stack_p) == correct);

    if(stack_p->size != 0){

        *last_elem_p = stack_p->data[stack_p->size - 1];

        stack_p->data[stack_p->size - 1] = NAN;

        stack_p->size--;

        if(stack_p->size*4 <= stack_p->capacity && stack_p->capacity != 1){

            stack_p->capacity /= 2;
            stack_p->data = (double*) safeRealloc(stack_p->data, stack_p->capacity * sizeof(double));

            printf("Stack " VIOLET "reallocate" DEFAULT ", new stack capacity: " YELLOW "%llu\n" DEFAULT, stack_p->capacity);
        }

    }else{

        printf(RED "ERROR: there is no elements in stack!\n" DEFAULT);
        *last_elem_p = NAN;
    }

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

                    return stack_elem_is_poison;

                }else if(i >= stack_p->size && ! isnan(stack_p->data[i])){

                    printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" YELLOW
                           "Element %llu: %lg is not a nan\n" DEFAULT,
                            stack_p, stack_p->size, stack_p->capacity, i, stack_p->data[i]);

                    return stack_unuse_elem_is_not_a_poison;
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

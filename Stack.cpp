#include "Common.h"
#include "Secure_shells.cpp"

stack_t* createStack(size_t capacity);
void     printStack(stack_t* stack_p);
void     stackPush(stack_t* stack_p, double new_elem);
void     stackPop(stack_t* stack_p, double* last_elem_p);
void     stackDelete(stack_t* stack_p);
bool     isStackCorrect(stack_t* stack_p);

int main(){

    stack_t* stack_p = createStack(5);
    printStack(stack_p);

    char command[100] = "";
    double buffer = 0;

    while(strcmp(command, "end") != 0){

        scanf("%s", command);

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

stack_t* createStack(size_t capacity){

    stack_t* stack_p = (stack_t*) safeCalloc(1, sizeof(stack_t));
    stack_p->data = (double*) safeCalloc(capacity, sizeof(double));
    stack_p->size = 0;
    stack_p->capacity = capacity;

    for(size_t i = 0; i < capacity; i++){

        stack_p->data[i] = NAN;
    }

    return stack_p;
}

void     printStack(stack_t* stack_p){

    assert(stack_p);

    assert(isStackCorrect(stack_p));

    printf("Printing of stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity:\n" DEFAULT,
            stack_p, stack_p->size, stack_p->capacity);

    for(size_t i = 0; i < stack_p->capacity; i++){

        printf("\t");
        if(i < stack_p->size){
            printf(GREEN "*"  DEFAULT "[" YELLOW "%llu" DEFAULT "]" DEFAULT " = " VIOLET "%lg\n" DEFAULT, i, stack_p->data[i]);
        }else{
            printf(DEFAULT " [" DEFAULT "%llu" DEFAULT "]" DEFAULT " = %lg\n", i, stack_p->data[i]);
        }
    }
}

void     stackPush(stack_t* stack_p, double new_elem){

    assert(stack_p);

    assert(isStackCorrect(stack_p));

    if(stack_p->size == stack_p->capacity){

        stack_p->capacity *= 2;
        stack_p->data = (double*) safeRealloc(stack_p->data, stack_p->capacity * sizeof(double));

        for(size_t i = stack_p->capacity / 2; i < stack_p->capacity; i++){

            stack_p->data[i] = NAN;
        }

        printf("Stack " VIOLET "reallocate" DEFAULT ", new stack capacity: " YELLOW "%llu\n" DEFAULT, stack_p->capacity);
    }

    stack_p->data[stack_p->size++] = new_elem;
}

void     stackPop(stack_t* stack_p, double* last_elem_p){

    assert(stack_p);
    assert(last_elem_p);

    assert(isStackCorrect(stack_p));

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

void     stackDelete(stack_t* stack_p){

    assert(stack_p);

    assert(isStackCorrect(stack_p));

    free(stack_p->data);
    free(stack_p);
}

bool     isStackCorrect(stack_t* stack_p){

    assert(stack_p);

    if(stack_p->data != NULL && stack_p->size <= stack_p->capacity){

        for(size_t i = 0; i < stack_p->capacity; i++){

            if(i < stack_p->size && isnan(stack_p->data[i])){

                printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" YELLOW
                       "Element %llu: %lg is NAN\n" DEFAULT,
                        stack_p, stack_p->size, stack_p->capacity, i, stack_p->data[i]);

                return false;

            }else if(i >= stack_p->size && ! isnan(stack_p->data[i])){

                printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" YELLOW
                       "Element %llu: %lg is not a nan\n" DEFAULT,
                        stack_p, stack_p->size, stack_p->capacity, i, stack_p->data[i]);

                return false;
            }
        }

        return true;
    }

    printf("Stack " CYAN "[%p]" DEFAULT " with " GREEN "%llu" DEFAULT " size and " YELLOW "%llu" DEFAULT " capacity is incorrect:\n" YELLOW
           "stack_p->data == NULL or size > capacity\n" DEFAULT,
            stack_p, stack_p->size, stack_p->capacity);

    return false;

}
//TODO в коммандной строке можем работать сразу с несколькими стэками: создать стэк, удалить стэк, добавить элемент/убрать

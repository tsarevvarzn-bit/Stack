#include "Common.h"

void printBeautifulSpaces(size_t capacity);

int main(){

    printf("<");
    printBeautifulSpaces(5);
    printf(">");

}

void printBeautifulSpaces(size_t capacity){

    while(capacity >= 10){

        capacity /= 10;
        putchar(' ');
    }
}

void* safeCalloc( const size_t number_of_elements, const size_t size_of_element);
void* safeRealloc(void* const old_pointer, const size_t new_size);



void*  safeCalloc(const size_t number_of_elements, const size_t size_of_element){

    void* pointer = calloc(number_of_elements, size_of_element);

    if(pointer == NULL){

        printf(RED "ERROR: calloc(number_of_elements, size_of_element), number of elements: %zu, size of one element: %zu, total: %zu, failed: %s" DEFAULT,
               number_of_elements, size_of_element, number_of_elements * size_of_element, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return pointer;
}

void*  safeRealloc(void* const old_pointer, const size_t new_size){

    assert(old_pointer);

    void* new_pointer = realloc(old_pointer, new_size);

    if(new_pointer == NULL){

        printf(RED "ERROR: realloc(old_pointer, new_size), old pointer: %p, new size %zu, failed: %s" DEFAULT,
               old_pointer, new_size, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return new_pointer;
}

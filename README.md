# Стэк

Реализация структуры данных стэк, основанной на принципе LIFO (Last In First Out) на языке C в виде библиотеки (набор функций для создания, использования и удаления стэков), main написан для тестирования этого набора функций.

# Что делает и как пользоваться

Основные для использования пользователем функции содержаться в главном файле Stack.cpp:
*code_errors createAndInitStack(stack_t** const stack_p_p, const size_t capacity) - выделяет динмически память под стэк размера capacity, 
вызывает функцию-инициализатор и кладет указатель на созданный стэк по адресу stack_p_p, возвращает код ошибки
*void stackInit(stack_t* const stack_p, const size_t capacity) - инициализирует стэк, лежащий по укащателю stack_p размера capacity, вызывается функцией createAndInitStack(), возвращает код ошибки
*code_errors stackPush(stack_t* const stack_p, const elem_type new_elem) - пушит в стэк по указателю stack_p элемент new_elem, при необходимости динамически расширяет стэк, возвращает код ошибки
*code_errors stackPop(stack_t* const stack_p, elem_type* const last_elem_p) - попит из стэка по указателю stack_p элемент, кладет его по указателю last_elem_p, если элементов нет печатает warning и выдает poison, выдает код ошибки
*void stackDelete(stack_t* const stack_p) - освобождает динамическую память, выделенную для стэка по указателю stack_p
*code_errors stackResizeUp(stack_t* const stack_p) - увеличивает емкость стэка в два раза, автоматически вызывается stackPush(), возвращает код ошибки
*code_errors stackResizeDown(stack_t* const stack_p) - уменьшает емкость стэка в два раза, автоматически вызывается stackPop(), возвращает код ошибки

## Структура проекта
Проект разделен на две папки с кодом и дополнительные файлы
```text
Onegin/
├── Common.h                           #Инклюд стандартных библиотек, общие константы, enum коды ошибок
├── DebugDefinition.h                  #Макросы для включения/выключения различных проверок и защит
├── TypeDefinition.h                   #Макросы для определения типа элемента стэка
├── Stack.cpp
├── InputOutput.cpp
├── Protect.cpp
├── .gitattributes                     # Настройки Git
├── .gitignore                         # Файлы и папки, игнорируемые Git
└── README.md                          # Документация проекта
```

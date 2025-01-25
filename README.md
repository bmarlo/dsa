## Usage

```C
#include "marlo/hash_table.h"
#include <stdio.h>

void root_handler(const char* s);
void foo_handler(const char* s);
void bar_handler(const char* s);
void foobar_handler(const char* s);

typedef void (*handler_t)(const char* s);

typedef struct container_t {
    const char* path;
    handler_t handler;
} container_t;

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("usage: <path> <string>\n");
        return 0;
    }

    hash_table_t* table = hash_table_new(HASH_TABLE_STRING, 0);
    if (table == NULL) {
        printf("`hash_table_new()` error\n");
        return 0;
    }

    container_t root;
    root.path = "/";
    root.handler = root_handler;

    container_t foo;
    foo.path = "/foo";
    foo.handler = foo_handler;

    container_t bar;
    bar.path = "/bar";
    bar.handler = bar_handler;

    container_t foobar;
    foobar.path = "/foobar";
    foobar.handler = foobar_handler;

    int error = 0;
    error |= hash_table_push(table, root.path, &root);
    error |= hash_table_push(table, foo.path, &foo);
    error |= hash_table_push(table, bar.path, &bar);
    error |= hash_table_push(table, foobar.path, &foobar);

    if (error == -1) {
        printf("`hash_table_push()` error\n");
        hash_table_release(table);
        return 0;
    }

    container_t* result = (container_t*) hash_table_at(table, argv[1]);
    if (result == NULL) {
        printf("handler not found, available handlers are:\n");
        hash_table_iterator_t iter = hash_table_begin(table);
        while (hash_table_is_valid(iter)) {
            hash_table_item_t item = hash_table_item(iter);
            container_t* container = (container_t*) item.value;
            printf("%s\n", container->path);
            iter = hash_table_next(iter);
        }

        hash_table_release(table);
        return 0;
    }

    result->handler(argv[2]);
    hash_table_release(table);
    return 0;
}

void root_handler(const char* s)
{
    printf("/ -> %s\n", s);
}

void foo_handler(const char* s)
{
    printf("/foo -> %s\n", s);
}

void bar_handler(const char* s)
{
    printf("/bar -> %s\n", s);
}

void foobar_handler(const char* s)
{
    printf("/foobar -> %s\n", s);
}
```

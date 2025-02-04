cmake_minimum_required(VERSION 3.10)

project(dsa)

function(c11 target)
    set_target_properties(${target}
        PROPERTIES
            C_STANDARD 11
            C_STANDARD_REQUIRED ON
            C_EXTENSIONS OFF
    )

    target_compile_options(${target}
        PRIVATE
            -Wall -Wextra -Wconversion -pedantic -Wno-sign-conversion
    )
endfunction()

add_library(dsa STATIC
    src/binary_heap.c
    src/binary_tree.c
    src/deque.c
    src/hash_set.c
    src/hash_table.c
    src/linked_list.c
    src/queue.c
    src/stack.c
    src/string.c
    src/vector.c
)

c11(dsa)
target_include_directories(dsa PUBLIC include)

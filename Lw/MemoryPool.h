#pragma once

#define memory_block_allocation(returned_type_pointer, size_block_memory) (returned_type_pointer*)malloc(size_block_memory)
#define memory_block_allocation_void(size_block_memory) malloc(size_block_memory)
#define free_block_memory(pointer_on_block_memory) free(pointer_on_block_memory)
#define memory_initialization(pointer_on_block_memory, constructor_obj) new(pointer_on_block_memory)constructor_obj
#define destructor(pointer_on_block_memory, constructor_obj) (pointer_on_block_memory)->~constructor_obj()

#define pointer_cast(pointer, type_cast) (reinterpret_cast<type_cast*>(pointer))
#define r_pointer_cast(pointer, type_cast) *(reinterpret_cast<type_cast*>(pointer))

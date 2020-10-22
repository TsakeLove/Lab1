# Lab1
The class allocator which can select data in heap, with size that you say. Memory is divided into blocks, every block contains **header** and memory. header has fixed size 8 bytes. That mean if need to allocate 8 bytes of memory actually needs 16 bytes free memory in one block. 


## Functions:

`void* memory_alloc(size_t size)` – search a free block with enough memory. When such block is found we create a block of given size and return pointer to the memory you can use. If there is more memory in the block that we need devide it on 2 blocks. If there is no block with enough memory return nullptr.

`void memory_free(void* adr)` – mark this block as free. 

`void unite(void* adr)` - united adjacent blocks, if they are free, with current block.

`void* memory_realloc(void* adr, size_t size)` – change size of chosen block, can make it smaller or bigger. If not enough memory in this block with free adjacent blocks then works as memory_alloc. If there not enough memory for moving of old block returns nullptr. If change of size successful returns pointer to the new block  and frees old block. 

## Examples:

#### Allocator creation
##### Code:
```c++
Allocator allocator;
allocator.init(130);
allocator.memory_dump();
```
##### Result:
```
*BLOCK*
USED:  0
SIZE:  122
SIZE OF PREVIOUS: 0`
```

#### Memory allocation
##### Code:
```c++
Allocator allocator;
allocator.init(130);
void* adr1 = allocator.memory_alloc(10);
void* adr2 = allocator.memory_alloc(10);
void* adr3 = allocator.memory_alloc(20);
void* adr4 = allocator.memory_alloc(10);
allocator.memory_dump();
```
##### Result:
```
*BLOCK*
USED:  1
SIZE:  12
SIZE OF PREVIOUS: 0

*BLOCK*
USED:  1
SIZE:  12
SIZE OF PREVIOUS: 12

*BLOCK*
USED:  1
SIZE:  20
SIZE OF PREVIOUS: 12

*BLOCK*
USED:  1
SIZE:  12
SIZE OF PREVIOUS: 20

*BLOCK*
USED:  0
SIZE:  34
SIZE OF PREVIOUS: 12
```

#### Memory free
##### Code:
```c++
Allocator allocator;
allocator.init(130);
void* adr1 = allocator.memory_alloc(10);
void* adr2 = allocator.memory_alloc(10);
void* adr3 = allocator.memory_alloc(20);
void* adr4 = allocator.memory_alloc(10);
allocator.memory_free(adr3);
allocator.memory_free(adr2);
allocator.memory_free(adr4);
allocator.memory_dump();
```
##### Result:
```
*BLOCK*
USED:  1
SIZE:  12
SIZE OF PREVIOUS: 0

*BLOCK*
USED:  0
SIZE:  102
SIZE OF PREVIOUS: 12
```

#### Memory reallocation
##### Code:
```c++
Allocator allocator;
allocator.init(130);
void* adr1 = allocator.memory_alloc(10);
void* adr2 = allocator.memory_alloc(10);
void* adr3 = allocator.memory_alloc(20);
void* adr4 = allocator.memory_alloc(10);
allocator.memory_free(adr3);
allocator.memory_realloc(adr2, 12);
allocator.memory_dump();
```
##### Result:
```
*BLOCK*
USED:  1
SIZE:  12
SIZE OF PREVIOUS: 0

*BLOCK*
USED:  1
SIZE:  12
SIZE OF PREVIOUS: 12

*BLOCK*
USED:  0
SIZE:  20
SIZE OF PREVIOUS: 12

*BLOCK*
USED:  1
SIZE:  12
SIZE OF PREVIOUS: 20

*BLOCK*
USED:  0
SIZE:  34
SIZE OF PREVIOUS: 12
```

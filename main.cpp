#include <iostream>
using namespace std;

struct Header{
    uint8_t used;
    uint16_t size;
    uint16_t previous_size;
};

class Allocator{
public:
    uint8_t* start;
    uint8_t* finish;
    size_t size;
    void init(size_t size);
    void* memory_alloc(size_t size);
    void* memory_realloc(void* adr, size_t size);
    void memory_free(void *adr);
    void create_header(void* start, uint8_t used, uint16_t size, uint16_t previous_size);
    void memory_dump();
    void unite(void* adr);
    size_t align(size_t size);
};

void Allocator::create_header(void* start, uint8_t used, uint16_t size, uint16_t previous_size){
    Header* header = (Header*) start;
    header->used = used;
    header->size = size;
    header->previous_size = previous_size;
}

void Allocator::init(size_t size) {
    start = (uint8_t *)malloc(size);
    finish = start + size - 1;
    this->size = size;
    create_header(start, 0, size - 8, 0);
}

void* Allocator::memory_alloc(size_t size) {
    Header *header = (Header *) start;
    size_t inbound = 8 + header->size;
    size_t previous_size = 0;
    size_t current_size = header->size;
    while (true) {
        if ((int) header->used == 0 && header->size - 12 >= align(size)) {
            create_header(header, 1, align(size), previous_size);
            create_header((uint8_t*) header + 8 + align(size), 0, current_size - 8 - align(size), align(size));
            return (uint8_t**)((uint8_t*) header + 8);
        }
        else {
            if (inbound < this->size) {
                previous_size = header->size;
                header = (Header *) ((uint8_t *) header + 8 + (int) header->size);
                current_size = header->size;
                inbound = inbound + 8 + header->size;
                if (inbound > this->size) return NULL;
            }
            else return NULL;
        }
    }
}

void* Allocator::memory_realloc(void* adr, size_t size){
    if (adr == NULL){
        return memory_alloc(size);
    } else{
        uint8_t * start = (uint8_t*) adr;
        Header* currentHeader = (Header*)(start - 8);
        Header* nextHeader = (Header*)(start + currentHeader->size);
        if (nextHeader->used == 0 && align(size) < currentHeader->size + nextHeader->size - 4){
            Header* next_next_header = (Header*)((uint8_t*)nextHeader + 8 + nextHeader->size);
            uint16_t size_next_header = nextHeader->size - (align(size) - currentHeader->size);
            currentHeader->size = align(size);
            nextHeader = (Header*)((uint8_t*)start + align(size));
            create_header(nextHeader, 0, size_next_header, align(size));
            if ((uint8_t*)next_next_header < finish) {
                next_next_header->previous_size = nextHeader->size;
            }
        }
        else {
            void *result = memory_alloc(size);
            if (result != NULL) {
                memory_free(adr);
            }
            return result;
        }
    }
    return nullptr;
}

void Allocator::memory_free(void *adr) {
    Header* header = (Header*)((uint8_t*)adr - 8);
    header->used = 0;
    unite(adr);
}

void Allocator::memory_dump() {
    Header *header = (Header *) start;
    size_t inbound = 8 + header->size;
    while (true) {
        if (inbound <= this->size) {
            cout << "\n*BLOCK*" << endl;
            cout << "USED:  " << (int)header->used << endl;
            cout << "SIZE:  " << header->size << endl;
            cout << "SIZE OF PREVIOUS: " << header->previous_size << endl;
            if (inbound == this->size){
                break;
            } else {
                uint8_t* p = (uint8_t*)header;
                header = (Header*)(p + 8 + header->size);
                inbound = inbound + 8 + header->size;
            }
        } else break;
    }
}

void Allocator::unite(void* adr) {
    uint8_t *start = (uint8_t *) adr;
    Header *current_header = (Header *) (start - 8);
    Header *previous_header = NULL;
    Header *next_header = (Header*)(((uint8_t *) current_header) + current_header->size + 8);
    if (current_header > (Header *) (uint8_t **) this->start) {
        previous_header = (Header *) (start - 16 - current_header->previous_size);
        if (previous_header->used == 0) {
            previous_header->size = previous_header->size + 8 + current_header->size;
            current_header = previous_header;
            next_header->previous_size = current_header->size;
        }
    }
    if (next_header->used == 0) {
        Header* next_next_header = (Header*)(((uint8_t*)next_header) + 8 + next_header->size);
        current_header->size = current_header->size + 8 + next_header->size;
        if ((uint8_t*)next_next_header < this->finish){
            next_next_header->previous_size = current_header->size;
        }
    }
}

size_t Allocator::align(size_t size) {
    if (size % 4 == 0){
        return size;
    } else{
        return size + (4 - (size % 4));
    }
}

int main() {
    Allocator allocator;
    allocator.init(130);
    void* adr1 = allocator.memory_alloc(10);
    void* adr2 = allocator.memory_alloc(8);
    void* adr3 = allocator.memory_alloc(30);
    void* adr4 = allocator.memory_alloc(10);
    allocator.memory_free(adr3);
    allocator.memory_realloc(adr2, 8);
    allocator.memory_dump();
    return 0;
}
//
// Created by Игорь on 18.10.2020.
//


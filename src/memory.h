#pragma once
#include <cstdint>

template<typename T>
T read(uintptr_t addr) {
    return *(T*)addr;
}

template<typename T>
void write(uintptr_t addr, T value) {
    *(T*)addr = value;
}

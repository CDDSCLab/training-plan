//
// Created by IcePig on 2020-07-27.
//

#ifndef HASH_H
#define HASH_H
#include <cstddef>
#include <cstdint>
#include <string>
#include "coding.h"

uint32_t Hash(const char* data, size_t n, uint32_t seed);
#endif //HASH_H

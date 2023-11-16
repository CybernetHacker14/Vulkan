#ifndef COMMON_H
#define COMMON_H

#ifdef _MSC_VER
    #pragma once
#endif  // _MSC_VER

typedef struct optional_uint {
    unsigned short flag;
    unsigned int data;
} optional;

#endif  // !COMMON_H

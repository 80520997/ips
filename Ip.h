

#pragma once
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef unsigned char byte;
typedef uint32_t uint;


class Ip {


public:
    static int init(const char *ipdb);

    static std::string find(const char *ip);

private:
    static byte *ipip_data;
    static byte *ipip_index;
    static uint *ipip_flag;
    static uint ipip_offset;
};
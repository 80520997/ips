
#include "Ip.h"

#define B2IL(b) (((b)[0] & 0xFF) | (((b)[1] << 8) & 0xFF00) | (((b)[2] << 16) & 0xFF0000) | (((b)[3] << 24) & 0xFF000000))
#define B2IU(b) (((b)[3] & 0xFF) | (((b)[2] << 8) & 0xFF00) | (((b)[1] << 16) & 0xFF0000) | (((b)[0] << 24) & 0xFF000000))

int Ip::init(const char *ipdb) {
    if (ipip_offset) {
        return 0;
    }
    FILE *file = NULL;

    if((file = fopen(ipdb, "rb")) == NULL) {
        printf("open fail  %s \n", ipdb);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    ipip_data = (byte *) malloc(size * sizeof(byte));
    size_t r = fread(ipip_data, sizeof(byte), (size_t) size, file);

    if (r == 0) {
        return 0;
    }

    fclose(file);

    uint length = B2IU(ipip_data);

    int index_size = (length-262148-262144);
    ipip_index = (byte *) malloc(index_size * sizeof(byte));
    memcpy(ipip_index, ipip_data + 4, index_size);
    ipip_offset = length;

    ipip_flag = (uint *) malloc(65536 * sizeof(uint));
    memcpy(ipip_flag, ipip_index, 65536 * sizeof(uint));

    return 0;
}


std::string Ip::find(const char *ip) {

    char result[8192];
    std::string addr;
    uint ips[4];
    int num = sscanf(ip, "%d.%d.%d.%d", &ips[0], &ips[1], &ips[2], &ips[3]);

    if (num == 4) {
        uint ip_prefix_value = ips[0] * 256 + ips[1];
        uint ip2long_value = B2IU(ips);
        uint start = ipip_flag[ip_prefix_value];
        uint max_comp_len = ipip_offset - 262148-262144 - 4;
        uint index_offset = 0;
        uint index_length = 0;
        for (start = start * 9 + 262144; start < max_comp_len; start += 9) {
            if (B2IU(ipip_index + start) >= ip2long_value) {
                index_offset = B2IL(ipip_index + start + 4) & 0x00FFFFFF;
                index_length = (ipip_index[start + 7] << 8) + ipip_index[start + 8];
                break;
            }
        }
        memcpy(result, ipip_data + ipip_offset + index_offset - 262144, index_length);
        addr.append((char *)ipip_data + ipip_offset + index_offset - 262144, index_length);
    }

    if(!addr.length()) {
        addr = "undefined";
    }

    return addr;
}

byte *Ip::ipip_data = 0;
byte *Ip::ipip_index = 0;
uint *Ip::ipip_flag = 0;
uint Ip::ipip_offset = 0;
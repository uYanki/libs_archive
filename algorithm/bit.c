// 实现:对字节的比特位的进行读写

#include <stdio.h>

// 输出二进制
void print_binary(unsigned int number) {
    if (number >> 1) print_binary(number >> 1);
    putc((number & 1) ? '1' : '0', stdout);
}

typedef unsigned char uint8_t;
typedef unsigned int  uint32_t;

// 从最高位开始寻找首个不为0的比特位
#define elem_t uint8_t
int first_bit_pos(elem_t byte) {
    uint32_t len = sizeof(elem_t) * 8;
    while (len)
        if (byte & (1 << (--len)))  //
            return len;
    return -1;
}

// pos<8
uint8_t set_bit(uint8_t byte, uint8_t pos, uint8_t bit) { byte = (bit != 0) ? (byte | 1 << pos) : (byte & ~(1 << pos)); }
uint8_t get_bit(uint8_t byte, uint8_t pos) { return (byte >> pos) & 0x1; }

// len+pos<8
uint8_t clear_bits(uint8_t byte, uint8_t pos, uint8_t len) { return byte & ((0xff << (pos + len + 1)) | (0xff >> (8 - pos))); }
uint8_t set_bits(uint8_t byte, uint8_t pos, uint8_t bits, uint8_t len) { return clear_bits(byte, pos, len) | (bits << pos); }
uint8_t get_bits(uint8_t byte, uint8_t pos, uint8_t len) { return (byte >> pos) & (0xff) >> (8 - len); }

// 大小写转换
char upper(char c) { return (c >= 'A' && c <= 'Z') ? (c) : (c & ~0x20); }
char lower(char c) { return (c >= 'a' && c <= 'z') ? (c) : (c | 0x20); }

int main() {
    int val = 213;

    printf("76543210 (bit)\n");

    print_binary(val);
    printf(" (origin)\n");

    // print_binary(set_bit(val, 4, 0));
    // printf(" (set_bit 4->0)\n");

    // printf("%d (get_bit from origin)\n", get_bit(val, 4));

    uint8_t bits = 4;
    uint8_t pos  = 3;
    // printf(" (pos:%d)\n", pos);
    // print_binary(bits);
    // printf(" (origin2:%d)\n", bits);

    uint8_t len = first_bit_pos(bits);

    print_binary(get_bits(val, pos, len));
    printf(" (get_bits)\n");

    print_binary(clear_bits(val, pos, len));
    printf(" (clear_bits)\n");

    print_binary(set_bits(val, pos, bits, len));
    printf(" (set_bits)\n");

    // 大小写转换
    printf("%c,%c", upper('c'), lower('C'));

    return 0;
}

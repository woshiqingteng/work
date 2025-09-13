# 1 "main.c"
# 1 "D:\\abc\\GIT²Ö¿â\\doc_and_source_for_mcu_mpu\\D1S\\source\\01_write_mem//"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "main.c"


void main(void)
{
    volatile int *p = 0x47FFC;

    *p = 0x5555AAAA;

    while (1);
}

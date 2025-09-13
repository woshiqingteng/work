

void main(void)
{
    volatile int *p = 0x47FFC;

    *p = 0x5555AAAA;

    while (1);
}


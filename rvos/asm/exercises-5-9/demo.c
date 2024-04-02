int foo(int a, int b)
{
    int c;
    asm volatile(
        "mul %[one], %[two], %[two];mul %[three], %[four], %[four];add %[result], %[one], %[three]"
        : [result] "=r"(c)
        : [one] "r"(a), [three] "r"(b), [two] "r"(a), [four] "r"(b)

    );
    return c;
}
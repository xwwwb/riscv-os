int foo(int a, int b)
{
    int c = a + *((&a) - 1);
    return c;
}

// int foo(int a, int b)
// {
//     int c = a + b;
//     return c;
// }
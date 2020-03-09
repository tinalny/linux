#include <iostream>

void swap(int &a,int &b)
{
    int tmp;
    tmp = a;
    a = b;
    b = tmp;
}

int main()
{
    int a =2;
    int b =4;
    swap(a,b);
    return 0;
}

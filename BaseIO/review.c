#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    FILE *fp = fopen("./test.txt","w");
    if(fp == NULL)
    {
        perror("fopen error");
        return -1;
    }
}


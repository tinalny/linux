#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *env[])
{
    int i = 0;
    char *ptr = getenv("MYVAL");
    if(ptr == NULL)
    {
        printf("have no MYVAL\n");
    }
    else
    {
        printf("MYVAL=%s\n",ptr);
    }
    /*
    extern char **environ;
    for(i = 0; environ[i] != NULL; i++)
    {
        printf("env[%d] = [%s]\n",i,environ[i]);
    }
    */
    return 0;
}

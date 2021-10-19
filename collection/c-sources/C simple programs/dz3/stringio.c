#include "stringio.h"


int strcmp_ (const char *s1, const char * s2)
{
  int i = 0;
  
  while (s1[i] && s1[i] == s2[i])
    { 
      i++;
    }

  return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

size_t strlen_ (const char *string)
{
    size_t i = 0;
    while(string[i]) 
        i++;
    return i;
}

char *strcpy_ (char *string1, const char * string2)
{
    size_t i = 0;
    while(string2[i])
    {
        string1[i] = string2[i];
        i++;
    }
    string1[i] = 0;
    return string1;
}

void print_array(char ** a, int n, int m)
{
    m = (n > m ? m : n);
    for(int i = 0; i < m; i++)
    {
        printf("%s\n", a[i]);
    }
}

void free_array(char **a, int n)
{
    for(int i = 0; i < n; i++)
    {
        free(a[i]);
    }
    free(a);
}

int read_array(char **a, int n, const char* name)
{
    int i;
    int j;
    char buf[LEN];
    FILE *fp;
    if(!(fp = fopen(name, "rt")))
        return ERROR_OPEN;
    for(i = 0; i < n; i++)
    {
        if(!fgets(buf, LEN, fp))
        {
            fclose(fp);
            free_array(a, i);
            return ERROR_READ;
        }
        for(j = 0; buf[j]; j++)
        {
            if(buf[j] == '\n')
            {
                buf[j] = 0;
                break;
            }
        }
        a[i] = (char*) malloc(sizeof(char) * (j + 1));
        if(!a[i])
        {
            fclose(fp);
            free_array(a, i);
            return ERROR_MEM;
        }
        strcpy_(a[i], buf);
    }
    fclose(fp);
    return ERROR_SUCCESS;
}
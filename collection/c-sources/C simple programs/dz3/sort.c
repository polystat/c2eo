#include "sort.h"
#include "stringio.h"

int cmp_len_inc(char* s1, char* s2){
    size_t len1 = strlen_(s1);
    size_t len2 = strlen_(s2);

    if(len1 > len2){
        return 1;
    } else if(len1 < len2){
        return -1;
    }
    return 0;
}

int cmp_len_dec(char* s1, char* s2){
    size_t len1 = strlen_(s1);
    size_t len2 = strlen_(s2);

    if(len1 < len2){
        return 1;
    }
    if(len1 > len2){
        return -1;
    }
    return 0;
}

int cmp_lex_inc(char* s1, char* s2){
    int lex = strcmp_(s1, s2);
    if(lex > 0){
        return 1;
    }
    if(lex < 0){
        return -1;
    }
    return 0;
}

int cmp_lex_dec(char* s1, char* s2){
    int lex = strcmp_(s1, s2);
    if(lex < 0){
        return 1;
    }
    if(lex > 0){
        return -1;
    }
    return 0;
}

int sort01(char** a, int n, char* x, int (*cmp) (char*, char*))
{
    int left = 0;
    int right = n;
    int s = n / 2;
    if((*cmp)(a[0], x) > 0)
    {
        return 0;
    }
    if((*cmp)(a[n - 1], x) < 0)
    {
        return n;
    }
    while(left < right)
    {
        if((*cmp)(a[s], x) < 0)
        {
            left = s + 1;
        }
        else
        {
            right = s;
        }
        s = (left + right) / 2;
    }
    return s;
}

void sort02(char** a, char** b, char** c ,int n, int m, int (*cmp) (char*, char*))
{
    int i = 0;
    int j = 0;
    int p = 0;
    while((i < n) && (j < m))
    {
        if((*cmp)(a[i], b[j]) < 0)
        {
            c[p] = a[i];
            p++;
            i++;
        }else
        {
            c[p] = b[j];
            p++;
            j++;
        }
    }
    if(p < n + m)
    {
        if(i < n)
        {
            while(p < n + m)
            {
                c[p] = a[i];
                i++;
                p++;
            }
        }else
        {
            while(p < n + m)
            {
                c[p] = b[j];
                j++;
                p++;
            }
        }
    }
}

int sort03(char** a, int n, char* x, int (*cmp) (char*, char*))
{
    char* tmp = 0;
    int i = 0;
    int j = n - 1;
    while (i <= j)
	{
		while (i < n && (*cmp)(a[i], x) < 0)
		{ 
            i++; 
        }
		
		while (j > 0 && !((*cmp)(a[j], x) < 0))
		{ 
            j--; 
        }
		
		if (i <= j) 
        {
            tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
            j--;
        }

	}	
    return i;
    
}

void sort04(char** a, int n, int (*cmp) (char*, char*))
{
    char* tmp = 0;
    for (int i = n - 1; i > 0; i --)
    {
        for (int j = 0; j < i; j++)
        {
            if ((*cmp)(a[j], a[j + 1]) > 0)
            {
                tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

void sort05(char** a, int n, int (*cmp) (char*, char*) )
{
    char* tmp = 0;
    int i;
    int j;
    int pos;

    for (i = 0; i < n - 1; i++)
    {
        pos = i;
        for (j = i; j < n; j++)
        {
            if ((*cmp)(a[j], a[pos]) < 0)
            {
               pos = j;
            }
        }
        tmp = a[i];
        a[i] = a[pos];
        a[pos] = tmp;
    }
}

void sort06(char** a, int n, int (*cmp) (char*, char*))
{
    char* tmp;
    int j;
    int pos;
    for(int i = 1; i < n; i++)
    {
        pos = 0;
        for(j = 0; j <= i; j++)
        {
            if(!((*cmp)(a[j], a[i]) < 0))
            {
                pos = j;
                break;
            }
        }
        tmp = a[i];
        for(j = i; j > pos; j--)
        {
            a[j] = a[j - 1];
        }
        a[pos] = tmp;
    }
}

void sort07(char** a, int n, int (*cmp) (char*, char*))
{
    char* tmp = 0;
    int pos = 0;
    for(int i = 1; i < n; i++)
    {
        int j;
        tmp = a[i];
        pos = sort01(a, i, tmp, cmp);
        for(j = i ; j > pos; j--)
        {
            a[j] = a[j - 1];
        }
        a[pos] = tmp;
    }
}

void sort08(char** a, char** b, int n, int (*cmp) (char*, char*))
{
    char** a_orig = a;
    char** tmp;
    int i;
    int mergeCounter = 1;
    int rem1 = n % 2;
    int rem2 = n % 2;
    while(2 * mergeCounter <= n)
    {
        int mg2 = mergeCounter * 2;
        for(i = 0; i + mg2 <= n; i += mg2)
        {
            sort02(a + i, a + i + mergeCounter, b + i, mergeCounter, mergeCounter, cmp);
        }
        rem1 = rem2;
        rem2 = n % (mg2);
        if(rem1 == rem2)
        {
            while(i < n)
            {
                b[i] = a[i];
                i++;
            }
        }
        else
        {
            sort02(a + i, a + i + rem2 - rem1, b + i, rem2 - rem1, rem1, cmp);
        }
        mergeCounter = mg2;
        tmp = a;
        a = b;
        b = tmp;
    }

    if(n - mergeCounter > 0)
    {
        sort02(a, a + mergeCounter, b , mergeCounter, n - mergeCounter, cmp);
        a = b;
    }

    if(a_orig != a)
    {
        for(int i = 0; i < n; i++)
        {
            a_orig[i] = a[i];
        }        
    }
}

int sort03_1(char** a, int n, char* x, int (*cmp) (char*, char*))
{
    char* tmp = 0;
    int i = 0;
    int j = n - 1;
    while (i <= j)
	{
		while ((*cmp)(a[i], x) < 0)
		{ 
            i++; 
        }
		
		while ((*cmp)(a[j], x) > 0)
		{ 
            j--; 
        }
		
		if (i <= j) 
        {
            tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
            j--;
        }

	}
    return i;
}

void sort09(char** a,int n, int (*cmp) (char*, char*))
{
    int i = 0;
    char* x = 0;
    while(n > 1)
    {
        x = a[n / 2];
        i = sort03_1(a, n, x ,cmp);
        if(i == n)
        {
            i--;
        }
        if(i < n - i)
        {
            sort09(a, i, cmp);
            n -= i;
            a += i;
        }
        else
        {
            sort09(a + i, n - i, cmp);
            n = i;
        }   
    }
}
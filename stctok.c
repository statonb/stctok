/*
**  stctok() -- public domain by Ray Gardner, modified by Bob Stout.
**              Renamed to stctok from stptok and modified by Brian Staton
**
**   You pass this function a string to parse, a buffer to receive the
**   "token" that gets scanned, the length of the buffer, and a string of
**   "break" characters that stop the scan.  It will copy the string into
**   the buffer up to any of the break characters, or until the buffer is
**   full, and will always leave the buffer null-terminated.  It will
**   return a pointer to the first non-breaking character after the one
**   that stopped the scan.
**
**   Back-to-back break characters can be handled one of two ways.
**   collapseFlag == 0 will return a zero-length NULL when back-to-back breaks
**   are encountered.  The return char* advances one position in the string.
**   collapseFlag != 0 will advance past all back-to-back break characters.
*/

#include <string.h>
#include <stdlib.h>

char *stctok(const char *s, char *tok, size_t toklen, char *brk, int collapseFlag)
{
    char *lim, *b;

    if (!*s)
        return NULL;

    lim = tok + toklen - 1;
    while ( *s && tok < lim )
    {
        for ( b = brk; *b; b++ )
        {
            if ( *s == *b )
            {
                *tok = 0;
                ++s;
                if (collapseFlag)   // use this to eat all brk characters
                {
                    b = brk;
                    do
                    {
                        if (*s == *b)
                        {
                            ++s;
                            b = brk;
                        }
                        else
                        {
                            ++b;
                        }
                    }
                    while (*s && *b);
                }
                return (char *)s;
            }
        }
        *tok++ = *s++;
    }
    *tok = 0;
    return (char *)s;
}

#ifdef TEST

#include <stdio.h>

#define COLLAPSE_FLAG   (0)

int main(int argc, char *argv[])
{
    char *ptr, buf[256];
    char *holdPtr;
    char *tokStr;
    int n;

    if (3 > argc)
    {
        ptr = "1,,,,5,,,,9";
        // ptr = "this,is,a,test";
        tokStr = ",";
        //  puts("Usage: stctok \"string\" \"token_string\"");
        //  return EXIT_FAILURE;
    }
    else
    {
        ptr = argv[1];
        tokStr = argv[2];
    }

    n = 1;
    do
    {
        holdPtr = ptr;
        ptr = stctok(ptr, buf, sizeof(buf), tokStr, COLLAPSE_FLAG);
        printf("Pass %d: stctok(\"%s\", \"%s\")\n  buf: \"%s\" len=%d\n  returned: \"%s\"\n"
               ,n
               ,holdPtr
               ,tokStr
               ,buf
               ,strlen(buf)
               ,ptr
              );
        ++n;
    }
    while (ptr && *ptr);
    printf("Upon exit ptr is %s and *ptr is %s\n"
           ,(ptr) ? "NULL" : "not NULL"
           ,(*ptr) ? "NULL" : "not NULL"
          );
    return EXIT_SUCCESS;
}

#endif /* TEST */




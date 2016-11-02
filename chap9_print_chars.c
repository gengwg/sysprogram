#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* allocates n arrays of chars containing from 2 elements (2 bytes)
   up to n+1 elments (n+1 bytes). then for each array, the loop 
   writes the character c into each byte except the last (leaving the
   0 that's already in the last byte). prints out the array as string.
   then frees the dynamically allocated memory.
 */
void print_chars (int n, char c)
{
    int i;

    for (i = 0; i < n; i++) {
        char *s;
        int j;

        /* allocate and zero an i+2 elment array of chars.
           note 'sizeof (char)' is always 1.
         */
        s = calloc (i + 2, 1);
        //s = calloc (i, 1);
        if (!s) {
            perror ("calloc");
            break;
        }

        for (j = 0; j < i + 1; j++)
            s[j] = c;
        printf ("%s\n", s);

        free(s);
    }
}

int main()
{
    print_chars (8, 'X');
}
/* 
   X
   XX
   XXX
   XXXX
   XXXXX

 */

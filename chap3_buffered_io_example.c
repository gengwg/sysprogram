/* Sample Program Using Buffered I/O */

#include <stdio.h>

int main (void) {
    FILE *in, *out;
    /*
       decalre two variables of struct pirate
       initialize one of them.
    */
    struct pirate {
        char            name[100];  // real name
        unsigned long   booty;      // in pounds sterling
        unsigned int    beard_len;  // in inches
    } p, blackbeard = { "Edward Teach", 950, 48};
    

    out = fopen ("data", "w");
    if (!out) {
        perror ("fopen");
        return 1;
    }

    /* Writing Binary Data

       size_t fwrite (void *buf,
                      size_t size,
                      size_t nr,
                      FILE *stream);
       a call to fwrite() will write to stream up to nr elements,
       each size bytes in length, from the data pointed by buff.

       file pointer will be advanced by total number of bytes written.

       return: number of elements (not bytes) successfully written.
       return value less than nr denotes error.
     */

    /* write blackbeard pirate to disk via an output stream */
    if (!fwrite (&blackbeard, sizeof (struct pirate), 1, out)) {
        perror ("fwrite");
        return 1;
    }

    if (fclose (out)) {
        perror ("fclose");
        return 1;
    }

    in = fopen("data", "r");
    if (!in) {
        perror ("fopen");
        return 1;
    }

    /* reads the data in from 'data' via a different stream to 
       the other instance of struct pirate.
     */
    if (!fread(&p, sizeof (struct pirate), 1, in)) {
        perror ("fread");
        return 1;
    }

    if (fclose (in)) {
        perror ("fclose");
        return 1;
    }

    /* write the contents of the structure to standard output */
    printf ("name=\"%s\" booty=%lu beard_len=%u\n",
            p.name, p.booty, p.beard_len);

    /* it's important to note that because of diff in variabe sizes,
       alignment, binary data written with one application may not 
       be readable by another application. 

       these things remain constant only on a particular machine type 
       with a particular ABI.
     */

    return 0;
}

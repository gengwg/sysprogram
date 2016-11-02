#include <unistd.h>
#include <stdio.h> 

#include <dirent.h>
#include <errno.h>

/* searches the diretory 'path' for a file named 'file'

   returns 0 if the file exists in path
   and nonzero otherwise
 */
int find_file_in_dir (const char *path, const char *file)
{
    struct dirent *entry;
    int ret = 1;

    DIR *dir;
    dir = opendir (path);

    errno = 0;
    while ((entry = readdir (dir)) != NULL) {
        if (strcmp(entry->d_name, file) ==0) {
            ret = 0;
            break;
        }
    }

    if (errno && !entry)
        perror ("readdir");

    closedir (dir);
    return ret;

}

int main()
{
    printf ("%d\n", find_file_in_dir ("/", "tmp"));
}


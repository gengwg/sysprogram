#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/cdrom.h>
#include <stdio.h>

/* out of band communication */

/* kernel receves an ioctl() request, it finds the device driver responsible
   for the file descripter provided and passes on the request for handing.
   the CD-ROM device driver receives the request and physically ejects the device.
 */

int main (int argc, char *argv[])
{
    int fd, ret;

    if (argc < 2) {
        fprintf (stderr,
                "usage: %s <device to eject>\n",
                argv[0]);
        return 1;
    }

    /* open the cd-rom read-only.
       O_NONBLOCK tells the kernel that we want to open the device
       evne if there is no media present in the device.
     */
    fd = open (argv[1], O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror ("open");
        return 1;
    }

    /* send the eject command to the cd-rom device */
    ret = ioctl (fd, CDROMEJECT, 0);
    if (ret) {
        perror ("ioctl");
        return 1;
    }

    ret = close (fd);
    if (ret) {
        perror ("close");
        return 1;
    }

    return 0;
}

/*
   $ gcc chap8_cdromeject.c -o eject
   $ ./eject /dev/cdrom 
 */

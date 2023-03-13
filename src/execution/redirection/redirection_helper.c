#include <err.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "redirection.h"

bool is_closing_op(char *word)
{
    if (word[0] == '\0')
        return false;

    if (word[0] == '-' && word[1] == '\0')
        return true;
    return false;
}

// stdlib's atoi returns 0 on error but it also returns 0 when word == '0'
int my_atoi(char *word)
{
    int num = atoi(word);
    if (num != 0 || (word[0] == '0' && word[1] == '\0'))
        return num;
    return -1;
}

// function for "< / > / <> / >> / >|"
// opens the file who's the result of 'word' 's expansion
int word_to_fd(char *word, int open_flags)
{
    int file_fd;

    file_fd = open(word, open_flags, 0664);
    if (file_fd == -1)
        errx(1, "error while trying to open '%s'", word);
    chmod(word, 0664);

    return file_fd;
}

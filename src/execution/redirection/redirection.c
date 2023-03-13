#include "redirection.h"

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "run_ast.h"

static int open_redirection(int file_fd, int io_number)
{
    // Duplicate *io_number's STD to retrieve it after redirection
    int fd_dup = dup(io_number);

    // Redirect file's fd to stdout
    int new_io_std = dup2(file_fd, io_number);
    if (new_io_std == -1)
    {
        close(fd_dup);
        close(file_fd);
        perror("open_redirection : bad fd\n");
        return -2;
    }
    assert(new_io_std == io_number);

    // Close given fd as it's not needed anymore
    if (close(file_fd) == -1)
    {
        close(fd_dup);
        close(new_io_std);
        errx(1, "close file_fd failed :/");
    }

    return fd_dup;
}

// close redirection by restoring *io_number's std and closing fd_dup
static int close_redirection(int fd_dup, int io_number)
{
    int new_io_std = dup2(fd_dup, io_number);
    if (new_io_std == -1)
    {
        close(fd_dup);
        errx(1, "Couldn't dup io_number's std");
    }

    // Close stdout duplicate as we retrieved our original stdout fd
    if (close(fd_dup) == -1)
        errx(1, "couldn't close fd");

    return 0;
}

// '<' : redirect word's fd to stdin
// ex  : cat < oo.txt (same as doing 'echo oo.txt')
static int open_redir_input(int *io_number, char *word, bool io_default)
{
    if (io_default)
        *io_number = 0;

    int file_fd = word_to_fd(word, O_RDONLY); // file from where to write

    return open_redirection(file_fd, *io_number);
}

// open '>' redirection and return the saved fd for *io_number
static int open_redir_output(int *io_number, char *word, bool io_default)
{
    if (io_default)
        *io_number = 1;

    int file_fd = word_to_fd(word, O_TRUNC | O_CREAT | O_WRONLY);

    return open_redirection(file_fd, *io_number);
}

// open word so we redirect stdout to append to word's fd
//  ex  : echo non >> oo.txt (non is APPENDED to the content of oo.txt)
static int open_redir_append_output(int *io_number, char *word, bool io_default)
{
    if (io_default)
        *io_number = 1;
    int file_fd = word_to_fd(word, O_APPEND | O_WRONLY | O_CREAT);

    return open_redirection(file_fd, *io_number);
}

static int open_fd_read_write(int *io_number, char *word, bool io_default)
{
    if (io_default)
        *io_number = 0;
    int file_fd = word_to_fd(word, O_CREAT | O_RDWR);

    return open_redirection(file_fd, *io_number);
}

static int ampersand_closing(int std_to_close)
{
    close(std_to_close);
    return -1;
}

static int open_dup_in_fd(int *io_number, char *word, bool io_default)
{
    if (io_default)
        *io_number = 0;
    int file_fd = my_atoi(word);
    if (file_fd == -1 && is_closing_op(word))
    {
        return ampersand_closing(*io_number);
    }

    return open_redirection(file_fd, *io_number);
}

static int open_dup_out_fd(int *io_number, char *word, bool io_default)
{
    if (io_default)
        *io_number = 1;
    int file_fd = my_atoi(word);
    if (file_fd == -1 && is_closing_op(word))
    {
        return ampersand_closing(*io_number);
    }

    return open_redirection(file_fd, *io_number);
}

int redirect(struct ast_redir *red)
{
    static const do_redirection functions[] = {
        [LESS] = &open_redir_input,           [GREAT] = &open_redir_output,
        [DGREAT] = &open_redir_append_output, [LESSAND] = &open_dup_in_fd,
        [GREATAND] = &open_dup_out_fd,        [CLOBBER] = &open_redir_output,
        [LESSGREAT] = &open_fd_read_write,
    };

    // OPEN REDIR
    int fd_dup = (*functions[red->redir_type])(&red->io_number, red->word,
                                               red->io_default);

    if (fd_dup == -2)
    {
        fflush(stderr);
        fflush(stdout);
        return 1;
    }

    // EXEC CMD //
    int return_code = run_ast(red->cmd);
    fflush(stdout);

    // CLOSE REDIR
    if (fd_dup != -1)
        close_redirection(fd_dup, red->io_number);

    return return_code;
}

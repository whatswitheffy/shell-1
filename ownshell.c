#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

char *get_word(char *end) {
    char ch, *word = NULL;
    int len = 0;
    ch = getchar();
    word = malloc(sizeof(char));
    while (ch != ' ' && ch != '\n') {
        word = realloc(word, (len + 1) * sizeof(char));
        word[len] = ch;
        len++;
        ch = getchar();
    }
    *end = ch;
    word = realloc(word, (len + 1) * sizeof(char));
    word[len] = '\0';
    return word;
}

char **get_list() {
    char **list, end;
    int count = 1;
    list = malloc(sizeof(char*));
    if (!list) {
        return NULL;
    }
    list[0] = get_word(&end);
    while (end != '\n') {
        list = realloc(list, (count + 1) * sizeof(char*));
        list[count] = get_word(&end);
        count++;
    }
    list = realloc(list, (count + 1) * sizeof(char*));
    list[count] = NULL;
    return list;
}

void delete_list(char **list) {
    int i;
    if (list == NULL)
        return;
    for (i = 0; list[i] != NULL; i++) {
        free(list[i]);
    }
    free(list[i]);
    free(list);
    return;
}

int check_io(char **list, int *io) {
    int flag = 0, fd = 0;
    for (int i = 0; list[i] != NULL; i++) {
        if (list[i][0] == '>') {
            if (list[i + 1] != NULL) {
                fd = open(list[i + 1], O_WRONLY | O_CREAT | O_TRUNC,
                                   S_IRUSR | S_IWUSR);
                flag = 1;
                break;
            }
        }
        if (list[i][0] == '<') {
            if (list[i + 1] != NULL) {
                fd = open(list[1], O_RDONLY | O_CREAT | O_TRUNC,
                               S_IRUSR | S_IWUSR);
                flag = 0;
                break;
            }
        }
    }
    *io = flag;
    return fd;
}


int perform(char **list) {
    pid_t pid;
    int fd = 0, io = 0;
    fd = check_io(list, &io);
    pid = fork();
    if (io) {
        dup2(fd, 1);
    } else {
        dup2(fd, 0);
    }
    if (pid > 0) {
        wait(NULL);
    } else {
        if (io) {
            dup2(fd, 1);
        } else {
            dup2(fd, 0);
        }
        if (execvp(list[0], list) < 0) {
            perror("Execvp failed!\n");
            return 1;
        }
    }
    return 0;
}

int main() {
    char **list = NULL;
    char erro;
    list = get_list();
    while (strcmp(list[0], "exit") && strcmp(list[0], "quit")) {
        erro = perform(list);
        if (erro) {
            break;
        }
        delete_list(list);
        list = get_list();
    }
    delete_list(list);
    return 0;
}

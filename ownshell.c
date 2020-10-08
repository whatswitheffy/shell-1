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

int io_file(char **list, int fd, pid_t pid) {
    for (int i = 0; list[i] != NULL; i++) {
        if (list[0][i] == '>') {
            if (list[i + 1] != NULL) {
                fd = open(list[i + 1],
                            O_WRONLY | O_CREAT | O_TRUNC,
                            S_IRUSR | S_IWUSR);
                if (pid == 0) {
                    dup2(fd, 1);
                    break;
                }
            }
        }
        if (list[0][i] == '<') {
            if (list[i + 1] != NULL) {
                fd = open(list[i + 1], 
                            O_RDONLY | O_CREAT | O_TRUNC,
                            S_IRUSR | S_IWUSR);
                if (pid == 0) {
                    dup2(fd, 0);
                    break;
                }

            }
        }
    }
    return fd;
}

void infinit(char **list) {
    pid_t pid;
    int fd = 0;
    while(strcmp(list[0], "exit") && strcmp(list[0], "quit")) {
        pid = fork();
        fd = io_file(list, fd, pid);
        if (pid > 0) {
            wait(NULL);
        } else {
            if (execvp(list[0], list) < 0) {
                perror("execvp failed!\n");
                return;
            }
        }
        delete_list(list);
        list = get_list();
    }
    delete_list(list);
    return;
}

int main() {
    char **list = NULL, flag;
    list = get_list();
    infinit(list);
    return 0;
}

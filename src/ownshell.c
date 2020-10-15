#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int cmp_exit(char **list) {
    int ans1, ans2;
    ans1 = strcmp(list[0], "exit");
    ans2 = strcmp(list[0], "quit");
    if (!ans1 || !ans2) {
        return 0;
    }
    return 1;
}

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

int search(char **list, char *word) {
    int pos;
    for (pos = 0; list[pos] != NULL; pos++) {
        if (!strcmp(word, list[pos])) {
            return pos;
        }
    }
    return -1;
}

void clean_io(char **list, int i) {
    int j = 0;
    while (list[i + j + 2] != NULL) {
        list[i + j] = list[i + j + 2];
        j++;
    }
    j--;
    list[i + j] = NULL;
    free(list[i + j + 1]);
    free(list[i + j + 2]);
    return;
}

int io_file(char **list, int fd, pid_t pid) {
    for (int i = 0; list[i] != NULL; i++) {
        if (list[i][0] == '>') {
            if (list[i + 1] != NULL) {
                if (list[i + 1] != NULL) {
                    fd = open(list[i + 1],
                                O_WRONLY | O_CREAT | O_TRUNC,
                                S_IRUSR | S_IWUSR);
                }
                clean_io(list, i);
                dup2(fd, 1);
                close(fd);
                break;
            }
        }
        if (list[i][0] == '<') {
            if (list[i + 1] != NULL) {
                if (list[i + 1] != NULL) {
                    fd = open(list[i + 1], 
                                O_RDONLY | O_CREAT | O_TRUNC,
                                S_IRUSR | S_IWUSR);
                }
                clean_io(list, i);
                dup2(fd, 0);
                close(fd);
                break;
            }
        }
    }
    return fd;
}

void implemintation(char **list, pid_t pid, int fd) {
    if (pid > 0) {
        wait(NULL);
    } else {
        fd = io_file(list, fd);
        if (execvp(list[0], list) < 0) {
            perror("Execution failed!\n");
            close(fd);
            return;
        }
    }
    return;
}

void hall_of_funcs(char **list) {
    pid_t pid;
    int fd = 0;
    list = get_list();
    while (cmp_exit(list)) {
        pid = fork();
        implemintation(list, pid, fd);
        delete_list(list);
        list = get_list();
    }
    delete_list(list);
    return;
}

int main() {
    char **list = NULL;
    hall_of_funcs(list);
    return 0;
}

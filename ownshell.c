#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char *get_word(char *last_ch) {
    char ch, *word = NULL;
    ch = getchar();
    int len = 0;
    word = malloc(sizeof(char));
    while (ch != ' ' && ch != '\n') {
        word = realloc(word, (len + 1) * sizeof(char));
        word[len] = ch;
        len++;
        ch = getchar();
    }
    *last_ch = ch;
    word = realloc(word, (len + 1) * sizeof(char));
    word[len] = '\0';
    return word;
}

char **get_list() {
    char **list = NULL, last_ch;
    int count = 1;
    list = malloc(sizeof(char*));
    if (!list)
        return NULL;
    list[0] = get_word(&last_ch);
    while (last_ch != '\n') {
        list = realloc(list, (count + 1) * sizeof(char*));
        list[count] = get_word(&last_ch);
        count++;
    }
    list = realloc(list, (count + 1) * sizeof(char*));
    list[count] = NULL;
    return list;
}

void free_list(char **list) {
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

int strcmp(char *str1, char *str2) {
    int i = -1;
    do {
        i++;
        if (str1[i] < str2[i])
            return 1;
        if (str1[i] > str2[i])
            return 2;
    } while (str1[i] && str2[i]);
    return 0;
}

int main() {
    char **list = NULL, ers = 0;
    char STP_WRD[] = "quit";
    list = get_list();
    while (strcmp(list[0], STP_WRD)) {
        if (ers)
            break;
        free_list(list);
        list = get_list();
    }
    free_list(list);
    return 0;
}

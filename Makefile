%: %.c
	gcc $@.c -o $@ -Wall -Werror -lm -fsanitize=undefined -fsanitize=address

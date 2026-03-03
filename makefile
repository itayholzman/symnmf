CC = gcc
CFLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

project: symnmf.o points_list_utils.o matrix_utils.o
	$(CC) -o project symnmf.o points_list_utils.o matrix_utils.o $(LIBS)

symnmf.o: symnmf.c symnmf.h points_list_utils.h matrix_utils.h
	$(CC) -c symnmf.c $(CFLAGS)

points_list_utils.o: points_list_utils.c points_list_utils.h
	$(CC) -c points_list_utils.c $(CFLAGS)

matrix_utils.o: matrix_utils.c matrix_utils.h
	$(CC) -c matrix_utils.c $(CFLAGS)

clean:
	rm -f *o
	rm project
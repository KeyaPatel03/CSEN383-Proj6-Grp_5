CC=gcc

compile:main.c common.c
	$(CC) -o pipe_simulator main.c common.c
CC=gcc

all:
	$(CC) -o main main.c

run_ls: all
	export TASK_INDEX=1 && ./main

run_date: all
	export TASK_INDEX=2 && ./main

clean:
	unset TASK_INDEX && rm -rf main

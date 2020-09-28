CC?=gcc

web-blocker: web-blocker.c
	$(CC) -pedantic -std=c89 -o web-blocker web-blocker.c

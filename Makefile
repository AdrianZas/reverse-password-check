all:
	gcc -o secret -s secret.c

strip:
	strip secret

clean:
	rm -f secret

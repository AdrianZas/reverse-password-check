#include <stdio.h>
#include <string.h>

void encrypt_password(char *out)
{

	char get_char(int index) {
		if (index < 26) return 'A' + index;
		else if (index < 52) return 'a' + (index - 26);
		else return '1' + (index - 52);
	}

	for (int i = 0; i < 5; i++) {
		out[i] = get_char((i * 7 + 3) % 62) ^ 0x13;
	}
	out[5] = '\0';
}

int main()
{
	char input[20];
	char pwd[20];

	printf("Enter password: ");
	scanf("%s", input);

	for(int i = 0; i < strlen(input); i++) {
		input[i] ^= 0x13;
	}

	encrypt_password(pwd);

	if (strcmp(input, pwd) == 0) {
		printf("Access granted\n");
	} else {
		printf("Access denied\n");
	}
}

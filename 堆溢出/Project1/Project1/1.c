#define _CRT_SECURE_NO_WARNINGS//c run time不要把它作为错误，strcpy

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
int hacked()
{
	printf("金秀贤！");
}

int sub(char* x)
{
	char y[10];
	strcpy(y, x);
	return 0;
}
//213115c9
//char* ov = "12341235123612371238";
char* ov = "1234123512361237\xc9\x15\x31\x21";
int main(int argc, char**argv)
{
	if (argc > 1)
		sub(ov);
	printf("exit");
}
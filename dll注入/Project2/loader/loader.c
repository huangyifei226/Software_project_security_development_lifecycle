#include<windows.h>
//数据类型，函数指针类型
typedef int (WINAPI *MY_PROC)(char *);//int返回值类型；WINAPI调用方式；*MY_PROC函数指针；char*参数列表

int main()
{
	MessageBoxA(0, 0, 0, 0);
	HMODULE hBaselib = LoadLibraryA("baselib.dll");
	if (hBaselib == NULL)
		return 0;
	MY_PROC func=(MY_PROC)GetProcAddress(hBaselib, "lib_function");
	//fun为指针变量，从已经加载的dll中获得已知名字的函数的地址
	func("run time load");//函数指针变量和函数名使用方法相同
}
#include<windows.h>
//�������ͣ�����ָ������
typedef int (WINAPI *MY_PROC)(char *);//int����ֵ���ͣ�WINAPI���÷�ʽ��*MY_PROC����ָ�룻char*�����б�

int main()
{
	MessageBoxA(0, 0, 0, 0);
	HMODULE hBaselib = LoadLibraryA("baselib.dll");
	if (hBaselib == NULL)
		return 0;
	MY_PROC func=(MY_PROC)GetProcAddress(hBaselib, "lib_function");
	//funΪָ����������Ѿ����ص�dll�л����֪���ֵĺ����ĵ�ַ
	func("run time load");//����ָ������ͺ�����ʹ�÷�����ͬ
}
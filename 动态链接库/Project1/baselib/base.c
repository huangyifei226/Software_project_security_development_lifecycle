#include <windows.h>
int intnal_function()
{
	return 0;
}
int lib_function(char* msg)
{
	//do some works
	MessageBoxA(0, "msg from base lib", msg, MB_OK);
	return 0;
}
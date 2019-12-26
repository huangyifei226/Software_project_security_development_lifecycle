#include <stdio.h>

#include <Windows.h>

#include <tlhelp32.h>

#include "fheaders.h"



#include <Windows.h>

#include <stdio.h>

#include "fheaders.h"

DWORD wmain()

{
			demoCreateRemoteThreadW(L"C:\\Users\\Lenovo\\source\\repos\\Project2\\Debug\\baselib.dll",8820);
			return(0);

	}
DWORD demoCreateRemoteThreadW(PCWSTR pszLibFile, DWORD dwProcessId)

{

	// Calculate the number of bytes needed for the DLL's pathname

	DWORD dwSize = (lstrlenW(pszLibFile) + 1) * sizeof(wchar_t);



	// Get process handle passing in the process ID

	HANDLE hProcess = OpenProcess(//OpenProcess用来打开一个已存在的进程对象，并返回进程的句柄

		PROCESS_QUERY_INFORMATION |

		PROCESS_CREATE_THREAD |

		PROCESS_VM_OPERATION |

		PROCESS_VM_WRITE,

		FALSE, dwProcessId);

	if (hProcess == NULL)

	{
	
		printf(TEXT("[-] Error: Could not open process for PID (%d).\n"), dwProcessId);

		return(1);

	}
	// Allocate space in the remote process for the pathname

	LPVOID pszLibFileRemote = (PWSTR)VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);
	//VirtualAllocEx在指定进程的虚拟地址空间中保留或开辟一段区域
	//执行成功就返回分配内存的首地址，不成功就是NULL。

	if (pszLibFileRemote == NULL)//分配不成功

	{
	
		printf(TEXT("[-] Error: Could not allocate memory inside PID (%d).\n"), dwProcessId);

		return(1);

	}
	// Copy the DLL's pathname to the remote process address space

	DWORD n = WriteProcessMemory(hProcess, pszLibFileRemote, (PVOID)pszLibFile, dwSize, NULL);
	//WriteProcessMemory此函数能写入某一进程的内存区域，返回非零值代表成功
	//pszLibFile指向要写的数据的指针，这里就是dll的绝对路径

	if (n == 0)

	{

		printf(TEXT("[-] Error: Could not write any bytes into the PID [%d] address space.\n"), dwProcessId);

		return(1);

	}
	// Get the real address of LoadLibraryW in Kernel32.dll

	PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
	//GetProcAddress检索指定的动态链接库(DLL)中的输出库函数地址
	if (pfnThreadRtn == NULL)

	{

		printf(TEXT("[-] Error: Could not find LoadLibraryA function inside kernel32.dll library.\n"));

		return(1);

	}
	// Create a remote thread that calls LoadLibraryW(DLLPathname)

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pfnThreadRtn, pszLibFileRemote, 0, NULL);

	if (hThread == NULL)

	{

		printf(TEXT("[-] Error: Could not create the Remote Thread.\n"));

		return(1);

	}

	else

		printf(TEXT("[+] Success: DLL injected via CreateRemoteThread().\n"));



	// Wait for the remote thread to terminate

	WaitForSingleObject(hThread, INFINITE);



	// Free the remote memory that contained the DLL's pathname and close Handles

	if (pszLibFileRemote != NULL)

		VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);



	if (hThread != NULL)

		CloseHandle(hThread);

	if (hProcess != NULL)
		CloseHandle(hProcess);
	return(0);

}
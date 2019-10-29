#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#define MAX_THREADS 10  //创建10个线程，效果更好更明显
#define BUF_SIZE 255

DWORD WINAPI MyThreadFunction(LPVOID lpParam);


// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
typedef struct MyData {
	int val1;
	int val2;
} MYDATA, *PMYDATA;


int _tmain()
{
	PMYDATA pDataArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];

	// Create MAX_THREADS worker threads.

	DWORD start = GetTickCount();

	for (int i = 0; i<MAX_THREADS; i++)
	{
		// Allocate memory for thread data.

		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(MYDATA));

		if (pDataArray[i] == NULL)
		{
			// If the array allocation fails, the system is out of memory
			// so there is no point in trying to print an error message.
			// Just terminate execution.
			ExitProcess(2);
		}

		// Generate unique data for each thread to work with.

		pDataArray[i]->val1 = i;
		pDataArray[i]->val2 = i + 100;  //假装给结构体两个参数，其实就是序号

		// Create the thread to begin execution on its own.
		/*
		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThreadFunction,       // thread function name
			pDataArray[i],          // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);   // 保存线程的ID号

		*/
									// Check the return value for success.
									// If CreateThread fails, terminate execution. 
									// This will automatically clean up threads and memory. 
		MyThreadFunction(pDataArray[i]);
		//if (hThreadArray[i] == NULL)
		//{
		//	printf("CreateThread Error(%d)", GetLastError());
			//ExitProcess(3);
		//}
	} // End of main thread creation loop.

	  // Wait until all threads have terminated.

	//WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);//等待10个子线程全部运行完毕后再退出
	//INFINITE表示无穷等待，也可以设置时间
	// Close all thread handles and free memory allocations.

	DWORD end = GetTickCount();
	printf("tick count: %d\n", end - start); //一共运行了多少时间

	/*for (int i = 0; i<MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;    // Ensure address is not reused.
		}
	}
	*/
	return 0;
}


DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	
	PMYDATA pDataArray;
	pDataArray = (PMYDATA)lpParam;
	Sleep(1000);//1000表示1秒
	printf("Parameters = %d,%d\n", pDataArray->val1, pDataArray->val2);// i和i+100

	return 0;
}

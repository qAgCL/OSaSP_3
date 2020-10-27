#include "pch.h"
#include "ChangeFunc.h"
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <tchar.h>
#include <fstream>
#include <vector>
#include <TlHelp32.h>
std::vector<DWORD> GetPIDByName(char* proc_name)
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	std::vector<DWORD> pids;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry))
		while (Process32Next(snapshot, &entry))
		{  
			if (!strcmp(entry.szExeFile, proc_name))
				pids.push_back(entry.th32ProcessID);
		}
	return pids;
}
extern "C" __declspec(dllexport) void find_change_string(char* ProcName, char *OldStr, char* NewStr) {

	unsigned char *p = NULL;
	MEMORY_BASIC_INFORMATION info;
	std::vector<DWORD> pids = GetPIDByName(ProcName);
	if (!pids.empty()) {
		for (int pidsNum = 0; pidsNum < pids.size(); pidsNum++) {
			HANDLE process = OpenProcess(
				PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE,
				false,
				pids[pidsNum]);
			std::cout << "PID: " << pids[pidsNum];
			show_modules(process);
			for (p = NULL; VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize)
			{
				std::vector<char> buffer;
				if (info.State == MEM_COMMIT &&
					(info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE))
				{
					SIZE_T bytes_read;
					buffer.resize(info.RegionSize);
					ReadProcessMemory(process, p, &buffer[0], info.RegionSize, &bytes_read);
					buffer.resize(bytes_read);
					for (int i = 0; i != buffer.size(); i++) {
						bool flag = true;
						for (int j = 0; j < strlen(OldStr); j++) {
							if (i + j < buffer.size() && OldStr[j] != buffer[i + j]) {
								flag = false;
								break;
							}
						}
						if (flag) {
							char *p = static_cast<char*>(info.BaseAddress) + i;
							int leng;
							if (strlen(OldStr) > strlen(NewStr)) {
								leng = strlen(NewStr);
							}
							else {
								leng = strlen(OldStr);
							}
							WriteProcessMemory(process, (LPVOID)p, NewStr, leng, NULL);
						}
					}
				}
			}
			CloseHandle(process);
		}
	}
	else {
		std::cout << "The program was not found";
	}
}
extern "C" __declspec(dllexport) void show_modules(HANDLE process) {

	unsigned char *p = NULL;
	MEMORY_BASIC_INFORMATION info;

	for (p = NULL;
		VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info);
		p += info.RegionSize)
	{
		printf("%#10.10x (%6uK)\t", info.BaseAddress, info.RegionSize / 1024);

		switch (info.State) {
		case MEM_COMMIT:
			printf("Committed");
			break;
		case MEM_RESERVE:
			printf("Reserved");
			break;
		case MEM_FREE:
			printf("Free");
			break;
		}
		printf("\t");
		switch (info.Type) {
		case MEM_IMAGE:
			printf("Code Module");
			break;
		case MEM_MAPPED:
			printf("Mapped     ");
			break;
		case MEM_PRIVATE:
			printf("Private    ");
		}
		printf("\t");
		int guard = 0, nocache = 0;

		if (info.AllocationProtect & PAGE_NOCACHE)
			nocache = 1;
		if (info.AllocationProtect & PAGE_GUARD)
			guard = 1;

		info.AllocationProtect &= ~(PAGE_GUARD | PAGE_NOCACHE);

		switch (info.AllocationProtect) {
		case PAGE_READONLY:
			printf("Read Only");
			break;
		case PAGE_READWRITE:
			printf("Read/Write");
			break;
		case PAGE_WRITECOPY:
			printf("Copy on Write");
			break;
		case PAGE_EXECUTE:
			printf("Execute only");
			break;
		case PAGE_EXECUTE_READ:
			printf("Execute/Read");
			break;
		case PAGE_EXECUTE_READWRITE:
			printf("Execute/Read/Write");
			break;
		case PAGE_EXECUTE_WRITECOPY:
			printf("COW Executable");
			break;
		}

		if (guard)
			printf("\tguard page");
		if (nocache)
			printf("\tnon-cachable");
		printf("\n");
	}
}
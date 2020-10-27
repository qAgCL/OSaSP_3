#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <tchar.h>
#include <fstream>
#include <vector>
#include "ChangeFunc.h"
#include <TlHelp32.h>

using namespace std;
unsigned long usage;
int main()
{
	string OldString;
	string NewString;
	string ProgramName;

	cout << "Write program name" << endl;
	getline(cin, ProgramName);
	char *ProgCharStr = new char[ProgramName.length() + 1];
	strcpy(ProgCharStr, ProgramName.c_str());

	cout << "Write old string" << endl;
	getline(cin,OldString);
    char *OldCharStr = new char[OldString.length() + 1];
	strcpy(OldCharStr, OldString.c_str());

	cout << "Write new string" << endl;
	getline(cin, NewString);
	char *NewCharStr = new char[NewString.length() + 1];
	strcpy(NewCharStr, NewString.c_str());
	find_change_string(ProgCharStr, OldCharStr, NewCharStr);
	
	return 0;
	
}


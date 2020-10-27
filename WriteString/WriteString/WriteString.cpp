// WriteString.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <Windows.h>
#include <iostream>
#include <psapi.h>
#include <string>
#include <tchar.h>
int main() {
	char String1[] = "Never gonna give you up";
	char String2[] = "Never gonna let you down";
	char String3[] = "Never gonna run around and desert you";
	char String4[] = "Never gonna make you cry";
	int pid = GetCurrentProcessId();
	while (1) {
		std::cout << "Pid: " << pid << std::endl;
		std::cout << "String1: " << String1 << std::endl << " Adress: "<< &String1<< std::endl;
		std::cout << "String2: " << String2 << std::endl << " Adress: " << &String2 << std::endl;
		std::cout << "String3: " << String3 << std::endl << " Adress: " << &String3 << std::endl;
		std::cout << "String4: " << String4 << std::endl << " Adress: " << &String4 << std::endl;
		getchar();
	}
	return 0;
}


#include "Shmem.h"
#include <iostream>
#include <string>
#include <sys/wait.h>

int main()
{
	HW::shmem::Map<std::string, std::string> map(1024);
	map.insert({"ABC", "ABCDEFGHIJKLMNOPQRSTUVWXYZ"});
	int pid = fork();
	if (pid < 0) {
		return 1;
	}
	else if (pid == 0) {
		std::cout << map["ABC"] << std::endl;
		map.insert({"123", "1234567891011121314151617181920"});
		map.set_value({"ABC", "ABBACDDCEFFEGHHGIJJI"});
	}
	else {
		wait(nullptr);
		for (auto i = map.crbegin(); i != map.crend(); ++i) {
			std::cout << i->first << " " << i->second << std::endl;
		}
		map.erase("123");
		for (auto i = map.cbegin(); i != map.cend(); ++i) {
			std::cout << i->first << " " << i->second << std::endl;
		}
	}
	return 0;
}

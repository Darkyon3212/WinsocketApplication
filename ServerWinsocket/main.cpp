#include <iostream>

#include "WinsocketManager.h"


int main()
{
	//Initiziate winsocket
	WinsocketManager::Initialize();

	SOCKET socket;
	addrinfo* result =  WinsocketManager::GetInstance().CreateSocket("27015", WinsocketManager::Protocol::UDP, socket);

	if (result != nullptr)
	{
		//int success = WinsocketManager::GetInstance().InitSocket(socket, result);

		//if (success == 0)
	//	{
		//	std::cout << "Successfully created socket!" << std::endl;
		//}
	}

	system("pause");

    return 0;
}


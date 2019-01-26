#include <iostream>

#include "WinsocketManager.h"
#include "ThreadManager.h"
#include "MessageJob.h"

int main()
{
	//Initiziate winsocket
	WinsocketManager::Initialize();
	ThreadManager::Initialize();

	ThreadManager::GetInstance().m_threads.push_back(std::thread(ThreadManager::ThreadRun));
	ThreadManager::GetInstance().AddJob(new MessageJob());

	//ThreadManager::ThreadRun();

	while (ThreadManager::GetInstance().m_threads.size() > 0) {}

	system("pause");

    return 0;
}


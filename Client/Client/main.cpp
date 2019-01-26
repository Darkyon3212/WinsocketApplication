#include <iostream>

#include "WinsocketManager.h"
#include "ThreadManager.h"

#include "MessageSendJob.h"
#include "ImageRecJob.h"

int main()
{
	WinsocketManager::Initialize();
	ThreadManager::Initialize();

	ThreadManager::GetInstance().m_threads.push_back(std::thread(ThreadManager::ThreadRun));
	//ThreadManager::GetInstance().AddJob(new MessageSendJob());
	ThreadManager::GetInstance().AddJob(new ImageRecJob());

	while (ThreadManager::GetInstance().m_threads.size() > 0) {}

	system("pause");


    return 0;
}


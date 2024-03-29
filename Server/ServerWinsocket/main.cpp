#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "WinsocketManager.h"
#include "ThreadManager.h"
#include "ServerManager.h"

#include "MessageJob.h"
#include "ImageSendJob.h"
#include "NetworkJob.h"
#include "ChunkSendJob.h"


class Split
{
public:
	static void split(const std::string &s, char delim, std::vector<std::string> &elems);
};

int main()
{
	//Initiziate winsocket
	WinsocketManager::Initialize();
	ThreadManager::Initialize();
	ServerManager::Initialize();

	for (int i = 0; i < 3; i++)
	{
		ThreadManager::GetInstance().m_threads.push_back(std::thread(ThreadManager::ThreadRun));
	}

	ThreadManager::GetInstance().AddJob(new NetworkJob(NetworkJob::EConnect));

	//ThreadManager::ThreadRun();

	while (true) 
	{
		std::string line;
		std::getline(std::cin, line);
		std::vector<std::string> words = std::vector<std::string>();

		Split::split(line, ' ', words);

		if (words.size() > 0)
		{

			if (words[0] == "sendTGA")
			{
				if (words.size() > 2)
				{
					const char* filepath = words[1].c_str();
					int clientID = std::stoi(words[2].c_str());

					if (clientID >= 0 && clientID < ServerManager::GetInstance().GetAvaibleIPs().size())
					{
						std::string clientIP = ServerManager::GetInstance().GetAvaibleIPs().at(clientID);

						ThreadManager::GetInstance().AddJob(new ImageSendJob(filepath, clientIP));
					}
					else
					{
						std::cout << "ClientID is out of range of avaible ips" << std::endl;
					}
				}
				else
				{
					std::cout << "Missing filepath argument" << std::endl;
				}
			}
			else if (words[0] == "clientLS")
			{
				for (int i = 0; i < ServerManager::GetInstance().GetAvaibleIPs().size(); i++)
				{
					std::cout << i << ": " << ServerManager::GetInstance().GetAvaibleIPs().at(i) << std::endl;
				}
			}
			else if (words[0] == "kick")
			{
				if (words.size() > 1)
				{
					int clientID = std::stoi(words[1].c_str());

					if (clientID >= 0 && clientID < ServerManager::GetInstance().GetAvaibleIPs().size())
					{
						std::string clientIP = ServerManager::GetInstance().GetAvaibleIPs().at(clientID);

						ThreadManager::GetInstance().AddJob(new NetworkJob(NetworkJob::EKick, clientIP));
					}
					else
					{
						std::cout << "ClientID is out of range of avaible ips" << std::endl;
					}
				}
			}
			else if (words[0] == "private")
			{
				if (words.size() > 2)
				{
					int clientID = std::stoi(words[1].c_str());
					std::string message = words[2];

					if (clientID >= 0 && clientID < ServerManager::GetInstance().GetAvaibleIPs().size())
					{
						std::string clientIP = ServerManager::GetInstance().GetAvaibleIPs().at(clientID);

						ThreadManager::GetInstance().AddJob(new MessageJob(MessageJob::EPrivate, clientIP, message));
					}
					else
					{
						std::cout << "ClientID is out of range of avaible ips" << std::endl;
					}
				}
			}
			else if (words[0] == "broadcast")
			{
				if (words.size() > 1)
				{
					std::string message = words[1];

					ThreadManager::GetInstance().AddJob(new MessageJob(MessageJob::EBroadcast, "", message));
				}
			}
			else if (words[0] == "sendTGAChunk")
			{
				if (words.size() > 5)
				{
					int clientID = std::stoi(words[2].c_str());
					std::string filepath = words[1];
					int xOffset = std::stoi(words[3].c_str());
					int yOffset = std::stoi(words[4].c_str());
					int width = std::stoi(words[5].c_str());
					int height = std::stoi(words[6].c_str());

					if (clientID >= 0 && clientID < ServerManager::GetInstance().GetAvaibleIPs().size())
					{
						std::string clientIP = ServerManager::GetInstance().GetAvaibleIPs().at(clientID);

						ThreadManager::GetInstance().AddJob(new ChunkSendJob(filepath, clientIP, xOffset, yOffset, width, height));
					}
					else
					{
						std::cout << "ClientID is out of range of avaible ips" << std::endl;
					}
				}
			}
		}
	}

	system("pause");

    return 0;
}

void Split::split(const std::string& s, char delim, std::vector<std::string>& elems)
{
	std::string word = "";
	for (int i = 0; i < s.size(); i++)
	{ 
		if (s.at(i) != delim && s.at(i) != '"')
		{
			word += s.at(i);
		}
		else if (s.at(i) == '"')
		{
			int index = s.find('"', i + 1);

			if (index != std::string::npos)
			{
				std::string subString = s.substr(i + 1, index - i - 1);
				elems.push_back(subString);
				word = "";
				i = index;
			}
		}
		else
		{
			elems.push_back(word);
			word = "";
		}
	}

	elems.push_back(word);
}


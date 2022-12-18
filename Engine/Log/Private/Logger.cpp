#include "pch.h"
#include "Log/Public/Logger.h"

void Logger::Warn(const WEngine::WString& exp)
{
	std::cout<<"\033[33m"<<GetTime()<<"Warning:"<<exp<<"\033[37m"<<std::endl;
}

void Logger::Debugger(const WEngine::WString& exp)
{
	std::cout<<"\033[35m"<<GetTime()<<"Debug:"<<exp<<"\033[37m"<<std::endl;
}

void Logger::Loading(const WEngine::WString& exp)
{
	std::cout<<"\033[37m"<<GetTime()<<exp<<"\033[37m"<<std::endl;
}

void Logger::Success(const WEngine::WString& exp)
{
	std::cout<<"\033[32m"<<GetTime()<<exp<<"\033[37m"<<std::endl;
}

void Logger::Error(const WEngine::WString& exp)
{
	std::cout<<"\033[31m"<<GetTime()<<"Error:"<<exp<<"\033[37m"<<std::endl;
}

void Logger::AssertFail(bool judge, const WEngine::WString& exp)
{
	if (!judge)
	{
		Error(exp);
		std::terminate();
	}
}
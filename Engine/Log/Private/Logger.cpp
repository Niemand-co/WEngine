#include "pch.h"
#include "Log/Public/Logger.h"

void Logger::Warn(std::string exp)
{
	std::cout<<"\033[33m"<<GetTime()<<"Warning:"<<exp<<"\033[37m"<<std::endl;
}

void Logger::Debugger(std::string exp)
{
	std::cout<<"\033[35m"<<GetTime()<<"Debug:"<<exp<<"\033[37m"<<std::endl;
}

void Logger::Loading(std::string exp)
{
	std::cout<<"\033[37m"<<GetTime()<<exp<<"\033[37m"<<std::endl;
}

void Logger::Success(std::string exp)
{
	std::cout<<"\033[32m"<<GetTime()<<exp<<"\033[37m"<<std::endl;
}

void Logger::Error(std::string exp)
{
	std::cout<<"\033[31m"<<GetTime()<<"Error:"<<exp<<"\033[37m"<<std::endl;
}

void Logger::AssertFail(bool judge, std::string exp)
{
	if (!judge)
	{
		Error(exp);
		std::terminate();
	}
}
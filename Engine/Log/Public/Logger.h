#pragma once

class Logger
{
public:
	static void Warn(std::string exp = "");
	template<typename T>
	static void Log(T exp = T(0));
	static void Debugger(std::string exp = "");
	static void Loading(std::string exp = "");
	static void Success(std::string exp = "");
	static void Error(std::string exp = "");
	static void AssertFail(bool judge, std::string exp = "");
private:
	static std::string GetTime()
	{
		time_t timer;
		time(&timer);
		tm date;
		gmtime_s(&date, &timer);
		date.tm_hour = (date.tm_hour + 8) % 24;
		char sDate[32];
		strftime(sDate, sizeof(sDate), "[%F %T]", &date);
		std::string s = sDate;
		return s;
	}
};

template<typename T>
void Logger::Log(T exp)
{
	std::cout << "\033[35m" << GetTime() << "Logging:" << exp << "\033[37m" << std::endl;
}

#define RE_LOG(expression) Logger::Log(expression);
#define RE_WARN(expression) Logger::Warn(expression);
#define RE_LOADING(expression) Logger::Loading(expression);
#define RE_SUCCESS(expression) Logger::Success(expression);
#define RE_ERROR(expression) Logger::Error(expression);
#if defined(_DEBUG) || defined(DEBUG)
#define RE_DEBUG(expression) Logger::Debugger(expression);
#define RE_ASSERT(exp, ...) Logger::AssertFail(exp, __VA_ARGS__); 
#else
#define RE_DEBUG(expression) ;
#define RE_ASSERT(exp, expression) ;
#endif
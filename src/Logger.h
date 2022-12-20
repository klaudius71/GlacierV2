#ifndef _LOGGER
#define _LOGGER

#define GLACIER_LOG(key, value) Logger::Log(key, value)

#if _DEBUG
#define GLACIER_DEBUG_LOG(key, value) Logger::Log(key, value)
#else
#define GLACIER_DEBUG_LOG(key, value)
#endif

class Logger
{
private:
	static Logger& Instance();
	Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	~Logger() = default;

	std::unordered_map<std::string, int> ints;
	std::unordered_map<std::string, float> floats;
	std::unordered_map<std::string, double> doubles;
	std::unordered_map<std::string, std::string> strings;

	void log(const char* const key, const int& value);
	void log(const char* const key, const float& value);
	void log(const char* const key, const double& value);
	void log(const char* const key, const std::string& value);

	const int& getInt(const char* const key);
	const int& getInt(const char* const key, const int& default_value);
	const float& getFloat(const char* const key);
	const float& getFloat(const char* const key, const float& default_value);
	const double& getDouble(const char* const key);
	const double& getDouble(const char* const key, const double& default_value);
	const std::string& getString(const char* const key);
	const std::string& getString(const char* const key, const std::string& default_value);

public:
	static void Log(const char* const key, const int& value) { Instance().log(key, value); }
	static void Log(const char* const key, const float& value) { Instance().log(key, value); }
	static void Log(const char* const key, const double& value) { Instance().log(key, value); }
	static void Log(const char* const key, const std::string& value) { Instance().log(key, value); }

	static const int& GetInt(const char* const key) { return Instance().getInt(key); }
	static const int& GetInt(const char* const key, const int& default_value) { return Instance().getInt(key, default_value); }
	static const float& GetFloat(const char* const key) { return Instance().getFloat(key); }
	static const float& GetFloat(const char* const key, const float& default_value) { return Instance().getFloat(key, default_value); }
	static const double& GetDouble(const char* const key) { return Instance().getDouble(key); }
	static const double& GetDouble(const char* const key, const double& default_value) { return Instance().getDouble(key, default_value); }
	static const std::string& GetString(const char* const key) { return Instance().getString(key); }
	static const std::string& GetString(const char* const key, const std::string& default_value) { return Instance().getString(key, default_value); }
};

#endif

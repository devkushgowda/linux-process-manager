#include "utils.hpp"

using namespace dpm;

bool  Utils::GetCurrentExePath(string& _exePath)
{
	return GetAbsoluteFilePath("/proc/self/exe", _exePath);
}

bool  Utils::GetCurrentExeDir(string& _exeDir, bool _withEndingSeperator)
{
	bool res = false;
	if (GetCurrentExePath(_exeDir))
	{
		auto pos = _exeDir.find_last_of('/');
		if (pos != string::npos)
		{
			_exeDir.erase(_withEndingSeperator ? pos + 1 : pos);
			res = true;
		}
	}
	return res;
}

bool  Utils::GetAbsoluteFilePath(string const& _filePath, string& _absolutepath)
{
	_absolutepath = "";

	if (!_filePath.size())
		return false;

	auto fullpath = realpath(_filePath.c_str(), NULL);
	if (fullpath)
	{
		_absolutepath = fullpath;
		free(fullpath);
	}
	return _absolutepath.size() > 0;
}

bool Utils::ResolveRootPath(string& _filePath)
{
	bool res = false;
	const string rootPath = "/root/";
	const string tildePath = "~/";

	if (StringUtil::StartsWith(_filePath, tildePath))
	{
		_filePath = rootPath + _filePath.erase(0, tildePath.length());
		res = true;
	}
	return res;

}


string Utils::GetTimeIntervalString(time_t const& _time)
{
	stringstream res;
	string resString;
	int hours = 0, mins = 0, days = 0;

	int secs = (int)difftime(time(NULL), _time);

	if (secs >= 60)
	{
		res.str("");
		mins = secs / 60;
		secs = (int)secs % 60;
		res << mins % 60 << " minutes, ";
		if (secs > 0)
		{
			res << secs << " seconds.";
		}
		resString = res.str();
	}
	else
	{
		res << secs << " seconds.";
		resString = res.str();
	}

	if (mins >= 60)
	{
		res.str("");
		hours = mins / 60;
		res << hours % 24 << " hours, ";
		resString = res.str() + resString;
	}

	if (hours >= 24)
	{
		res.str("");
		days = hours / 24;
		res << days << " days, ";
		resString = res.str() + resString;
	}

	return resString;
}

int Utils::GetFileSize(string const& _fileName)
{
	if (_fileName.size() < 1)
		return -1;
	ifstream in(_fileName, ifstream::ate | ifstream::binary);
	return (int)in.tellg();
}

bool Utils::IsValidIpAddress(string const& _ipAddress)
{
	struct sockaddr_in sa;
	return -1 != inet_pton(AF_INET, _ipAddress.c_str(), &(sa.sin_addr));
}

bool Utils::IsFileExists(string const& _fileName)
{
	// This function checks for the file in specified path */
	ifstream infile(_fileName.c_str());
	return infile.good();
}

bool Utils::IsDirectoryExists(string const& _dirName)
{
	struct stat info;
	stat(_dirName.c_str(), &info);
	return info.st_mode & S_IFDIR;
}

bool Utils::CreateDirectoryTreeIfNotExists(string const& _dirPath)
{
	bool res = true;
	string dirPath = _dirPath;
	size_t pos = 0;
	string dir;

	if (dirPath[dirPath.size() - 1] != '/') {
		// force trailing / so we can handle everything in loop
		dirPath += '/';
	}

	while ((pos = dirPath.find_first_of('/', pos)) != string::npos) {
		dir = dirPath.substr(0, pos++);
		if (dir.size() == 0) continue; // if leading / first time is 0 length
		if (mkdir(dir.c_str(), 0755) && errno != EEXIST) {
			res = false;
			break;
		}
	}
	return res;
}

void StringUtil::ToUpper(string& _str)
{
	transform(_str.begin(), _str.end(), _str.begin(), ::toupper);
}

bool StringUtil::EndsWith(string const& _str, string const& _ending)
{
	return _str.length() >= _ending.length() && !_str.compare(_str.length() - _ending.length(), _ending.length(), _ending);
}

bool StringUtil::StartsWith(string const& _str, string const& _starting)
{
	return _str.length() >= _starting.length() && !_str.compare(0, _starting.length(), _starting);
}

bool StringUtil::Contains(string const& _str, string const& _subStr)
{
	return (_str.find(_subStr) != string::npos);
}

void StringUtil::ToLower(string& _str)
{
	transform(_str.begin(), _str.end(), _str.begin(), ::tolower);
}

string ConsoleUtil::Red(string _str)
{
	return KRED + _str + RST;
}

string ConsoleUtil::Yellow(string _str)
{
	return KYEL + _str + RST;
}

string ConsoleUtil::Green(string _str)
{
	return KGRN + _str + RST;
}

string ConsoleUtil::Blue(string _str)

{
	return KBLU + _str + RST;
}

string ConsoleUtil::White(string _str)

{
	return KWHT + _str + RST;
}

string ConsoleUtil::Magenta(string _str)
{
	return KMAG + _str + RST;
}

string ConsoleUtil::Bold(string _str)
{
	return KBLD + _str + RST;
}

string ConsoleUtil::UnderLine(string _str)
{
	return KUDL + _str + RST;
}

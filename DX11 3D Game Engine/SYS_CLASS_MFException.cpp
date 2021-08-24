#include "SYS_CLASS_MFException.h"

MFException::MFException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* MFException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str(); // make sure the soonly dead mmemory is stored in safe place
	return whatBuffer.c_str();
}

const char* MFException::GetType() const noexcept
{
	return "Base MF Exception";
}

int MFException::GetLine() const noexcept
{
	return line;
}

const std::string& MFException::GetFile() const noexcept
{
	return file;
}

std::string MFException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
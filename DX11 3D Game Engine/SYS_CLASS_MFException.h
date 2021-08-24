#pragma once
#include <exception>
#include <string>

class MFException : public std::exception
{
public:
	MFException(int line, const char* file) noexcept;
	// construct all info into one str
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string whatBuffer;
};
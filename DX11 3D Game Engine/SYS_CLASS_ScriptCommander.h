#pragma once
#include <vector>
#include <string>
#include "SYS_CLASS_MFException.h"

/**
 * @brief a simple commandline processor that can trigger mini tools like art resource coordinate flipper
 * It could prevent main engine running, which is a bad practice (exception affects control flow)
 */
class ScriptCommander
{
public:
	class Completion : public MFException
	{
	public:
		Completion(const std::string& content) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
	private:
		std::string content;
	};
	class Exception : public MFException
	{
	public:
		Exception(int line, const char* file, const std::string& script = "", const std::string& message = "") noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
	private:
		std::string script;
		std::string message;
	};
public:
	ScriptCommander(const std::vector<std::string>& args);
};
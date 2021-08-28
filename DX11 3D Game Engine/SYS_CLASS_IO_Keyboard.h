#pragma once

#include <queue>
#include <bitset>
#include <sstream>
#include <optional>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}
		bool IsPress() const noexcept {	return type == Type::Press;	}
		bool IsRelease() const noexcept	{ return type == Type::Release;	}
		unsigned char GetCode() const noexcept { return code; }
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// key event stuff
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	// pull from the event queue
	std::optional<Keyboard::Event> ReadKey() noexcept;
	// check the event queue
	bool KeyIsEmpty() const noexcept;
	// clear the event queue
	void FlushKey() noexcept;
	// char event stuff - used for text input
	std::optional<char> ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	// autorepeat control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;
private: // expose them to FRIEND window
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u; // total 256 keycodes
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates; // accessible with index
	// two types of events, so two buffers 
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};
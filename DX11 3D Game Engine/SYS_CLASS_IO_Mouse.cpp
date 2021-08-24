#include "SYS_CLASS_IO_Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x,y };
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return rightIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::Flush() noexcept
{
	buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	leftIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	leftIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	rightIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	rightIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
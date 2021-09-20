#pragma once
// define this before #include "SYS_SET_FilterWinH.h" to wnable win stuff 
#include "SYS_SET_FilterWinH.h"
#include "SYS_SET_ConditionalNoexcept.h"
#include "SYS_CLASS_MFException.h"

#include <assert.h>
#include <memory>
#include <string>

namespace GPipeline
{
	class Surface
	{
	public:
		class Color
		{
		public:
			/// stores ARGB values (packed together)
			unsigned int dword;
		public:
			constexpr Color() noexcept : dword()
			{}
			constexpr Color(const Color& col) noexcept
				:
				dword(col.dword)
			{}
			constexpr Color(unsigned int dw) noexcept
				:
				dword(dw)
			{}
			constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) noexcept
				:
				dword((x << 24u) | (r << 16u) | (g << 8u) | b)
			{}
			constexpr Color(unsigned char r, unsigned char g, unsigned char b) noexcept
				:
				dword((r << 16u) | (g << 8u) | b)
			{}
			constexpr Color(Color col, unsigned char x) noexcept
				:
				Color((x << 24u) | col.dword)
			{}
			Color& operator =(Color color) noexcept
			{
				dword = color.dword;
				return *this;
			}
			constexpr unsigned char GetX() const noexcept
			{
				return dword >> 24u;
			}
			constexpr unsigned char GetA() const noexcept
			{
				return GetX();
			}
			constexpr unsigned char GetR() const noexcept
			{
				return (dword >> 16u) & 0xFFu;
			}
			constexpr unsigned char GetG() const noexcept
			{
				return (dword >> 8u) & 0xFFu;
			}
			constexpr unsigned char GetB() const noexcept
			{
				return dword & 0xFFu;
			}
			void SetX(unsigned char x) noexcept
			{
				dword = (dword & 0xFFFFFFu) | (x << 24u);
			}
			void SetA(unsigned char a) noexcept
			{
				SetX(a);
			}
			void SetR(unsigned char r) noexcept
			{
				dword = (dword & 0xFF00FFFFu) | (r << 16u);
			}
			void SetG(unsigned char g) noexcept
			{
				dword = (dword & 0xFFFF00FFu) | (g << 8u);
			}
			void SetB(unsigned char b) noexcept
			{
				dword = (dword & 0xFFFFFF00u) | b;
			}
		};
	public:
		class Exception : public MFException
		{
		public:
			Exception(int line, const char* file, std::string note) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			const std::string& GetNote() const noexcept;
		private:
			std::string note;
		};
	public:
		//Surface(unsigned int width, unsigned int height, unsigned int pitch) noexcept;
		Surface(unsigned int width, unsigned int height) noexcept;
		Surface(Surface&& source) noexcept;
		Surface(Surface&) = delete;
		Surface& operator=(Surface&& donor) noexcept;
		Surface& operator=(const Surface&) = delete;
		~Surface();
		void Clear(Color fillValue) noexcept;
		void PutPixel(unsigned int x, unsigned int y, Color c) noxnd;
		Color GetPixel(unsigned int x, unsigned int y) const noxnd;
		unsigned int GetWidth() const noexcept;
		unsigned int GetHeight() const noexcept;
		Color* GetBufferPtr() noexcept;
		const Color* GetBufferPtr() const noexcept;
		const Color* GetBufferPtrConst() const noexcept;
		static Surface FromFile(const std::string& name);
		void Save(const std::string& filename) const;
		void Copy(const Surface& src) noxnd;
	private:
		Surface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) noexcept;
	private:
		std::unique_ptr<Color[]> pBuffer;
		unsigned int width;
		unsigned int height;
	};
}
#pragma once
/// Won't throw exception in release build
#define noxnd noexcept(!IS_DEBUG)

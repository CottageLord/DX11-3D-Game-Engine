#pragma once

// error exception helper macro
#define MFWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,(hr) )
#define MFWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define MFWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ ) 
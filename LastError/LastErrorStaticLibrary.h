#pragma once
#include<Windows.h>

namespace LastErrorStaticLibrary
{
	class LastError
	{
	public:
		static LPSTR FormatLastError(DWORD dwErrorID);

		static VOID PrintLastError(DWORD dwErrorID);
	};
}
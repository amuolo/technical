#pragma once

#ifndef INCLUDE_UTILS_RESOURCES_H_
#define INCLUDE_UTILS_RESOURCES_H_

#include <windows.h>

namespace tech
{
	int get_used_memory_percentage()
	{
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		return statex.dwMemoryLoad;
	}

}


#endif
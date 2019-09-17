#pragma once

// Generic Definitions
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

// Windows Includes
#include <windows.h>
#include <Shellapi.h>

#undef min
#undef max

// Std Includes
#include <iostream>
#include <sstream>
#include <memory>
#include <thread>
#include <vector>
#include <map>

// Namespaces
using namespace std::literals;
using namespace string_literals;

// Dependency Definitions

// Dependency Includes
// #include <steam\steam_api.h>

// Project Definitions
#define ZONETOOL_VERSION		100
#define ZONETOOL_VERSION_STR	"r100"
#define ZONETOOL_BRANCH			"master-public"

// Project Includes
#include "ZoneTool.hpp"

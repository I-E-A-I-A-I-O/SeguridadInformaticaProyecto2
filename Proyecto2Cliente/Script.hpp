#pragma once

#include "natives.h"
#include "Poco/Net/Net.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/HTTPClientSession.h"
#include <filesystem>
#include <Shlobj.h>
#include <vector>
#include <string>
#include <Windows.h>
#include <chrono>
#include <ctime>

void ScriptMain();
void ThreadMain();
void OnAbort();

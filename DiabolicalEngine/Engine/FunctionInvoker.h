#pragma once
#include "Threading/ThreadsafeVector.h"
#include <functional>

class FunctionInvoker 
{
private:
	static ThreadsafeVector<std::function<void()>> Functions;
public:
	static void Defer(std::function<void()> Function);
	static void InvokeAll();
};
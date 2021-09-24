#include "FunctionInvoker.h"

ThreadsafeVector<std::function<void()>> FunctionInvoker::Functions;

void FunctionInvoker::Defer(std::function<void()> Function)
{
	Functions.Insert(Function);
}

void FunctionInvoker::InvokeAll()
{
	for (int Index = 0; Index < Functions.Size(); Index++)
	{
		std::function<void()> Func = *Functions.GetAt(Index);
		Func();
	}
	Functions.Clear();
}
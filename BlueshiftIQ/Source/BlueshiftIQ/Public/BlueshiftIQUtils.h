// Copyright (c) Blueshift Interactive Ltd (2026)

#pragma once

namespace Blueshift::IQ::Utils
{
	void RunOnGameThread(const TFunctionRef<void()> Func);
	FVector2D GetScreenResolution();
}
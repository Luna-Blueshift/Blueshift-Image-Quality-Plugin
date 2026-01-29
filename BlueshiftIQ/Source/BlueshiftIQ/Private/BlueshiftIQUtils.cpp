// Copyright (c) Blueshift Interactive Ltd (2026)

#include "BlueshiftIQUtils.h"

/// UE includes
#include "GameFramework/GameUserSettings.h"

using namespace Blueshift::IQ;

void Utils::RunOnGameThread(const TFunctionRef<void()> Func)
{
	if (IsInGameThread() || IsInParallelGameThread())
	{
		Func();
	}
	else
	{
		AsyncTask(ENamedThreads::GameThread, [Func]()
		{
			Func();
		});
	}
}

FVector2D Utils::GetScreenResolution()
{
	const UGameUserSettings* const GameUserSettings = UGameUserSettings::GetGameUserSettings();
	if (GameUserSettings == nullptr)
	{
		return FVector2D::ZeroVector;
	}

	const FIntPoint ScreenResolution = GameUserSettings->GetScreenResolution();
	return FVector2D(static_cast<float>(ScreenResolution.X), static_cast<float>(ScreenResolution.Y));
}
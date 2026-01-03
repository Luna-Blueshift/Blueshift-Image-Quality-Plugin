// Copyright (c) Blueshift Interactive Ltd (2026)

#include "Manager/BlueshiftIQManager.h"

/// Blueshift includes
#include "BlueshiftIQPCH.h"
#include "BlueshiftIQLog.h"
#include "BlueshiftIQUtils.h"

/// FSR includes
#if WITH_FSR
#include "FFXFSRSettings.h"
#endif
#if WITH_FSR4
#include "FFXFSR4Settings.h"
#elif WITH_FSR3
#include "FFXFSR3Settings.h"
#endif

using namespace Blueshift::IQ;

bool UBlueshiftIQManager::IsFSRSupported() const
{
	return WITH_FSR;
}

bool UBlueshiftIQManager::IsFFISupported() const
{
	return WITH_FSR;
}

void UBlueshiftIQManager::UpdateFSREnabled()
{
#if WITH_FSR
	check(IsInGameThread());
	
	if (IsFSRSupported())
	{
#if WITH_FSR_GENERIC
		CVarEnableFSR->Set(DesiredUpscaler == EBlueshiftUpscalerMode::FSR);
#elif WITH_FSR4
		CVarEnableFSR4->Set(DesiredUpscaler == EBlueshiftUpscalerMode::FSR);
#elif WITH_FSR3
		CVarEnableFSR3->Set(DesiredUpscaler == EBlueshiftUpscalerType::FSR);
#endif
	}
#endif
}

void UBlueshiftIQManager::ApplyFSRSettings()
{
#if WITH_FSR
	check(IsInGameThread());
	
	if (DesiredUpscaler != EBlueshiftUpscalerMode::FSR
		|| IsFSRSupported() == false)
	{
		return;
	}
	
	IConsoleVariable* const CVarScreenPercentage = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	const float OldScreenPercentage = CVarScreenPercentage->GetFloat();
	
	// Quality Mode
	FixFSRMode();
	int32 OldQualityMode = 0;
	int32 NewQualityMode = 0;
#if WITH_FSR_GENERIC
	OldQualityMode = CVarFSRQualityMode->GetInt();
	CVarFSRQualityMode->Set(FSRMode);
	NewQualityMode = CVarFSRQualityMode->GetInt();
#elif WITH_FSR4
	OldQualityMode = CVarFSR4QualityMode->GetInt();
	CVarFSR4QualityMode->Set(FSRMode);
	NewQualityMode = CVarFSR4QualityMode->GetInt();
#elif WITH_FSR3
	OldQualityMode = CVarFSR4QualityMode->GetInt();
	CVarFSR3QualityMode->Set(FSRMode);
	NewQualityMode = CVarFSR3QualityMode->GetInt();
#endif
	UE_LOG(LogBlueshiftImageQuality, Log, TEXT("%hs: FSR Quality Mode changed from %i to %i (desired: %i)"), __FUNCTION__, OldQualityMode, NewQualityMode, FSRMode);
	UE_LOG(LogBlueshiftImageQuality, Log, TEXT("%hs: FSR changed r.ScreenPercentage from %f to %f"), __FUNCTION__, OldScreenPercentage, CVarScreenPercentage->GetFloat());

	// Sharpness
	FixFSRSharpness();
	float OldSharpness = 0.0f;
	float NewSharpness = 0.0f;
#if WITH_FSR_GENERIC
	OldSharpness = CVarFSRSharpness->GetFloat();
	CVarFSRSharpness->Set(FSRSharpness);
	NewSharpness = CVarFSRSharpness->GetFloat();
#elif WITH_FSR4
	OldSharpness = CVarFSR4Sharpness->GetFloat();
	CVarFSR4Sharpness->Set(FSRSharpness);
	NewSharpness = CVarFSR4Sharpness->GetFloat();
#elif WITH_FSR3
	OldSharpness = CVarFSR3Sharpness->GetFloat();
	CVarFSR3Sharpness->Set(FSRSharpness);
	NewSharpness = CVarFSR3Sharpness->GetFloat();
#endif
	UE_LOG(LogBlueshiftImageQuality, Log, TEXT("%hs: FSR Sharpness changed from %f to %f (desired: %f)"), __FUNCTION__, OldSharpness, NewSharpness, FSRSharpness);

	// Frame Interpolation (aka. Frame Gen)
#if WITH_FSR4 || WITH_FSR3
	CVarEnableFFXFI->Set(bDesiredFrameGen);
	UE_LOG(LogBlueshiftImageQuality, Log, TEXT("%hs: FSR Frame Interpolation: %s"), __FUNCTION__, bDesiredFrameGen ? TEXT("on") : TEXT("off"));
#endif
#endif
}

void UBlueshiftIQManager::SetFSRToDefaults()
{
	FSRMode = 0;
	FSRSharpness = 0.0f;
}

void UBlueshiftIQManager::FixFSRMode()
{
	// Quality Modes defined in CVarFSR4QualityMode
	FSRMode = FMath::Clamp(FSRMode, 0, 4);
}

void UBlueshiftIQManager::FixFSRSharpness()
{
	FSRSharpness = FMath::Max(FSRSharpness, 0.0f);
}

void UBlueshiftIQManager::SetFSRMode(const int32 InFSRMode)
{
	FSRMode = InFSRMode;
	FixFSRMode();
}

void UBlueshiftIQManager::SetFSRSharpness(const float InSharpness)
{
	FSRSharpness = InSharpness;
	FixFSRSharpness();
}
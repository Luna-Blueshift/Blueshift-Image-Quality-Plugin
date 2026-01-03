// Copyright (c) Blueshift Interactive Ltd (2026)

#pragma once

/// DLSS defines
#ifndef WITH_DLSS
#define WITH_DLSS 0
#endif


/// FSR defines
#ifndef WITH_FSR4
#define WITH_FSR4 0
#endif

#ifndef WITH_FSR3
#define WITH_FSR3 0
#endif

// Define WITH_FSR if we're using the legacy FSR3/FSR4 separate plugins
// If we're using generic FSR then this will already be set, and thus ignored on compile time
#ifndef WITH_FSR
#define WITH_FSR WITH_FSR3 || WITH_FSR4
#endif

#ifndef WITH_FSR_GENERIC
#define WITH_FSR_GENERIC 0
#endif


/// XeSS defines
#ifndef WITH_XESS
#define WITH_XESS 0
#endif
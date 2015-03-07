// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#pragma once
#include "DebugDefine.h"
#if defined WindowPhone
#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#if defined WindowStore
#include <mfreadwrite.h>
#endif

class SoundFileReader
{
public:
	SoundFileReader(_In_ Platform::String^ soundFileName);

	Platform::Array<byte>^ GetSoundData() const;
	WAVEFORMATEX* GetSoundFormat() const;

private:
	Platform::Array<byte>^    m_soundData;
	Platform::Array<byte>^    m_soundFormat;
};

#endif
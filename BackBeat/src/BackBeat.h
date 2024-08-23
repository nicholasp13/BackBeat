#pragma once

// For BackBeat applications

#include <stdio.h>

#include "BackBeat/Core/Application.h"
#include "BackBeat/Core/Core.h"
#include "BackBeat/Core/FileDialog.h"
#include "BackBeat/Core/FileSystem.h"
#include "BackBeat/Core/Input.h"
#include "BackBeat/Core/KeyCodes.h"
#include "BackBeat/Core/Layer.h"
#include "BackBeat/Core/LayerStack.h"
#include "BackBeat/Core/Log.h"
#include "BackBeat/Core/MouseCodes.h"
#include "BackBeat/Core/Timer.h"
#include "BackBeat/Core/Window.h"

#include "BackBeat/Audio/Audio.h"
#include "BackBeat/Audio/AudioSystem.h"
#include "BackBeat/Audio/Helpers/int24.h"
#include "BackBeat/Audio/FileIO/WAVFileBuilder.h"
#include "BackBeat/Audio/PlayBack/PlayerManager.h"
#include "BackBeat/Audio/Recorder/RecorderManager.h"
#include "BackBeat/Audio/Instruments/Sampler/Sampler.h"
#include "BackBeat/Audio/Instruments/Synth/Synth.h"

#include "BackBeat/ImGui/ImGuiWidgets.h"

#ifdef BB_PLATFORM_WINDOWS
	#include "Platform/Windows/Windows.h"
	#include "Platform/Windows/WindowsWindow.h"
	#include "Platform/Windows/Audio/WindowsAudioSystem.h"
#endif
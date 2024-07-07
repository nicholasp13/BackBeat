#pragma once

#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <mmeapi.h>

#include "BackBeat/Core/Log.h"
namespace BackBeat {

	namespace Windows {

		// ---- Windows Constants ---- //

		// Window API type REFERENCE_TIME conversions
		constexpr int
			ReftimesPerSecond = 10000000,
			ReftimesPerMillisecond = 10000;
		// Window API struct IDs
		constexpr IID IID_IAudioClient = __uuidof(IAudioClient);
		constexpr CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
		constexpr IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
		constexpr REFIID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

		// ---- Helper static functions ---- //

		// Checks result with possible Windows error codes and logs which error was found
		// Returns true if the result was successful, else returns false as an error was found
		static bool CheckFailureHR(HRESULT hr)
		{
			switch (hr)
			{

			// SUCCESS CASE
			case (S_OK):
			{
				return true;
			}

			// Windows API Function(s): CoInitializeEx()
			case (S_FALSE):
			{
				BB_CORE_ERROR("Windows Error S_FALSE has occured.");
				return false;
			}

			// Windows API Function(s): 
			// CoCreateInstance(), GetDefaultAudioEndpoint(), IMMDevice::Activate(), IAudioClient::GetMixFormat(),
			// IAudioClient::GetDevicePeriod(), IAudioClient::Initialize(), IAudioClient::GetBufferSize(),
			// IAudioClient::GetService(), IAudioClient::GetCurrentPadding(), IAudioRenderClient::GetBuffer()
			case (E_POINTER):
			{
				BB_CORE_ERROR("Windows Error E_POINTER has occured.");
				return false;
			}

			// Windows API Function(s): 
			// GetDefaultAudioEndpoint(), IMMDevice::Activate(), IAudioClient::Initialize(),
			// IAudioRenderClient::ReleaseBuffer()
			case (E_INVALIDARG):
			{
				BB_CORE_ERROR("Windows Error E_INVALIDARG has occured.");
				return false;
			}

			// Windows API Function(s): GetDefaultAudioEndpoint()
			case (E_NOTFOUND):
			{
				BB_CORE_ERROR("Windows Error E_NOTFOUND has occured.");
				return false;
			}

			// Windows API Function(s): 
			// GetDefaultAudioEndpoint(), IMMDevice::Activate(), IAudioClient::GetMixFormat(), IAudioClient::Initialize()
			case (E_OUTOFMEMORY):
			{
				BB_CORE_ERROR("Windows Error E_OUTOFMEMORY has occured.");
				return false;
			}

			// Windows API Function(s): CoCreateInstance(), IMMDevice::Activate(), IAudioClient::GetService()
			case (E_NOINTERFACE):
			{
				BB_CORE_ERROR("Windows Error E_NOINTERFACE has occured");
				return false;
			}

			// Windows API Function(s): CoInitializeEx()
			case (RPC_E_CHANGED_MODE):
			{
				BB_CORE_ERROR("Windows Error RPC_E_CHANGED_MODE has occured.");
				return false;
			}

			// Windows API Function(s): CoCreateInstance()
			case (REGDB_E_CLASSNOTREG):
			{
				BB_CORE_ERROR("Windows Error REGDB_E_CLASSNOTREG has occured.");
				return false;
			}

			// Windows API Function(s): CoCreateInstance()
			case (CLASS_E_NOAGGREGATION):
			{
				BB_CORE_ERROR("Windows Error CLASS_E_NOAGGREGATION has occured");
				return false;
			}

			// Windows API Function(s): 
			// IMMDevice::Activate(), IAudioClient::GetMixFormat(), IAudioClient::GetDevicePeriod(),
			// IAudioClient::Initialize(), IAudioClient::GetBufferSize(), IAudioClient::GetService(),
			// IAudioClient::GetCurrentPadding(), IAudioClient::Start(), IAudioRenderClient::GetBuffer(),
			// IAudioRenderClient::ReleaseBuffer()
			case (AUDCLNT_E_DEVICE_INVALIDATED):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_DEVICE_INVALIDATED has occured");
				return false;
			}

			// Windows API Function(s): 
			// IAudioClient::GetMixFormat(), IAudioClient::GetDevicePeriod(), IAudioClient::Initialize(),
			// IAudioClient::GetBufferSize(), IAudioClient::GetService(), IAudioClient::GetCurrentPadding(),
			// IAudioClient::Start(), IAudioClient::Stop(), IAudioRenderClient::GetBuffer(),
			// IAudioRenderClient::ReleaseBuffer()
			case (AUDCLNT_E_SERVICE_NOT_RUNNING):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_SERVICE_NOT_RUNNING has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_ALREADY_INITIALIZED):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_ALREADY_INITIALIZED has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize(), IAudioClient::GetService()
			case (AUDCLNT_E_WRONG_ENDPOINT_TYPE):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_WRONG_ENDPOINT_TYPE has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize(), IAudioRenderClient::ReleaseBuffer()
			case (AUDCLNT_E_BUFFER_SIZE_ERROR):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_BUFFER_SIZE_ERROR has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_CPUUSAGE_EXCEEDED):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_CPUUSAGE_EXCEEDED has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_DEVICE_IN_USE):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_CPUUSAGE_EXCEEDED has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_ENDPOINT_CREATE_FAILED):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_ENDPOINT_CREATE_FAILED has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_INVALID_DEVICE_PERIOD):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_INVALID_DEVICE_PERIOD has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_UNSUPPORTED_FORMAT):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_UNSUPPORTED_FORMAT has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_EXCLUSIVE_MODE_NOT_ALLOWED has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Initialize()
			case (AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_BUFDURATION_PERIOD_NOT_EQUAL has occured");
				return false;
			}

			// Windows API Function(s): 
			// IAudioClient::GetBufferSize(), IAudioClient::GetService(),
			// IAudioClient::GetCurrentPadding(), IAudioClient::Start(), IAudioClient::Stop()
			case (AUDCLNT_E_NOT_INITIALIZED):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_NOT_INITIALIZED has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Start()
			case (AUDCLNT_E_NOT_STOPPED):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_NOT_STOPPED has occured");
				return false;
			}

			// Windows API Function(s): IAudioClient::Start()
			case (AUDCLNT_E_EVENTHANDLE_NOT_SET):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_EVENTHANDLE_NOT_SET has occured");
				return false;
			}

			// Windows API Function(s): IAudioRenderClient::GetBuffer()
			case (AUDCLNT_E_BUFFER_ERROR):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_BUFFER_ERROR has occured");
				return false;
			}

			// Windows API Function(s): IAudioRenderClient::GetBuffer()
			case (AUDCLNT_E_BUFFER_TOO_LARGE):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_BUFFER_TOO_LARGE has occured");
				return false;
			}

			// Windows API Function(s): IAudioRenderClient::GetBuffer(), IAudioRenderClient::ReleaseBuffer()
			case (AUDCLNT_E_OUT_OF_ORDER):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_OUT_OF_ORDER has occured");
				return false;
			}

			// Windows API Function(s): IAudioRenderClient::GetBuffer()
			case (AUDCLNT_E_BUFFER_OPERATION_PENDING):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_BUFFER_OPERATION_PENDING has occured");
				return false;
			}

			// Windows API Function(s): IAudioRenderClient::ReleaseBuffer()
			case (AUDCLNT_E_INVALID_SIZE):
			{
				BB_CORE_ERROR("Windows Error AUDCLNT_E_INVALID_SIZE has occured");
				return false;
			}

			default:
			{
				BB_CORE_ERROR("Unknown Windows has occured");
				return false;
			}

			}

		}

		// Checks result with possible Windows error codes and logs which error was found
		// Returns true if the result was successful, else returns false as an error was found
		static bool CheckFailureMR(MMRESULT mr)
		{
			switch (mr)
			{

			// SUCCESS CASE
			case (MMSYSERR_NOERROR):
			{
				return true;
			}

			// Windows API Function(s): midiInOpen()
			case (MMSYSERR_ALLOCATED):
			{
				BB_CORE_ERROR("Windows Error MMSYSERR_ALLOCATED has occured.");
				return false;
			}

			// Windows API Function(s): midiInOpen(), midiInGetDevCapsA()
			case (MMSYSERR_BADDEVICEID):
			{
				BB_CORE_ERROR("Windows Error MMSYSERR_BADDEVICEID has occured.");
				return false;
			}

			// Windows API Function(s): midiInOpen()
			case (MMSYSERR_INVALFLAG):
			{
				BB_CORE_ERROR("Windows Error MMSYSERR_INVALFLAG has occured.");
				return false;
			}

			// Windows API Function(s): midiInOpen(), midiInGetDevCapsA()
			case (MMSYSERR_INVALPARAM):
			{
				BB_CORE_ERROR("Windows Error MMSYSERR_INVALPARAM has occured.");
				return false;
			}

			// Windows API Function(s): midiInOpen(), midiInClose(), midiInGetDevCapsA()
			case (MMSYSERR_NOMEM):
			{
				BB_CORE_ERROR("Windows Error MMSYSERR_NOMEM has occured.");
				return false;
			}

			// Windows API Function(s): midiInClose(), midiInStart(), midiInStop()
			case (MMSYSERR_INVALHANDLE):
			{
				BB_CORE_ERROR("Windows Error MMSYSERR_INVALHANDLE has occured.");
				return false;
			}

			// Windows API Function(s): midiInGetDevCapsA()
			case (MMSYSERR_NODRIVER):
			{
				BB_CORE_ERROR("Windows Error MMSYSERR_NODRIVER has occured.");
				return false;
			}

			// Windows API Function(s): midiInClose()
			case (MIDIERR_STILLPLAYING):
			{
				BB_CORE_ERROR("Windows Error MIDIERR_STILLPLAYING has occured.");
				return false;
			}

			default:
			{
				BB_CORE_ERROR("Unknown Windows has occured");
				return false;
			}

			}
		}

// ---- Windows Macros ---- //

#define CHECK_FAILURE_HRESULT( hr ) if (!BackBeat::Windows::CheckFailureHR(hr)) goto FailureExit;
#define CHECK_FAILURE_MMRESULT( mr ) if (!BackBeat::Windows::CheckFailureMR(mr)) goto FailureExit;

	}

}
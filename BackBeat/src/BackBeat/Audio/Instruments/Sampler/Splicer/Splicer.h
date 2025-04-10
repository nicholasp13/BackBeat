#pragma once

#include "BackBeat/Audio/Instruments/Sampler/SampleBuilder.h"
#include "BackBeat/Audio/Fx/TimeStretcher.h"
#include "BackBeat/Audio/Fx/PitchShifter.h"
#include "BackBeat/Audio/Fx/LowPassFilterFx.h"
#include "BackBeat/Audio/Fx/HighPassFilterFx.h"
#include "BackBeat/Audio/Instruments/Synth/SynthBase.h"
#include "SplicerPlayer.h"
namespace BackBeat {

	constexpr unsigned int SplicerMaxBufferSize = 960000; // 20 seconds im 48k buffer rate, mono

	// Unlike instrument objects these parameters are not shared rather each object will have an
	// Update(Params params) function to take in. This is because these process are more costly and 
	// processes a larger data size so Splicer should be called by the user instead of on a loop
	struct SplicerParameters
	{
		float masterGainDB = Audio::DBDefault; // in decibel
		float leftGainDB   = Audio::DBDefault; // in decibel
		float rightGainDB  = Audio::DBDefault; // in decibel

		// Fx object params
		TimeStretcherParameters timeStretcherParams = TimeStretcherParameters();
		PitchShifterParameters pitchShifterParams = PitchShifterParameters();
		LowPassFilterFxParameters lowPassFilterParams = LowPassFilterFxParameters();
		HighPassFilterFxParameters highPassFilterParams = HighPassFilterFxParameters();

		SplicerParameters& operator = (const SplicerParameters& rhs)
		{
			masterGainDB = rhs.masterGainDB;
			leftGainDB = rhs.leftGainDB;
			rightGainDB = rhs.rightGainDB;
			timeStretcherParams = rhs.timeStretcherParams;
			pitchShifterParams = rhs.pitchShifterParams;
			lowPassFilterParams = rhs.lowPassFilterParams;
			highPassFilterParams = rhs.highPassFilterParams;

			return *this;
		}

		bool operator == (const SplicerParameters& rhs)
		{
			const float delta = 0.0001f;
			bool equals = true;

			equals = equals && Audio::EqualsFloat(masterGainDB, rhs.masterGainDB, delta);
			equals = equals && Audio::EqualsFloat(leftGainDB, rhs.leftGainDB, delta);
			equals = equals && Audio::EqualsFloat(rightGainDB, rhs.rightGainDB, delta);
			equals = equals && (timeStretcherParams == rhs.timeStretcherParams);
			equals = equals && (pitchShifterParams == rhs.pitchShifterParams);
			equals = equals && (lowPassFilterParams == rhs.lowPassFilterParams);
			equals = equals && (highPassFilterParams == rhs.highPassFilterParams);

			return equals;
		}

		bool operator != (const SplicerParameters& rhs)
		{
			return !(*this == rhs);
		}
	};

	class Splicer
	{
	public:
		Splicer();
		~Splicer();

		void Update(SplicerParameters params); // Processes the whole sample data with the fx objects
		void Reset(); // Puts original data from m_InputBuffer

		bool SetSampleData(float* sampleData, unsigned int numFrames, AudioProps props);
		bool SetSampleData(std::shared_ptr<Track> track, unsigned int start, unsigned int end);

		inline bool IsPlayable() { return m_Playable; }
		inline bool IsLoaded() { return m_Player.IsLoaded(); }
		inline unsigned int GetInputSize() { return m_SampleInputSize; } // Size of one channels buffer
		inline unsigned int GetOutputSize() { return m_SampleOutputSize; } // Size of one channels buffer
		inline float* GetLeftChannel() { return m_SplicedLeftChannel; }
		inline float* GetRightChannel() { return m_SplicedRightChannel; }
		inline UUID GetID() { return m_Player.GetProc()->GetID(); }
		inline std::shared_ptr<SplicerPlayerProcessor> GetProc() { return m_Player.GetProc(); }
		inline SplicerPlayer* GetPlayer() { return &m_Player; }

	private:
		bool m_Playable;
		unsigned int m_SampleInputSize;  // Total size of input buffer data
		unsigned int m_SampleOutputSize; // Size of one channels buffer

		float m_SplicedRightChannel[SplicerMaxBufferSize] = {};         // Spliced buffers are separated as some fx
		float m_SplicedLeftChannel[SplicerMaxBufferSize] = {};          // need to be calculated by each channel separately
		float m_InputBuffer[SplicerMaxBufferSize * Audio::Stereo] = {};
		float m_TempBuffer[SplicerMaxBufferSize] = {};                  // Needed for some fx to make sure input data is not lost/overwritten 

		AudioProps m_InputProps;

		SplicerPlayer m_Player;

		SplicerParameters m_Params;

		// Fx objects
		TimeStretcher m_TimeStretcher;
		PitchShifter m_PitchShifter;
		LowPassFilterFx m_LowPassFilter;
		HighPassFilterFx m_HighPassFilter;

	private:
		void SplitInputBuffer();
		void ProcessFx();
		void ProcessTimeStretcher();
		void ProcessPitchShifter();
		void ProcessFilters();
		void ApplyGain();
	};

}
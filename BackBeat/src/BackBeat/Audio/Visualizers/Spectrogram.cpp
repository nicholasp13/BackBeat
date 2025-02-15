#include "bbpch.h"

#include "Spectrogram.h"
namespace BackBeat {


	Spectrogram::Spectrogram(unsigned int bufferSize)
		: 
		m_BufferSize(bufferSize),
		m_ComplexBuffer(complexArray(bufferSize))
	{

	}

	Spectrogram::~Spectrogram()
	{

	}

	Spectrogram::Spectrogram(const Spectrogram& other)
		:
		m_BufferSize(other.m_BufferSize),
		m_ComplexBuffer(other.m_ComplexBuffer)
	{
		
	}

	Spectrogram& Spectrogram::operator= (const Spectrogram& rhs)
	{
		m_BufferSize = rhs.m_BufferSize;
		m_ComplexBuffer = rhs.m_ComplexBuffer;
		return *this;
	}

	void Spectrogram::Get(float* input, float* output, unsigned int numSamples)
	{
		if (numSamples > m_BufferSize)
			return;

		Audio::Algorithms::fftw3(input, &m_ComplexBuffer, numSamples);

		float* realNums = m_ComplexBuffer.realNums;
		float* imNums = m_ComplexBuffer.imNums;
		float real = 0.0f;
		float im = 0.0f;

		for (unsigned int i = 0; i < numSamples; i++)
		{
			real = realNums[i];
			im = imNums[i];
			output[i] = sqrtf(real * real + im * im);
		}

	}

	void Spectrogram::GetNormalized(float* input, float* output, unsigned int numSamples)
	{
		if (numSamples > m_BufferSize)
			return;

		Audio::Algorithms::fftw3(input, &m_ComplexBuffer, numSamples);

		float* realNums = m_ComplexBuffer.realNums;
		float* imNums = m_ComplexBuffer.imNums;
		float real = 0.0f;
		float im = 0.0f;
		float x = 0.0f;
		float max = 0.0f;

		for (unsigned int i = 0; i < numSamples; i++)
		{
			real = realNums[i];
			im = imNums[i];
			x = sqrtf(real * real + im * im);

			output[i] = x;

			if (x > max)
				max = x;
		}

		for (unsigned int i = 0; i < numSamples; i++)
		{
			output[i] /= max;
		}

	}

	void Spectrogram::GetNormalizedRMS(float* input, float* output, unsigned int numSamples)
	{
		if (numSamples > m_BufferSize)
			return;

		Audio::Algorithms::fftw3(input, &m_ComplexBuffer, numSamples);

		float* realNums = m_ComplexBuffer.realNums;
		float* imNums = m_ComplexBuffer.imNums;
		float real = 0.0f;
		float im = 0.0f;
		float x = 0.0f;
		float sum = 0.0f;

		for (unsigned int i = 0; i < numSamples; i++)
		{
			real = realNums[i];
			im = imNums[i];
			x = real * real + im * im;
			sum += x;
			output[i] = sqrtf(x);
		}

		float rms = sqrtf(sum / (float)numSamples);

		for (unsigned int i = 0; i < numSamples; i++)
		{
			output[i] /= rms;
		}

	}

}
#include "bbpch.h"

#include <fftw3.h>

#include "Algorithms.h"
namespace BackBeat {

	namespace Audio {

		void Algorithms::fftw3(float* input, complexArray* output, unsigned int numSamples)
		{
			if (!input)
				return;
			if (!output)
				return;
			if (!output->realNums || !output->imNums)
				return;

			fftwf_complex* in = nullptr, 
				*out = nullptr;

			fftwf_plan plan;

			// Allocate fftw variables
			in = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * numSamples);
			out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * numSamples);
			plan = fftwf_plan_dft_1d(numSamples, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

			// Copy input to in[index][REAL]
			for (unsigned int i = 0; i < numSamples; i++)
			{
				in[i][Real] = input[i];
				in[i][Imaginary] = 0.0f;
			}

			// Execute plan as needed
			fftwf_execute(plan);

			// Copy out[index][REAL] to output
			for (unsigned int i = 0; i < numSamples; i++)
			{
				output->realNums[i] = out[i][Real];
				output->imNums[i] = out[i][Imaginary];
			}

			// Free fftw variables
			fftwf_destroy_plan(plan);
			fftwf_free(in); fftwf_free(out);

		}

		// NOTE: The output is not normalized and to get the original data divide output by numSamples
		void Algorithms::ifftw3(complexArray* input, float* output, unsigned int numSamples)
		{
			if (!input)
				return;
			if (!input->realNums || !input->imNums)
				return;
			if (!output)
				return;

			fftwf_complex* in = nullptr,
				*out = nullptr;

			fftwf_plan plan;

			// Allocate fftw variables
			in = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * numSamples);
			out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * numSamples);
			plan = fftwf_plan_dft_1d(numSamples, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

			// Copy input to in[index][REAL]
			auto inputReal = input->realNums;
			auto inputIm = input->imNums;
			for (unsigned int i = 0; i < numSamples; i++)
			{
				in[i][Real] = inputReal[i];
				in[i][Imaginary] = inputIm[i];
			}

			// Execute plan as needed
			fftwf_execute(plan);

			// Copy out[index][REAL] to output
			for (unsigned int i = 0; i < numSamples; i++)
			{
				output[i] = out[i][Real];
			}

			// Free fftw variables
			fftwf_destroy_plan(plan);
			fftwf_free(in); fftwf_free(out);
		}

	}

}
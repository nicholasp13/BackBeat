#include "bbpch.h"

#include <fftw3.h>

#include "Algorithms.h"
namespace BackBeat {

	namespace Audio {

		void Algorithms::fftw3(float* input, complexArray* output, unsigned int numSamples)
		{
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
			}

			// Execute plan as needed
			fftwf_execute(plan);

			// Copy out[index][REAL] to output
			float* outputReal = output->realNums;
			float* outputIm = output->imNums;
			for (unsigned int i = 0; i < numSamples; i++)
			{
				outputReal[i] = out[i][Real];
				outputIm[i] = out[i][Imaginary];
			}

			// Free fftw variables
			fftwf_destroy_plan(plan);
			fftwf_free(in); fftwf_free(out);

		}

	}

}
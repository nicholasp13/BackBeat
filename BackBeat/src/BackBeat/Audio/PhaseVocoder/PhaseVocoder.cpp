#include "bbpch.h"

#include "PhaseVocoder.h"
namespace BackBeat {

	PhaseVocoder::PhaseVocoder()
	{

	}

	PhaseVocoder::~PhaseVocoder()
	{

	}

	// NOTES:
	// - Phase vocoder consists of complimenary of FFT and IFFT processing blocks
	// - Translates real and imaginary componets of FFT output to magnitude and phase arrays
	// - Puts output into "bins" surround (k) frequency including 0Hz DC detector block (which is just the average of the samples)
	//  
	// Steps:
	// 1. Takes N samples in time x(n) into an FFT.
	// 2. Takes N samples from the FFT output real and imaginary arrays and mathmatically massages into an N-point magnitude and angle(phase) arrays
	//    using the equation
	//        - X = a +jb = Real + j Imaginary
	//        - |X| = sqrt(a^2 +b^2)
	//        - PHI = tan^-1(b/a)
	// 
	// - |X| is the maginutude i.e. the amplitude of an oscillator
	// - PHI is the starting phase of the frequency
	// - All exact sample information is lost
	// 
	// 3. Then the phase vocoder does some manipulation to this data
	// 4. IFFT part takes the processed N samples of magnitude and phase and puts each into a bin and in parrallel proces them
	// 5. IFFT output is then summed and outputted into y(n) 
 	//
	// HOW THE VOCODER USES THE PHASE INFO:
	// - At 0 degrees this is the mex amplitude of the sin
	// - How does the vocoder deal with a wave traveling multiple times?
	//   Ans: The FFT gives a range of only -180, 180 degrees (or -pi, pi radians)
	// - Phase vocoders also have a "memory" to store the amount of rotations from one frame to the next 
	//
	// PHASE DEVIATION:
	// - Vocoder takes these frames of info and adds the data together
	// - The hop size (ha) on the analysis side determines how quickly each new  FFT snapshot is taken
	// - If we know the first frame's information and know (ha) we can make a guess on where the phase will be for that frequency
	//   using the frequency of the bin it is in (wk)
	// - However if at the next frame the guess is wrong, the difference between the guess and the actual data is called the
	//   phase deviation phi
	// - phi is then converted back to the range of -pi, pi called the principal argument using a modulo operation.
	// - THe principal argument is expressed as princArg
	// - This gives us the equation
	//       - phase deviation = phase captured - phase predicteed
	//       - phase incoming of the actual frequency = wk * ha + princArg(phase deviation)
	// - In addition to other phase vocoder algos, this info can be used to calculate the actual frequency:
	//       - f (actual frequency)(k) = phase incoming of the actual frequency / (2pi * ha) * fs (sample rate)
	// 
	// CODING THE PHASE VOCODER:
	// - Overlap will be between 0 and 99.9% (100% or less than 0% overlap is meaningless)
	// - First output frame is just zeros
	// - Subsequent output frames are accumalted partly with the previous frame and the rest with zero-padding
	// - No FFT or IFFT process expects the input = output
	// 
	// - Use a circular buffer
	//  
	//

}
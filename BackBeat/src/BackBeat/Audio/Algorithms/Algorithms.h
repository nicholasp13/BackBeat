#pragma once

namespace BackBeat {

	// Indeces for complexArray
	constexpr unsigned int Real = 0, Imaginary = 1;

	struct complexArray
	{

		unsigned int size = 0;
		float* realNums = nullptr;
		float* imNums = nullptr;

		complexArray(unsigned int size = 0)
			:
			realNums(new float[size]),
			imNums(new float[size]),
			size(size)
		{

		}

		~complexArray()
		{
			delete[size] realNums;
			delete[size] imNums;
		}

		complexArray(const complexArray& other)
		{
			delete[size] realNums;
			delete[size] imNums;

			size = other.size;
			realNums = new float[size];
			imNums = new float[size];

			memcpy(realNums, other.realNums, std::size_t(size * sizeof(float)));
			memcpy(imNums, other.imNums, std::size_t(size * sizeof(float)));
		}

		complexArray(complexArray&& rhs) noexcept
		{
			delete[size] realNums;
			delete[size] imNums;

			size = rhs.size;
			realNums = rhs.realNums;
			imNums = rhs.imNums;

			rhs.realNums = nullptr;
			rhs.imNums = nullptr;
		}

		complexArray& operator = (complexArray&& rhs) noexcept
		{
			delete[size] realNums;
			delete[size] imNums;

			size = rhs.size;
			realNums = rhs.realNums;
			imNums = rhs.imNums;

			rhs.realNums = nullptr;
			rhs.imNums = nullptr;

			return *this;
		}

		complexArray& operator = (const complexArray& other)
		{
			delete[size] realNums;
			delete[size] imNums;

			size = other.size;
			realNums = new float[size];
			imNums = new float[size];

			memcpy(realNums, other.realNums, std::size_t(size * sizeof(float)));
			memcpy(imNums, other.imNums, std::size_t(size * sizeof(float)));

			return *this;
		}

		// Treats complexArray as a long single buffer
		float operator [] (unsigned int index)
		{
			if (index >= (size * 2))
				return 0.0f;

			unsigned int actualIndex = (unsigned int)floor((float)index / 2.0f);
			if (index % 2 == 0)
				return realNums[actualIndex];
			else
				return imNums[actualIndex];
		}

		// Same functionality as [][] in terms on indexing
		float Get(unsigned int index1, unsigned int index2)
		{
			if (index2 > Imaginary)
				return 0.0f;
			if (index1 >= size)
				return 0.0f;

			if (index2 == Real)
				return realNums[index1];
			else
				return imNums[index1];
		}

		// Same functionality as [][] in terms on indexing
		void Set(unsigned int index1, unsigned int index2, float value)
		{
			if (index2 > Imaginary)
				return;
			if (index1 >= size)
				return;

			if (index2 == Real)
				realNums[index1] = value;
			else
				imNums[index1] = value;
		}

		// Same functionality as [] in terms on indexing
		void Set(unsigned int index, float value)
		{
			if (index >= (size * 2))
				return;

			unsigned int actualIndex = (unsigned int)floor((float)index / 2.0f);
			if (index % 2 == 0)
				realNums[actualIndex] = value;
			else
				imNums[actualIndex] = value;
		}
	};

	namespace Audio {

		namespace Algorithms {

			void fftw3(float* input, complexArray* output, unsigned int numSamples);
			void ifftw3(complexArray* input, float* output, unsigned int numSamples);

		}

	}


}
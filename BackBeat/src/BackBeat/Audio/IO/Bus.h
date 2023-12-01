#pragma once

// Bus holds the input or output buffers for their specific AudioProcessor
namespace BackBeat {

// NOTE: Might add flags to bus classes

	enum class BusDirection{
		Input = 0,
		Output
	};

	class Bus
	{
	public:

		virtual unsigned int GetBufferSize() { return 0; }
		virtual BusDirection GetDirection() { return BusDirection::Input; }
		virtual bool IsActive() { return true; }

	};
}
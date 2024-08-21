#pragma once

#include "bbpch.h"

// Simple SpinLock that is not currently used by BackBeat

#include <mutex>
namespace BackBeat {

	class SpinLock
	{
	public:
		void Lock() { while (flag.test_and_set()); }
		void Unlock() { flag.clear(); }
		bool TryLock() { return !(flag.test_and_set()); }

	private:
		std::atomic_flag flag = ATOMIC_FLAG_INIT;
	};

}
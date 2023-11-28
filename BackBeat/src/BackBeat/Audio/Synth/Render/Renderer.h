#pragma once

namespace BackBeat {

	class Renderer
	{
	public:
		virtual void Stop() {};
		virtual void Render() {};
		virtual bool IsRendering() { return true; };
	};
}
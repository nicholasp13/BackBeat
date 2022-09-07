#pragma once

#include "BackBeat/Core.h"
#include "Layer.h"

#include <vector>


namespace BackBeat {

	typedef std::vector<Layer*> LAYERS;

	class BACKBEAT_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		LAYERS::iterator begin() { return m_Layers.begin(); }
		LAYERS::iterator end() { return m_Layers.end(); }

	private:
		LAYERS m_Layers;
		LAYERS::iterator m_LayerInsert;
	};
}


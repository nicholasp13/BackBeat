#pragma once

// NOTE: Unused and possible will be deleted later when serialization is fully realized

#include <vector>
#include <memory>

namespace BackBeat {

	// NOTE: Should have a position member that is the main sorted (todo after making headway to ImGui widget)
	struct Snippet
	{
		unsigned int start;
		unsigned int end;
		bool on;
	};

	class Snippets
	{
	public:
		Snippets();
		~Snippets();

		int Add(std::shared_ptr<Snippet> snippet);
		bool Delete(unsigned int index);
		std::shared_ptr<Snippet> Get(unsigned index);

		bool SetMin(unsigned int min);
		bool SetMax(unsigned int max);

		inline unsigned int GetMin() { return m_Min; }
		inline unsigned int GetMax() { return m_Max; }
		inline unsigned int GetSize() { return (unsigned int)m_Snippets.size(); }


	private:
		unsigned int m_Min;
		unsigned int m_Max;
		std::vector<std::shared_ptr<Snippet>> m_Snippets;

	private:
		void Update();
	};

}
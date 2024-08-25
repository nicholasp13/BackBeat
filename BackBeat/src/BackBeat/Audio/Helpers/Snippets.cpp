#include "bbpch.h"

// TODO: Unit test snippets

#include "Snippets.h"
namespace BackBeat {

	Snippets::Snippets()
		: m_Min(0u), m_Max(1u)
	{

	}

	Snippets::~Snippets()
	{

	}

	// @return
	//     -1    - If failed to add due to overlapping with another snippet or if the bounds of the Snippet are outside the min or max
	//     index - Index of snippet in Snippets
	int Snippets::Add(std::shared_ptr<Snippet> snippet)
	{
		if (!snippet)
			return -1;
		if (snippet->start >= snippet->end)
			return -1;
		if (snippet->start < m_Min || snippet->start >= m_Max || snippet->end <= m_Min || snippet->end > m_Max)
			return -1;

		int index = 0;

		if (GetSize() == 0)
		{
			m_Snippets.push_back(snippet);
			return index;
		}

		std::shared_ptr<Snippet> snip = nullptr;
		for (auto itr = m_Snippets.begin(); itr != m_Snippets.end(); itr++)
		{
			snip = *itr;
			if (snip->start > snippet->end)
			{
				m_Snippets.insert(itr, snippet);
				return index;
			}
			index++;
		}

		snip = *(m_Snippets.end() - 1);
		if (snip->end < snippet->start)
		{
			m_Snippets.push_back(snippet);
			return index;
		}

		return -1;
	}

	bool Snippets::Delete(unsigned int index)
	{
		if (index >= GetSize())
			return false;

		m_Snippets.erase(m_Snippets.begin() + index);
		return true;
	}

	std::shared_ptr<Snippet> Snippets::Get(unsigned index)
	{
		if (index >= GetSize())
			return nullptr;

		return m_Snippets[index];
	}

	bool Snippets::SetMin(unsigned int min)
	{
		if (min >= m_Max)
			return false;

		if (m_Min == min)
			return true;

		m_Min = min;
		Update();
		return true;
	}

	bool Snippets::SetMax(unsigned int max)
	{
		if (max <= m_Min)
			return false;

		if (m_Max == max)
			return true;

		m_Max = max;
		Update();
		return true;
	}

	void Snippets::Update()
	{
		std::shared_ptr<Snippet> snip = nullptr;
		for (unsigned int i = 0; i < m_Snippets.size(); i++)
		{
			snip = m_Snippets[i];
			if (snip->start < m_Min || snip-> start >= m_Max || snip->end <= m_Min || snip->end > m_Max)
			{
				m_Snippets.erase(m_Snippets.begin() + i);
				--i;
			}
		}
	}
}
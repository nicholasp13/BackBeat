#pragma once

namespace BackBeat {

	class UUID
	{
	public:
		UUID();
		UUID(unsigned long long id);

		inline operator unsigned long long() const { return m_ID; }
	private:
		unsigned long long m_ID;
	};

}

namespace std {
	template <typename T> struct hash;

	template<>
	struct hash<BackBeat::UUID>
	{
		std::size_t operator()(const BackBeat::UUID& uuid) const
		{
			return (unsigned long long)uuid;
		}
	};

}
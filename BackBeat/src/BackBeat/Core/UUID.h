#pragma once

namespace BackBeat {

	class UUID
	{
	public:
		UUID();
		UUID(unsigned long long id);
		UUID(const UUID&) = default;

		inline UUID operator == (UUID id) { return m_ID == id.m_ID; }
		inline UUID operator != (UUID id) { return m_ID != id.m_ID; }
		inline operator unsigned long long() const { return m_ID; }
		inline std::string ToString() { return std::to_string(m_ID); }


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
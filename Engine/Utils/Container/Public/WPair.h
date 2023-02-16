#pragma once

namespace WEngine
{
	
	template<typename k, typename v>
	class WPair
	{
	public:

		WPair() = default;

		WPair(const k& key, const v& val);

		explicit WPair(const WPair& other)
			: m_key(other.m_key), m_val(other.m_val)
		{
		}

		~WPair() = default;

		k& First() { return m_key; }

		const k& First() const { return m_key; }

		v& Second() { return m_val; }

		const v& Second() const { return m_val; }

	private:

		k m_key;

		v m_val;

	};

	template<typename k, typename v>
	WPair<k, v>::WPair(const k& key, const v& val)
		: m_key(key), m_val(val)
	{
		
	}

	template<typename k, typename v>
	WPair<k, v> MakePair(k key, v val)
	{
		return WPair(key, val);
	}

}
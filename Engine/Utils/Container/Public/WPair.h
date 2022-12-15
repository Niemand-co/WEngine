#pragma once

namespace WEngine
{
	
	template<typename k, typename v>
	class WPair
	{
	public:

		WPair(const k& key, const v& val);

		WPair(const WPair&) = default;

		~WPair() = default;

		const k& First() const { return m_key; }

		const v& Second() const { return m_val; }

	private:

		k m_key;

		v m_val;

	};

	template<typename k, typename v>
	inline WPair<k, v>::WPair(const k& key, const v& val)
		: m_key(key), m_val(val)
	{
		
	}

	template<typename k, typename v>
	WPair<k, v>&& MakePiar(k key, v val)
	{
		return WPair<key, val>;
	}

}
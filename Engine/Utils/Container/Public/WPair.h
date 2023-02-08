#pragma once

namespace WEngine
{
	
	template<typename k, typename v>
	class WPair
	{
	public:

		WPair() = default;

		WPair(const k& key, const v& val);

		WPair(const WPair&) = default;

		~WPair() = default;

		k& First() { return m_key; }

		v& Second() { return m_val; }

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
	WPair<k, remove_const_and_reference<v>> MakePiar(k key, remove_const_and_reference<v> val)
	{
		return WPair(key, val);
	}

}
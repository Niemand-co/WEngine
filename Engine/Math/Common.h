#pragma once

template<typename T>
T Saturate(T t)
{
	if(t > T(1))return T(1);
	else if(t < T(0))return T(0);
	return t;
}
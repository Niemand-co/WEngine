#pragma once
#ifndef SR_TOOL_H
#define SR_TOOL_H

template<typename T>
bool isSameTpye(T para_one, T para_two)
{
	return true; 
}

template<typename T1, typename T2>
bool isSameTpye(T1 para_one, T2 para_two)
{
	return false;
}

template<typename T>
bool isSameType(T para)
{
	return true;
}

#endif
#pragma once

template<typename HandleType>
class WRDGRegistry
{
public:

	WRDGRegistry();

	~WRDGRegistry();

	void Insert(HandleType)

private:

	WEngine::WArray<HandleType> Array;

};

class WRDGPass
{
public:

	WRDGPass();

	~WRDGPass();

};

class WRDGBuilder
{
public:

	WRDGBuilder();

	~WRDGBuilder();

private:

	

	WEngine::WArray<

};
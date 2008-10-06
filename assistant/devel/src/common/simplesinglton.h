

#pragma once

template <typename T>
class CSimpleSinglton
{
public:
	static T& Get()
	{
		static T obj;
		return obj;
	}
};

template <typename T, typename P>
class CSimpleSinglton2
{
public:
	static T& Get(P* p = NULL)
	{
		static T obj(p);
		return obj;
	}
};
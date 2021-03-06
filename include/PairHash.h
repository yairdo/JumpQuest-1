#pragma once
template<class T>
struct ArgsHash {
	std::size_t operator () (const std::pair<T,T>& p) const {
		std::size_t h1 = std::hash<T>()(p.first);
		std::size_t h2 = std::hash<T>()(p.second);
		return h1 ^ h2;
	}
};

template < class T>
struct KeyEqual
{
	bool operator()(const std::pair<T,T>& a1, const std::pair<T,T>& a2) const
	{
		return (a1.first == a2.first && a1.second == a2.second);
	}
};
#pragma once

template<typename A, typename B>
class Pair
{
private:
	A pkey;
	B pvalue;

public:
	Pair() = default;
	Pair(const A& key, const B& value) : pkey(key), pvalue(value) {}
	~Pair() = default;

	const A& getKey() const { return pkey; }
	const B& getValue() const { return pvalue; }
};
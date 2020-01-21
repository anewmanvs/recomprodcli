#ifndef GENTPLMAP
#define GENTPLMAP

#include <utility>
#include <tuple>

typedef std::tuple<long, long, long> lll_t;
typedef std::tuple<long, long> ll_t;

struct khash_lllt : public std::unary_function<lll_t, std::size_t>
{
	std::size_t operator()(const lll_t &t) const
	{
		return std::get<0>(t) ^ std::get<1>(t) ^ std::get<2>(t);
	}
};

struct kequal_lllt : public std::binary_function<lll_t, lll_t, bool>
{
	bool operator()(const lll_t &ta, const lll_t &tb) const
	{
		return (
				std::get<0>(ta) == std::get<0>(tb) &&
				std::get<1>(ta) == std::get<1>(tb) &&
				std::get<2>(ta) == std::get<2>(tb)
			);
	}
};

struct khash_llt : public std::unary_function<ll_t, std::size_t>
{
	std::size_t operator()(const ll_t &t) const
	{
		return std::get<0>(t) ^ std::get<1>(t);
	}
};

struct kequal_llt : public std::binary_function<ll_t, ll_t, bool>
{
	bool operator()(const ll_t &ta, const ll_t &tb) const
	{
		return (
				std::get<0>(ta) == std::get<0>(tb) &&
				std::get<1>(ta) == std::get<1>(tb)
			);
	}
};

#endif  // GENTPLMAP

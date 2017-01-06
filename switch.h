//
//  switch.h
//  switch
//
//  Created by Sandy Martel on 5/1/17.
//  Copyright © 2017 sandy. All rights reserved.
//

#ifndef H_CPP_SWITCHER
#define H_CPP_SWITCHER

#include <type_traits>
#include <tuple>
#include <utility>

namespace Switcher_details
{

template<typename LHS, typename RHS>
struct value_equal_impl
{
	inline bool operator()( const LHS &lhs, const RHS &rhs ) { return lhs == rhs; }
};

// specialisation for matches marked as 'ignore'
template<typename RHS>
struct value_equal_impl<std::decay<decltype(std::ignore)>::type,RHS>
{
	inline bool operator()( const std::decay<decltype(std::ignore)>::type &lhs, const RHS &rhs ) { return true; }
};

// helper function
template<typename LHS, typename RHS>
inline bool value_equal( const LHS &lhs, const RHS &rhs )
{
	return value_equal_impl<LHS,RHS>()( lhs, rhs );
}

template <size_t I>
struct tuple_equal
{
	template <typename LHS, typename RHS>
	inline bool operator()( const LHS &lhs, const RHS &rhs )
	{
		return tuple_equal<I-1>()( lhs, rhs) and value_equal( std::get<I-1>(lhs), std::get<I-1>(rhs) );
	}
};

template <>
struct tuple_equal<0>
{
	template <typename LHS, typename RHS>
	inline bool operator()( const LHS &lhs, const RHS &rhs ) { return true; }
};

}

template<typename ...VALUES>
class Switcher
{
  private:
	std::tuple<VALUES...> values;
	bool matched = false;

	template <class T, std::size_t... I>
	constexpr auto extract_matches( T &&t, std::index_sequence<I...>)
	{
		return std::forward_as_tuple( std::get<I>( std::forward<T>(t))... );
	}

  public:
	Switcher( VALUES ...v ) : values( v... ){}
	
	template<typename ...MATCHES_AND_FUNC>
	Switcher &Case( MATCHES_AND_FUNC ...t )
	{
		static_assert( (sizeof...(MATCHES_AND_FUNC)-1) == sizeof...(VALUES), "" );
		if ( not matched )
		{
			auto matches_and_func = std::make_tuple( t... );
			auto matches = extract_matches(
								std::forward<std::tuple<MATCHES_AND_FUNC...>>( matches_and_func ),
								std::make_index_sequence<sizeof...(VALUES)>{} );
			if ( Switcher_details::tuple_equal<sizeof...(VALUES)>()( matches, values ) )
			{
				// marked as done and call the function
				matched = true;
				std::get<sizeof...(VALUES)>( matches_and_func )();
			}
		}
		return *this;
	}
	template<typename FUNC>
	void Default( const FUNC &i_func )
	{
		if ( not matched )
			i_func();
	}
};

template<typename ...T>
Switcher<T...> Switch( T ...t )
{
	return Switcher<T...>( t... );
}

#endif

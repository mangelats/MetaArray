//
// Main (and only) project file
//


#include <cstddef> // std::size_t

// I prefere the specialization first because it does all the work
// This is required to do so.
template <typename T, T ...> struct MetaArray;



//
// Template specialization for non-empty array
// It makes all the actual job
//

template <typename T, T first, T ... array>
struct MetaArray <T, first, array...> {
	// Self type (using diferent names because it's more readable in different situations)
	using This = MetaArray<T, first, array...>;
	using Itself = This;

	// All the transformations are self-explanatory

	template <T n> using PushFront = MetaArray<T, n, first, array...>;
	template <T n> using PushBack = MetaArray<T, first, array..., n>;

	using RemoveFront = MetaArray<T, array...>;
	using Reverse = typename RemoveFront::Reverse::template PushBack<first>;	// Reverse uses RemoveFront
	using RemoveBack = typename Reverse::RemoveFront::Reverse;					// RemoveBack uses Reverse


	// Convertion to regular array and its size.
	static constexpr std::size_t size = sizeof...(array)+1;
	static constexpr T values[size] = {first, array...};


	// Forwards this template parameters to another class.
	//Usefull for modifying the array and ten use it in another class.
	//It may require a second step if adding more parameters is required.
	template<template <typename UT, UT ...> typename U> using Forward = U<T, array...>;
};



//
// Base struct
// It is used when the variadic array is empty
// So we have an empty array
//

template <typename T, T ...>
struct MetaArray {
	// Self type (using diferent names because it's more readable in different situations)
	using This = MetaArray<T>;
	using Itself = This;

	// Any Push makes an array with 1 element
	template <T n> using PushFront = MetaArray<T, n>;
	template <T n> using PushBack = MetaArray<T, n>;

	// There is nothing to be removed. So we keep the same array
	using RemoveFront = This;
	using RemoveBack = This;

	// The reverse of an empty array is an empty array
	using Reverse = This;

	// Empty array always have size 0
	static constexpr std::size_t size = 0;
	static constexpr T * values = nullptr; // Makes some algorithm work that wouldn't otherwise.


	// Forwards this template parameters to another class.
	//Usefull for modifying the array and ten use it in another class.
	//It may require a second step if adding more parameters is required.
	template<template <typename UT, UT ...> typename U> using Forward = U<T>;
};

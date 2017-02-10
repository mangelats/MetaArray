# MetaArray
This project is a small C++17 helper class that allows to create and modify a variadic template parameter easily.

I always understand everythong better if it's an example, so here it goes (this code does **NOT** compile, they are separate code parts)
## Installation
Download the file `MetaArray.cxx` to your project and import it as a header.

## Define the array
```cpp
// Our array (is usefull to make an alias if you are going to use it a lot [like here])
// All other examples use this class
using MyArray = MetaArray<int, 0, 1, 2, 3, 4, 5, 6>;
```

## Values
All this values are defines as `static constexpr`, so they can be used as template arguments.
```cpp
// How many values we have
MyArray::size      // 7
// values is the array of the values (for an empty array, values is nullptr)
MyArray::values[2] // 2
```

## Transformations
```cpp
typename MyArray::template PushFront<24> // MetaArray<int, 24, 0, 1, 2, 3, 4, 5, 6>
typename MyArray::template PushBack<24>  // MetaArray<int, 0, 1, 2, 3, 4, 5, 6, 24>
typename MyArray::RemoveFront            // MetaArray<int, 1, 2, 3, 4, 5, 6>
typename MyArray::RemoveBack             // MetaArray<int, 0, 1, 2, 3, 4, 5>
typename MyArray::Reverse                // MetaArray<int, 6, 5, 4, 3, 2, 1, 0>
```

## Forward the result to another class
```cpp
template <typename T, T ... array> // It requires this template parameters
struct Example {
  // [...]
};

typename MyArray::RemoveFront::Reverse::Forward<Example> // Example<int, 6, 5, 4, 3, 2, 1>
```
Some times you want to have a different template parameters in the class. Sometimes, it can easily be solved by using an alias:
```cpp
template<bool enabled, int ... values>
struct Example2 {
  // [...]
};

template <typename T, T ... array>
using Example2_From_Array = Example2<true, array...>; // Error if T is not int

typename MyArray::Reverse::template Forward<Example2_From_Array> // Example2<true, 6, 5, 4, 3, 2, 1, 0>
```
For more even more complex cases, you can use a helper struct:
```cpp
#include <type_traits> // std::is_same_v

template <typename T, T ... array>
struct Example2_Builder {
  // Make the error readable
  static_assert(std::is_same_v<T, int>, "Wrong array type.");
  
  template <bool enabled>
  using Result = Example2<enabled, array...>;
};

typename MyArray::template Forward<Example2_Builder>::template Result<true> // Example2<true, 0, 1, 2, 3, 4, 5, 6>
typename MyArray::template Forward<Example2_Builder>::template Result<false> // Example2<false, 0, 1, 2, 3, 4, 5, 6>
```

#include <memory>
#include <type_traits>

template<typename T,typename Alloc = std::allocator<T>>
class Vector{
	/*
	 Vector class is clone of std::vector
	 to learn std::vector architecture
	 */
	static_assert(std::is_same<T,typename Alloc::value_type>::value,"the allocator value type is not matched the vecotr value type.");
	static_assert(!std::is_const<T>::value,"this library forbids containers of const elements");
private:
	using traits = std::allocator_traits<Alloc>;
public:
	using value_type = T;
	using allocator_type = Alloc;
	using size_type = unsigned int;
	using difference_type = int;
	using reference = T&;
	using const_reference = const T&;
	using pointer = typename traits::pointer;
	using const_pointer = typename traits::const_pointer;
private:
	pointer e; // pointer of array (pointer of first element) 
	size_type length = 0,cap = 1;
	Alloc alloc;
};

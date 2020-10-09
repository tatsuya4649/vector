#include <vector/vector.h>

template <typename T,typename Alloc>
void Vector<T,Alloc>::extension(){
	pointer e_ = alloc.allocate(cap * 2);
	for (int i=0;i<length;++i){
		traits::construct(alloc,e_+i,*(e+i));
		traits::destroy(alloc,e+i);
	}
	alloc.deallocate(e,cap);
	e = e_;
	cap *= 2;
}

template <typename T,typename Alloc>
void Vector<T,Alloc>::extension(size_type n){
	unsigned int r = 1;
	while(cap * r < n){
		r *= 2;
	}
	if (r == 1) return;
	pointer e_ = alloc.allocate(cap * r);
	for (int i=0;i<length;++i){
		traits::construct(alloc,e_+i,*(e+i));
		traits::destroy(alloc,e+i);
	}
	alloc.deallocate(e,cap);
	e = e_;
	cap *= r;
}

template <typename T,typename Alloc>
template <typename... Args>
void Vector<T,Alloc>::emplace_back(Args&&... args){
	if (length == cap) extension();
	traits::construct(alloc, e + length,std::forward<Args>(args)...);
	length++;
}

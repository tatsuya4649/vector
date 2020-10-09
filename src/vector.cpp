#include <vector/vector.h>

template <typename T,typename Alloc>
Vector<T,Alloc>::Vector(const Alloc& a) noexcept : alloc{a}{
	e = alloc.allocate(cap);
}

template <typename T,typename Alloc>
Vector<T,Alloc>::Vector(size_type n,const Alloc& a) : alloc{a}{
	while (cap < n){
		cap *= 2;
	}
	e = alloc.allocate(cap);
	for(int i=0;i<n;++i){
		emplace_back();
	}
}

template <typename T,typename Alloc>
Vector<T,Alloc>::Vector(size_type n,const_reference value,const Alloc& a) : alloc{a}{
	while (cap < n){
		cap *= 2;
	}
	e = alloc.allocate(cap);
	for (int i=0;i<n;++i){
		emplace_back(value);
	}
}

template <typename T,typename Alloc>
template <typename InputIter>
Vector<T,Alloc>::Vector(InputIter first,InputIter last,const Alloc& a) : alloc{a}{
	e = alloc.allocate(cap);
	for (InputIter i = first; i != last; i++){
	       emplace_back(*i);
	}
}

template <typename T,typename Alloc>
Vector<T,Alloc>::Vector(const Vector& x,const Alloc& a) : alloc{a}{
	while( cap < x.length ){
		cap *= 2;
	}
	length = x.length;
	e = alloc.allocate(cap);
	for (int i=0;i<x.length;++i){
		traits::construct(alloc,e+i,*(x.e + i));
	}
}

template <typename T,typename Alloc>
Vector<T,Alloc>::Vector(Vector&& x,const Alloc& a) noexcept : alloc{a}{
	cap = x.cap;
	length = x.length;
	e = x.e;
	x.e = nullptr;
}

template <typename T,typename Alloc>
Vector<T,Alloc>::~Vector(){
	if (e != nullptr){
		for (int i=0;i<length;++i){
			traits::destroy(alloc,e+i);
		}
		alloc.deallocate(e,cap);
	}
}

template <typename T,typename Alloc>
Vector<T,Alloc>& Vector<T,Alloc>::operator=(const Vector& x){
	for (int i=0;i<length;++i){
		traits::destroy(alloc,e+i);
	}
	alloc.deallocate(e,cap);
	length = x.length;
	cap = 1;
	while(cap < length){
		cap *= 2;
	}
	e = alloc.allocate(cap);
	for (int i=0;i<length;++i){
		traits::construct(alloc,e+i,*(x.e + i));
	}
	return *this;
}

template <typename T,typename Alloc>
Vector<T,Alloc>& Vector<T,Alloc>::operator=(Vector&& x){
	for (int i=0;i<length;++i){
		traits::destroy(alloc,e+i);
	}
	alloc.deallocate(e,cap);
	length = x.length;
	cap = x.cap;
	length = x.length;
	e = x.e;
	x.e = nullptr;
	return *this;
}







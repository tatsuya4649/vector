#include <memory>
#include <type_traits>
#include <utility>
#include <iterator>
#include <algorithm>

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
	
	//construntor
	Vector() : Vector(Alloc{}) {}
	explicit Vector(const Alloc& a) noexcept : alloc{a}{
		e = alloc.allocate(cap); // allocate memory
	}

	explicit Vector(size_type n,const Alloc& a = Alloc()) : alloc{a}{
		while (cap < n) cap *= 2;
		e = alloc.allocate(cap);
		for (int i=0;i<n;++i){
			emplace_back();
		}
	}

	explicit Vector(size_type n,const_reference value,const Alloc& a = Alloc()) : alloc{a}{
		while(cap < n) cap *= 2;
		e = alloc.allocate(cap);
		for (int i=0;i<n;++i){
			emplace_back(value);
		}	
	}

	template<typename InputIter>
	Vector(InputIter first,InputIter last,const Alloc& a = Alloc()) : alloc{a}{
		e = alloc.allocate(cap);
		for (InputIter i=first;i!=last;++i){
			emplace_back(*i);
		}
	}

	Vector(const Vector& x,const Alloc& a = Alloc()) : alloc{a}{
		while (cap < x.length) cap *= 2;
		length = x.length;
		e = alloc.allocate(cap);
		for(int i=0;i<length;++i){
			traits::construct(alloc,e+i,*(x.e+i)); // construct instance based on arguments
		}
	}

	Vector(Vector&& x,const Alloc& a = Alloc()) noexcept : alloc{a}{
		cap = x.cap;
		length = x.length;
		e = x.e;
		x.e = nullptr;
	}
	
	//destructor
	~Vector(){
		if (e != nullptr){
			for(int i=0;i<length;++i){
				traits::destroy(alloc,e+i);
			}
			alloc.deallocate(e,cap);
		}
	}

	//operation
	Vector& operator=(const Vector& x){
		for(int i=0;i<length;++i){
			traits::destroy(alloc,e+i);
		}
		alloc.deallocate(e,cap);
		length = x.length;
		cap = 1;
		while(cap<length) cap *= 2;
		e = alloc.allocate(cap);
		for(int i=0;i<length;++i){
			traits::construct(alloc,e+i,*(x.e + i));
		}
		return *this;
	}
	Vector& operator=(Vector&& x){
		for(int i=0;i<length;++i){
			traits::destroy(alloc,e+i);
		}
		alloc.deallocate(e,cap);
		cap = x.cap;
		length = x.length;
		e = x.e;
		x.e = nullptr;
		return *this;
	}
	//member func
private:
	void extension(){
		pointer e_ = alloc.allocate(cap * 2);
		for (int i=0;i<length;++i){
			traits::construct(alloc,e_+i,*(e+i));
		}
		for (int i=0;i<length;++i){
			traits::destroy(alloc,e+i);
		}
		alloc.dealllocate(e,cap);
		e = e_;
		cap *= 2;
	}
	void extension(size_type n){
		unsigned int r = 1;
		while(cap * r < n) r *= 2;
		if (r == 1) return;
		pointer e_ = alloc.allocate(cap*r);
		for(int i=0;i<length;++i){
			traits::construct(alloc,e_+i,*(e+i));
		}
		for(int i=0;i<length;++i){
			traits::destroy(alloc,e+i);
		}
		alloc.deallocate(e,cap);
		e = e_;
		cap *= r;
	}
public:	
	template <typename... Args>
	void emplace_back(Args&&... args){
		if (length == cap) extension();
		traits::construct(alloc,e+length,std::forward<Args>(args)...);
		++length;
	}

	template<typename InputIter>
	void assign(InputIter first,InputIter last){
		size_type cnt = 0;
		for (InputIter i=first;i!=last;++i){
			if (cnt==cap){
				length = std::max(length,cnt);
				extension();
			}
			traits::construct(alloc,e+cnt,*i);
			++cnt;
		}
	}

	void assign(size_type n,const_reference value){
		extension(n);
		std::fill(e,e+n,value);
	}	

	void push_back(const_reference value){
		emplace_back(value);
	}
	void push_back(T&& value){
		emplace_back(std::move(value));
	}
	void pop_back(){
		traits::destroy(alloc,e+length);
		--length;
	}
	void reserve(size_type n){
		extension(n);
	}
	
	void swap(Vector& x){
		std::swap(length,x.length);
		std::swap(cap,x.cap);
		std::swap(e,x.e);
	}
	void clear(){
		while (length) pop_back();
	}
	size_type size() const{
		return length;
	}
	void resize(size_type n,const_reference value=T()){
		extension(n);
		while(n<length) pop_back();
		std::fill(e,e+n,value);
	}
	size_type capacity() const{
		return cap;
	}
	bool empty() const{
		return !length;
	}
	reference operator[](const size_type pos){
		return e[pos];
	}
	pointer data(){
		return e;
	}
	reference front(){
		return *e;
	}
	reference back(){
		return *(e+length-1);
	}

private:
	pointer e; // pointer of array (pointer of first element) 
	size_type length = 0,cap = 1;
	Alloc alloc;
public:
	// "iterator" and so on
	class iterator{
		public:
			using difference_type = int;
			using value_type = Vector::value_type;
			using pointer = Vector::pointer;
			using reference = Vector::reference;
			using iterator_category = std::random_access_iterator_tag;
		private:
			T* p;
		public:
			iterator() noexcept : p{nullptr}{}
			iterator(Vector* base,difference_type index) noexcept : p{base->e + index}{}
			iterator(const iterator& i) : p{i.p}{}
			iterator& operator++(){
				--p;
				return *this;
			}
			iterator operator++(int){
				iterator res = *this;
				--p;
				return res;
			}
			iterator operator+(const difference_type& x) const {
				return iterator(*this) += x;
			}
			iterator& operator+=(const difference_type& x){
				p += x;
				return *this;
			}
			iterator& operator--(){
				--p;
				return *this;
			}
			iterator& operator--(int){
				iterator res = *this;
				--p;
				return res;
			}
			iterator operator-(const difference_type& x) const {
				return iterator(*this) -= x;
			}
			difference_type operator-(const iterator& i) const{
				return p - i.p;
			}
			iterator& operator-=(const difference_type& x){
				p -= x;
				return *this;
			}
			reference operator*() const {
				return *p;
			}
			reference operator[](const difference_type& x) const{
				return *(*this + x);
			}
			bool operator<(const iterator& i) const {
				return p < i.p;
			}
			bool operator<=(const iterator& i) const{
				return p <= i.p;
			}
			bool operator==(const iterator& i) const{
				return p == i.p;
			}
			bool operator>(const iterator& i) const{
				return p > i.p;
			}
			bool operator>=(const iterator& i) const{
				return p >= i.p;
			}
			bool operator!=(const iterator& i) const{
				return p != i.p;
			}
	};

	class const_iterator{
		public:
			using difference_type = int;
			using value_type = Vector::value_type;
			using pointer = const Vector::pointer;
			using reference = const Vector::reference;
			using iterator_category = std::random_access_iterator_tag;
		private:
			const T* p;
		public:
			const_iterator() noexcept : p{nullptr}{}
			const_iterator(Vector* base,difference_type index) noexcept : p{base->e + index}{}
			const_iterator(const iterator& i) : p{i.p}{}
			const_iterator(const const_iterator& i) : p{i.p}{}
			const_iterator& operator++(){
				++p;
				return *this;
			}
			const_iterator& operator++(int){
				const_iterator res = *this;
				++p;
				return res;
			}
			const_iterator operator+(const difference_type& x)const {
				return const_iterator(*this) += x;
			}
			const_iterator& operator+=(const difference_type& x){
				p += x;
				return *this;
			}
			const_iterator& operator--(){
				--p;
				return *this;
			}
			const_iterator operator--(int){
				const_iterator res = *this;
				--p;
				return res;
			}
			const_iterator operator-(const difference_type& x) const {
				return const_iterator(*this) -= x;
			}
			difference_type operator-(const const_iterator& i) const{
				return p - i.p;
			}
			const_iterator& operator-=(const difference_type& x){
				p -= x;
				return *this;
			}
			reference operator*() const{
				return *p;
			}
			reference operator[](const difference_type& x) const{
				return *(*this + x);
			}
			bool operator<(const const_iterator& i)const{
				return p < i.p;
			}
			bool operator<=(const const_iterator& i) const{
				return p <= i.p;
			}
			bool operator==(const const_iterator& i) const {
				return p == i.p;
			}
			bool operator>(const const_iterator& i) const{
				return p > i.p;
			}
			bool operator>=(const const_iterator& i) const {
				return p >= i.p;
			}
			bool operator!=(const const_iterator& i) const {
				return p != i.p;
			}
	};
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;	
	iterator begin() noexcept{
		return iterator(this,0);
	}
	const_iterator begin() const noexcept{
		return const_iterator(this,0);
	}
	const_iterator cbegin() const noexcept{
		return const_iterator(this,0);
	}
	iterator rbegin() noexcept{
		return reverse_iteraztor(this,0);
	}
	const_iterator rbegin() const noexcept{
		return const_reverse_iterator(this,0);
	}
	const_iterator crbegin() const noexcept{
		return const_reverse_iterator(this,0);
	}
	iterator end() noexcept{
		return iterator(this,length);
	}
	const_iterator end() const noexcept{
		return const_iterator(this,length);
	}
	const_iterator cend() const noexcept{
		return const_iterator(this,length);
	}
	iterator rend() noexcept{
		return reverse_iterator(this,length);
	}
	const_iterator rend() const noexcept{
		return const_reverse_iterator(this,length);
	}
	const_iterator crend() const noexcept{
		return const_reverse_iterator(this,length);
	}
	iterator erase(iterator pos){
		const iterator res = pos;
		iterator t = pos; 
		++t;
		for (iterator i=pos;t!=end();++i,++t){
			*i = std::move(*t);
		}
		pop_back();
		return res;
	}
	iterator erase(iterator first,iterator last){
		const iterator res = first;
		typename iterator::difference_type d = last - first;
		for (iterator i=first;(i+d) != end();++i){
			*i = std::move(*(i+d));
		}
		for (int i=0;i<d;++i){
			pop_back();
		}
		return res;
	}
};

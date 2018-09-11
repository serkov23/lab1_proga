#ifndef UNTITLED_KDEQUE_H
#define UNTITLED_KDEQUE_H

#include <exception>
#include <stdexcept>
#include <algorithm>
#include <iostream>

template<class T>
class KIterator;

template<class T, size_t grow_const = 2>
class KDeque {
public:

	friend class KIterator<T>;

	typedef KIterator<T> iterator;
protected:
	T * beg;
	size_t _size{}, _capacity{};

	bool need_to_grow() const;

	void grow();

	bool need_to_lower() const;

	void lower();

public:

	KDeque();

	explicit KDeque(size_t _size);

	KDeque(size_t _size, T val);

	KDeque(const KDeque &base);

	KDeque(KDeque &&base) noexcept;

	KDeque(std::initializer_list<T> val);

	T &operator[](size_t i) const;

	KDeque &operator=(const KDeque &base);

	KDeque &operator=(KDeque &&base) noexcept;


	iterator begin() const;

	iterator end() const;

	const size_t size() const;

	bool empty() const;

	void clear();

	size_t capacity() const;

	T &front() const;

	T &back() const;

	void push_back(const T &val);

	template <class ...Types>
	void emplace_back(Types&& ...values);

	template <class ...Types>
	void emplace_front(Types&& ...values);

	void pop_back();

	void push_front(const T &val);

	void pop_front();

	template<class T1, size_t grow_const1, size_t grow_const2>
	friend void swap(KDeque<T1, grow_const1> &op1, KDeque<T1, grow_const2> &op2);

	bool operator==(const KDeque &op) const;

	bool operator!=(const KDeque &op) const;

	KDeque operator+(const KDeque &op) const;


	KDeque &operator+=(const KDeque &op);

	~KDeque();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template<class T>
class KIterator {
	T *it;
protected:
	void check_pointer() const;

public:
	KIterator();

	explicit KIterator(T *it);

	const KIterator &operator++();

	const KIterator operator++(int);

	T &operator*() const;

	T *operator->();

	const KIterator &operator--();

	const KIterator operator--(int);

	bool operator==(const KIterator &op) const;

	bool operator!=(const KIterator &op) const;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



template<class T>
void KIterator<T>::check_pointer() const {
	if (!it)
		throw std::runtime_error("can't dereference pointer to NULL");
}

template<class T>
KIterator<T>::KIterator() : it(nullptr) {}

template<class T>
KIterator<T>::KIterator(T *it) : it(it) {}

template<class T>
const KIterator<T> &KIterator<T>::operator++() {
	check_pointer();
	it++;
	return *this;
}

template<class T>
const KIterator<T> KIterator<T>::operator++(int) {
	check_pointer();
	auto old_it = *this;
	it++;
	return old_it;
}

template<class T>
T &KIterator<T>::operator*() const {
	check_pointer();
	return *it;
}

template<class T>
T *KIterator<T>::operator->() {
	check_pointer();
	return it;
}

template<class T>
const KIterator<T> &KIterator<T>::operator--() {
	check_pointer();
	it--;
	return *this;
}

template<class T>
const KIterator<T> KIterator<T>::operator--(int) {
	check_pointer();
	auto old_it = *this;
	it--;
	return old_it;
}

template<class T>
bool KIterator<T>::operator==(const KIterator &op) const {
	return it == op.it;
}

template<class T>
bool KIterator<T>::operator!=(const KIterator &op) const {
	return it != op.it;
}

template<class T, size_t grow_const>
KDeque<T, grow_const>::KDeque() : beg(nullptr), _size(0), _capacity(0) {}

template<class T, size_t grow_const>
KDeque<T, grow_const>::KDeque(const size_t _size) : _size(_size), beg(new T[_size]), _capacity(_size) {}

template<class T, size_t grow_const>
KDeque<T, grow_const>::KDeque(const size_t _size, const T val) : KDeque(_size) {
	for (auto &i : (*this)) {
		i = val;
	}
}

template<class T, size_t grow_const>
KDeque<T, grow_const>::KDeque(const KDeque &base) : KDeque(base.size()) {
	for (size_t i = 0; i < _size; i++)
		beg[i] = base[i];
}

template<class T, size_t grow_const>
KDeque<T, grow_const>::KDeque(KDeque &&base) noexcept : beg(base.beg), _size(base.size()),
_capacity(base.capacity()) {
	base.beg = nullptr;
	base._size = 0;
	base._capacity = 0;
}

template<class T, size_t grow_const>
T &KDeque<T, grow_const>::operator[](const size_t i) const {
	if (i >= _size)
		throw std::logic_error("invalid index");
	return beg[i];
}

template<class T, size_t grow_const>
const size_t KDeque<T, grow_const>::size() const {
	return _size;
}

template<class T, size_t grow_const>
bool KDeque<T, grow_const>::empty() const {
	return size() == 0;
}

template<class T, size_t grow_const>
void KDeque<T, grow_const>::clear() {
	if (!empty()) {
		_size = 0;
		_capacity = 0;
		delete[] beg;
		beg = nullptr;
	}
}

template<class T, size_t grow_const>
size_t KDeque<T, grow_const>::capacity() const {
	return _capacity;
}

template<class T, size_t grow_const>
KDeque<T, grow_const>::~KDeque() {
	if (capacity() > 0)
		delete[] beg;
}

template<class T, size_t grow_const>
KDeque<T, grow_const> &KDeque<T, grow_const>::operator=(const KDeque &base) {
	clear();
	_size = base.size();
	_capacity = base.capacity();
	if (capacity() > 0) {
		beg = new T[capacity()];
		for (int i = 0; i < size(); i++)
			beg[i] = base[i];
	}
	return *this;
}

template<class T, size_t grow_const>
KDeque<T, grow_const>::KDeque(const std::initializer_list<T> val) : KDeque(val.size()) {
	iterator it(beg);
	for (auto i : val) {
		*it = i;
		it++;
	}
}

template<class T, size_t grow_const>
KDeque<T, grow_const> &KDeque<T, grow_const>::operator=(KDeque &&base) noexcept {
	if (!empty())
		delete[]beg;

	beg = base.beg;
	_size = base.size();
	_capacity = base.capacity();
	base.beg = nullptr;
	base._size = 0;
	base._capacity = 0;
	return *this;
}

template<class T, size_t grow_const>
KIterator<T> KDeque<T, grow_const>::begin() const {
	return iterator(beg);
}

template<class T, size_t grow_const>
bool KDeque<T, grow_const>::need_to_grow() const {
	return capacity() == size();
}

template<class T, size_t grow_const>
void KDeque<T, grow_const>::grow() {
	if (!need_to_grow())
		return;
	if (capacity() == 0)
		_capacity = 1;
	else
		_capacity *= grow_const;
	auto old_arr = beg;
	beg = new T[capacity()];
	for (int i = 0; i < size(); i++)
		beg[i] = old_arr[i];
	delete[]old_arr;
}

template<class T, size_t grow_const>
bool KDeque<T, grow_const>::need_to_lower() const {
	return empty() || (capacity() / size() >= grow_const);
}

template<class T, size_t grow_const>
void KDeque<T, grow_const>::lower() {
	if (!need_to_lower())
		return;
	_capacity /= grow_const;
	auto old_arr = beg;
	beg = new T[capacity()];
	for (int i = 0; i < size(); i++) {
		beg[i] = old_arr[i];
	}
	delete[] old_arr;
}

template<class T, size_t grow_const>
KIterator<T> KDeque<T, grow_const>::end() const {
	return iterator(beg + size());
}

template<class T, size_t grow_const>
T &KDeque<T, grow_const>::front() const {
	return *beg;
}

template<class T, size_t grow_const>
T &KDeque<T, grow_const>::back() const {
	return *(--end());
}

template<class T, size_t grow_const>
void KDeque<T, grow_const>::push_back(const T &val) {
	grow();
	*end() = val;
	_size++;
}

template<class T, size_t grow_const>
void KDeque<T, grow_const>::pop_back() {
	if (empty())
		throw std::overflow_error("trying to delete from empty deque");
	_size--;
	lower();
}

template<class T, size_t grow_const>
void KDeque<T, grow_const>::push_front(const T &val) {
	push_back(val);
	for (int i = size() - 1; i > 0; i--) {
		std::swap(beg[i], beg[i - 1]);
	}
}

template<class T, size_t grow_const>
void KDeque<T, grow_const>::pop_front() {
	for (int i = 1; i < size(); i++) {
		std::swap(beg[i], beg[i - 1]);
	}
	pop_back();
}

template<class T, size_t grow_const1, size_t grow_const2>
void swap(KDeque<T, grow_const1> &op1, KDeque<T, grow_const2> &op2) {
	std::swap(op1.beg, op2.beg);
	std::swap(op1._capacity, op2._capacity);
	std::swap(op1._size, op2._size);
}

template<class T, size_t grow_const>
bool KDeque<T, grow_const>::operator==(const KDeque &op) const {
	if (size() != op.size())
		return false;
	for (int i = 0; i < size(); i++) {
		if (beg[i] != op[i])
			return false;
	}
	return true;
}

template<class T, size_t grow_const>
bool KDeque<T, grow_const>::operator!=(const KDeque &op) const {
	return !(operator==(op));
}

template<class T, size_t grow_const>
KDeque<T, grow_const> KDeque<T, grow_const>::operator+(const KDeque &op) const {
	auto buf = *this;
	buf += op;
	return buf;
}

template<class T, size_t grow_const>
KDeque<T, grow_const> &KDeque<T, grow_const>::operator+=(const KDeque &op) {
	for (auto i : op) {
		push_back(i);
	}
	return *this;
}

template<class T, size_t grow_const>
template<class... Types>
void KDeque<T, grow_const>::emplace_front(Types &&... values) {
	emplace_back(values...);
	for (int i = size() - 1; i > 0; i--) {
		std::swap(beg[i], beg[i - 1]);
	}
}

template<class T, size_t grow_const>
template<class... Types>
void KDeque<T, grow_const>::emplace_back(Types &&... values) {
	grow();
	*end() = T(std::forward<Types>(values)...);
	_size++;
}


template<class T, size_t grow_const>
std::istream& operator>>(std::istream& in, KDeque<T, grow_const>& deq) {
	size_t n;
	in >> n;
	for (int i = 0; i < n; i++) {
		T buf;
		in >> buf;
		deq.emplace_back(buf);
	}
	return in;
}

template<class T, size_t grow_const>
std::ostream& operator<<(std::ostream& out, KDeque<T, grow_const>& deq) {
	for (auto i : deq) {
		out << i << ' ';
	}
	out << '\n';
	return out;
}


#endif //UNTITLED_KDEQUE_H

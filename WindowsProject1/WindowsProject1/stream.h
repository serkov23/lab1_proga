#pragma once
#define UNICODE
#include<Windows.h>
#include<windowsx.h>
#include<sstream>
#include<string>
#include"KDeque.h"
class KStream {
protected:
	HWND hwnd;
	const size_t maxn = 1000;
public:
	explicit KStream(HWND hwnd);;

};

class KOstream :public KStream
{
public:
	explicit KOstream(HWND hwnd);

	void clear();

	template<typename T>
	KOstream& operator<<(const T& val);
};

class KIstream :public KStream
{
public:
	explicit KIstream(HWND hwnd);;

	template<class T>
	KIstream& operator>>(T& val);
};

template<typename T>
KOstream &KOstream::operator<<(const T &val) {
	std::wstringstream ss;
	ss << val;
	auto str = new wchar_t[maxn];
	ss.get(str, maxn);
	SendMessage(hwnd, LB_ADDSTRING, 0, (LPARAM)str);
	delete[] str;
	return *this;
}


template<class T>
KIstream &KIstream::operator>>(T &val) {
	auto str = new wchar_t[maxn];
	Edit_GetText(hwnd, str, maxn);
	std::wstringstream ss(str);

	ss >> val;

	delete[]str;
	if (!ss)
		throw std::runtime_error("invalid input");
	return *this;
}



template<class T, size_t grow_const>
KOstream& operator<< (KOstream& out, const KDeque<T, grow_const>& deq) {
	out.clear();
	for (auto i : deq) {
		out << i;
	}
	out << (std::wstring(L"size: ")+std::to_wstring(deq.size()) + L"       capacity: " + std::to_wstring(deq.capacity()));
	return out;
}
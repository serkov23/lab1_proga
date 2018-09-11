#include "stdafx.h"
#include "stream.h"

KStream::KStream(HWND hwnd) :hwnd(hwnd) {}

void KOstream::clear() {
	SendMessage(hwnd, LB_RESETCONTENT, 0, 0);
}

KOstream::KOstream(HWND hwnd) :KStream(hwnd) {}


KIstream::KIstream(HWND hwnd) :KStream(hwnd) {}


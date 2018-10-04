#pragma once
#include "stdafx.h"
class contact
{
private:
	sockaddr_in youAddr;
	int youAddrSize;
public:
	wchar_t name[99];
	contact(char ipget[18], int lisport);
	contact();
	void setip(char ipget[18]);
	void setport(int lisport);
	void setname(wchar_t setme[]);
	sockaddr_in getaddr() {
		return youAddr;
	}
	~contact();
};


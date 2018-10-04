#include "stdafx.h"
//#include "contact.h"


contact::contact(char ipget[18], int lisport)
{
	memset(youAddr.sin_zero, 0, sizeof(youAddr.sin_zero));
	youAddr.sin_family = AF_INET;
	youAddr.sin_port = htons(lisport);
	inet_pton(AF_INET, ipget, &youAddr.sin_addr);
	int youAddrSize = sizeof(youAddr);
}
contact::contact()
{
	memset(youAddr.sin_zero, 0, sizeof(youAddr.sin_zero));
	youAddr.sin_family = AF_INET;
	youAddr.sin_port = htons(0000);
	inet_pton(AF_INET,"1.1.1.1", &youAddr.sin_addr);
	int youAddrSize = sizeof(youAddr);
}
void contact::setip(char ipget[18]) {
	inet_pton(AF_INET, ipget, &youAddr.sin_addr);
}
void contact::setport(int lisport) {
		youAddr.sin_port = htons(lisport);
}
void contact::setname(wchar_t setme[]) {
	for(int o=0;o<99;o++)
	name[o] = setme[o];
}
contact::~contact()
{
}

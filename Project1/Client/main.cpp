#include <iostream>
#include <WinSock.h>
#include <Windows.h>
#include <stdlib.h>
#include "define.h"
#include "DES.h"

#pragma comment( lib, "Wsock32.lib" )

using namespace std;

char key[9] = {'W', 'H', 'I', 'T', 'E', 'H', 'A', 'T', '\0'};
char strSocketBuffer[256];
char strStdinBuffer[256];
char strIpAddr[16];
SOCKET nConnectSocket = INVALID_SOCKET;

void SecretChat(int nSock, char *pRemoteName, char *pKey);
size_t TotalRecv(int s, void *buf, size_t len, int flags);
DWORD WINAPI MyThread(LPVOID lpParameter);

int main()
{
	int nConnectLength;
	struct sockaddr_in sDestAddr;
	WSADATA sentWsa;
	CDesOperate des;

	
	char clear[13] = {'B', 'L', 'A', 'C', 'K', 'H', 'A', 'T', 'h', 'e', 'l', 'l', 'o'};
	char cipher[129] = "";
	char decode_text[17] = "";
	
	cout << "Please input the server IP address:" << endl;
	cin >> strIpAddr;

	if(WSAStartup(WSA_VERSION, &sentWsa) != 0)
	{
		cout << "Socket Startup Failure" << endl;
		return 1;
	}

	if((nConnectSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cout << "Socket Error" << endl;
		exit(0);
	}

	sDestAddr.sin_family = AF_INET;
	sDestAddr.sin_port = htons(1010); //Server Port No.
	sDestAddr.sin_addr.s_addr = inet_addr(strIpAddr);

	if(connect(nConnectSocket, (struct sockaddr*) &sDestAddr, sizeof(sDestAddr)) != 0)
	{
		cout << "Connect Error" << endl;
		exit(0);
	}
	else
	{
		cout << "Connect Success!" << endl;
		cout << "Begin to chat ..." << endl;
		SecretChat(nConnectSocket, inet_ntoa(sDestAddr.sin_addr), key);
	}
	closesocket(nConnectSocket);
	
}

void SecretChat(int nSock, char *pRemoteName, char *pKey)
{
	CDesOperate cDes;  
	char *cipher;

	if(strlen(pKey)!=8)  
	{   
		printf("Key length error");   
		return ;  
	}   

	HANDLE handle1;
	handle1 = CreateThread(NULL,0,MyThread,NULL,0,NULL);

	if(handle1 == NULL)
	{
		cout << "Create Thread Fail" << endl;
		exit(0);
	}

	while(1)
	{
		memset(&strStdinBuffer, 0, 256);
		while(strStdinBuffer[0] == 0)
			cin.getline(strStdinBuffer, 256);
		int nLen = 256;

		int length = ((strlen(strStdinBuffer)/9)+1)*8*8 + 1;
		cipher = new char[length];
		cDes.Encry(strStdinBuffer, strlen(strStdinBuffer), cipher, length, pKey, strlen(pKey));

		if(send(nConnectSocket, cipher, strlen(cipher), 0) != strlen(cipher))
			cout << "Send Error" << endl;
		else if(memcmp("quit", strSocketBuffer, 4) == 0)
		{
			cout << "Quit!" << endl;
			break;
		}
	}
}

DWORD WINAPI MyThread(LPVOID lpParameter)
{
	CDesOperate cDes;
	char plainText[9] = "";
	while(1)
	{
		memset(&strSocketBuffer, 0, 256);
		int nLength = 0;
		nLength = TotalRecv(nConnectSocket, strSocketBuffer, 64, 0);

		if(nLength == -1)
			continue;

		//TODO:DECODE
		cDes.Decry(strSocketBuffer, nLength, plainText, 9, key, strlen(key));

		cout << "Receive Message from " << strIpAddr << ": " << plainText << endl;
		if(memcmp("quit", strSocketBuffer, 4) == 0)
		{
			cout << "Quit!" << endl;
			break;
		}
	}
	return 0;
}

size_t TotalRecv(int s, void *buf, size_t len, int flags)
{
	size_t nCurSize = 0;
	while(nCurSize < len)
	{
		size_t nRes = recv(s,((char*)buf)+nCurSize,len-nCurSize,flags); 
		if(nRes<0 || nRes+nCurSize>len)   
		    return -1;   
		nCurSize += nRes; 
	}
	return nCurSize;
}
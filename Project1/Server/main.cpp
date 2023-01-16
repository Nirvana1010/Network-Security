#include <iostream>
#include <WinSock.h>
#include <Windows.h>
#include <stdlib.h>
#include "define.h"
#include "DES.h"

#pragma comment( lib, "Wsock32.lib" )

using namespace std;

char strSocketBuffer[256];
char strStdinBuffer[256];
char key[9] = {'W', 'H', 'I', 'T', 'E', 'H', 'A', 'T', '\0'};
SOCKET nListenSocket = INVALID_SOCKET, nAcceptSocket = INVALID_SOCKET;

void SecretChat(int nSock, char *pRemoteName, char *pKey);
size_t TotalRecv(int s, void *buf, size_t len, int flags);
DWORD WINAPI MyThread(LPVOID lpParameter);

int main()
{
	int nLength = 256;
	struct sockaddr_in sLocalAddr, sRemoteAddr;
	WSADATA sentWsa;
	CDesOperate des;
	char clear[8] = {'B', 'L', 'A', 'C', 'K', 'H', 'A', 'T'};
	char cipher[129] = "";
	char decode_text[17] = "";
	
	if(WSAStartup(WSA_VERSION, &sentWsa) != 0)
	{
		cout << "Socket Startup Failed" << endl;
		return 1;
	}

	nListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	nAcceptSocket = socket(AF_INET, SOCK_STREAM, 0);

	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_port = htons(1010);
	sLocalAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	sRemoteAddr.sin_family = AF_INET;
	sRemoteAddr.sin_port = htons(80);
	sRemoteAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(nListenSocket == INVALID_SOCKET || nAcceptSocket == INVALID_SOCKET)
	{
		cout << "Socket Error" << endl;
		exit(0);
	}

	if(bind(nListenSocket, (struct sockaddr*) &sLocalAddr, sizeof(sockaddr)) == -1)
	{
		cout << "Bind Error" << endl;
		exit(0);
	}

	if(listen(nListenSocket, 5) == -1)
	{
		cout << "Listen Error" << endl;
		exit(0);
	}

	cout << "Listening..." << endl;
	cout << "(Send 'quit' to end chatting)" << endl;
	nAcceptSocket = accept(nListenSocket, (struct sockaddr*) &sRemoteAddr, &nLength);

	closesocket(nListenSocket);

	cout << "Server: got connection of " << inet_ntoa(sRemoteAddr.sin_addr)
		<< ", port: " << ntohs(sRemoteAddr.sin_port) 
		<< ", socket:" << nAcceptSocket << endl;

	SecretChat(nAcceptSocket, inet_ntoa(sRemoteAddr.sin_addr), key);

	closesocket(nAcceptSocket);
	
}

void SecretChat(int nSock, char *pRemoteName, char *pKey)
{
	CDesOperate cDes;
	char plainText[9] = "";
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
		memset(&strSocketBuffer, 0, 256);
		int nLength = 0;
		nLength = TotalRecv(nSock, strSocketBuffer, 64, 0);

		if(nLength == -1)
			continue;

		//DECODE
		cDes.Decry(strSocketBuffer, nLength, plainText, 9, pKey, strlen(pKey));

		cout << "Recerve Message from " << pRemoteName << ": " << plainText << endl;
		if(memcmp("quit", plainText, 4) == 0)
		{
			cout << "Quit!" << endl;
			break;
		}
	}
}

DWORD WINAPI MyThread(LPVOID lpParameter)
{
	CDesOperate cDes;  
	char *cipher;
	while(1)
	{
		memset(&strStdinBuffer, 0, 256);
		while(strStdinBuffer[0] == 0)
			cin.getline(strStdinBuffer, 256);
		int nLen = 256;

		//ENCODE
		int length = ((strlen(strStdinBuffer)/9)+1)*8*8 + 1;
		cipher = new char[length];
		cDes.Encry(strStdinBuffer, strlen(strStdinBuffer), cipher, length, key, strlen(key));

		if(send(nAcceptSocket,cipher, strlen(cipher), 0) != strlen(cipher))
			cout << "Send Error" << endl;
		else if(memcmp("quit", strSocketBuffer, 4) == 0)
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
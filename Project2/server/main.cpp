#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <stdlib.h>
#include "DES.h"
#include "RSA.h"

#pragma comment( lib, "Wsock32.lib" )
#define STDIN 0

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
	char clear[8] = {'B', 'L', 'A', 'C', 'K', 'H', 'A', 'T'};
	char decode_text[17] = "";
	char desKey[9] = "";

	CRSASection cRsaSection;
	PublicKey cRsaPublicKey;
	unsigned __int64 EncryptDesKey[8];
	cRsaPublicKey = cRsaSection.GetPublicKey();
	
	unsigned short *pDesKey = (unsigned short *)desKey;
	unsigned short *pDecryptDesKey = (unsigned short *)EncryptDesKey;
	
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

	if(send(nAcceptSocket, (char *)(&cRsaPublicKey), sizeof(cRsaPublicKey),0) != sizeof(cRsaPublicKey)) 
	{  
		perror("send");  
		exit(0); 
	} 
	else 
	{  
		printf("successful send the RSA public key. \n"); 
	}

	if(8*sizeof(unsigned __int64) != 
		TotalRecv(nAcceptSocket, (char *)EncryptDesKey, 8*sizeof(unsigned __int64),0)) 
	{  
		perror("TotalRecv DES key error");  
		exit(0); 
	} 
	else 
	{  
		printf("successful get the DES key. \n");    
		for(int i = 0; i < 8; i++)
		{
			pDecryptDesKey[i] = cRsaSection.Decry(EncryptDesKey[i]);
		}
	} 

	char *res = (char*) pDecryptDesKey;

	SecretChat(nAcceptSocket, inet_ntoa(sRemoteAddr.sin_addr), res);

	closesocket(nAcceptSocket);
	
}

void SecretChat(int nSock, char *pRemoteName, char *pKey)
{
	CDesOperate cDes;
	fd_set cHandleSet;
	struct timeval tv;
	int nRet;
	char plainText[9] = "";
	char *cipher;
	if(strlen(pKey)!=8)  
	{   
		printf("Key length error");   
		return ;  
	}   
	
	while(1)  
	{    
		FD_ZERO(&cHandleSet);    
		FD_SET(nSock, &cHandleSet);    
		FD_SET(0, &cHandleSet);   
		tv.tv_sec = 1;   
		tv.tv_usec = 0;    
		nRet = select(nSock+1, &cHandleSet, NULL, NULL, &tv); 
		if(nRet = SOCKET_ERROR)   
		{    
			printf("Select ERROR!\n");
			errno = WSAGetLastError();
			printf("errno: %d\n", errno);
			break;   
		}   
		if(0 == nRet)   
			continue;
		if(FD_ISSET(nSock,&cHandleSet))   
		{    
			memset(&strSocketBuffer, 0, 256);   
			int nLength = 0;    
			nLength = TotalRecv(nSock, strSocketBuffer, 64, 0);   
			if(nLength == -1)
				continue;    
			cDes.Decry(strSocketBuffer, nLength, plainText, 9, pKey, strlen(pKey));

			cout << "Recerve Message from " << pRemoteName << ": " << plainText << endl;
			if(memcmp("quit", plainText, 4) == 0)
			{
				cout << "Quit!" << endl;
				break;
			}     
		}     
		if(FD_ISSET(0,&cHandleSet))   
		{        
			memset(&strStdinBuffer, 0, 256);
			while(strStdinBuffer[0] == 0)
				cin.getline(strStdinBuffer, 256);
			int nLen = 256;  

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

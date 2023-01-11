#include <iostream>
#include <WinSock.h>
#include <Windows.h>
#include <stdlib.h>
#include "RSA.h"
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
	CRSASection cRsaSection;
	char desKey[9] = "";
	PublicKey cRsaPublicKey;
	unsigned short *pDesKey = (unsigned short *)desKey;
	unsigned __int64 EncryptDesKey[8];
	
	char clear[13] = {'B', 'L', 'A', 'C', 'K', 'H', 'A', 'T', 'h', 'e', 'l', 'l', 'o'};
	char cipher[129] = "";
	char decode_text[17] = "";
	
	//des.Encry(clear, 13, cipher, 129, key, 8);
	//cout << endl << "cipher: " <<  cipher << endl;

	//des.Decry(cipher, 129, decode_text, 13, key, 8);
	//decode_text[16] = '\0';
	////strncpy(decode_text, decode_text, 5);
	//cout << decode_text << endl;

	//cout << endl << "hello: " << endl;
	//strncpy(clear, "hello", 5);
	//clear[5] = '\0';
	//memset(cipher, '\0', sizeof(cipher));
	//memset(decode_text, '\0', sizeof(decode_text));
	//
	//des.Encry(clear, 5, cipher, 65, key, 8);
	//cout << endl << "cipher: " <<  cipher << endl;

	//des.Decry(cipher, 65, decode_text, 9, key, 8);
	//decode_text[16] = '\0';
	////strncpy(decode_text, decode_text, 5);
	//cout << decode_text << endl;
	//
	//
	cout << "Please input the server IP address:" << endl;
	cin >> strIpAddr;

	if(WSAStartup(WSA_VERSION, &sentWsa) != 0)
	{
		cout << "SocketÆô¶¯Ê§°Ü" << endl;
		return 1;
	}

	if((nConnectSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cout << "Socket Error" << endl;
		exit(0);
	}

	sDestAddr.sin_family = AF_INET;
	sDestAddr.sin_port = htons(1010);//Server ¶Ë¿ÚºÅ
	sDestAddr.sin_addr.s_addr = inet_addr(strIpAddr);

	if(connect(nConnectSocket, (struct sockaddr*) &sDestAddr, sizeof(sDestAddr)) != 0)
	{
		cout << "Connect Error" << endl;
		exit(0);
	}
	else
	{

		printf("Connect Success!  \n"); 
		des.GenerateDesKey(desKey);
		//cout << "init: " << desKey << endl;
		printf("Create DES key success\n"); 
		if(sizeof(cRsaPublicKey) == 
			TotalRecv(nConnectSocket,(char *)&cRsaPublicKey, sizeof(cRsaPublicKey),0)) 
		{  
			printf("Successful get the RSA public Key\n");  
		} 
		else 
		{  
			perror("Get RSA public key ");  
			exit(0); 
		} 

		//cout << "public key: n	" << cRsaPublicKey.nN << endl;
		//cout << "public key: e	" << cRsaPublicKey.nE << endl;
		
		for(int i = 0; i < 8; i++)
		{
			EncryptDesKey[i] = cRsaSection.Encry(pDesKey[i], cRsaPublicKey);
			//cout << EncryptDesKey[i] << " ";
		}
		if(sizeof(UINT64)*8 != send(nConnectSocket, (char *)EncryptDesKey, sizeof(UINT64)*8,0)) 
		{  
			perror("Send DES key Error");  
			exit(0); 
		} 
		else 
			printf("Successful send the encrypted DES Key\n");  
		cout << "Begin to chat ..." << endl;
		SecretChat(nConnectSocket, inet_ntoa(sDestAddr.sin_addr), desKey);
	}
	closesocket(nConnectSocket);
	
}

void SecretChat(int nSock, char *pRemoteName, char *pKey)
{
	CDesOperate cDes;  
	char *cipher;
	//cout << strlen(pKey) << endl;
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
			//if(fgets(strStdinBuffer, 256, stdin) == NULL)
			//	continue;
			cin.getline(strStdinBuffer, 256);
		//fgets(strStdinBuffer, 256, stdin);
		//cin >> strStdinBuffer;
		int nLen = 256;

		//TODO:ENCODE
		int length = ((strlen(strStdinBuffer)/9)+1)*8*8 + 1;
		cipher = new char[length];
		//cout << sizeof(strStdinBuffer) << endl;
		cDes.Encry(strStdinBuffer, strlen(strStdinBuffer), cipher, length, pKey, strlen(pKey));
		//cout << "send cipher:" << cipher << endl;
		//cout << strlen(cipher) << endl;

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
		//if(nLength != 256)
		//	break;
		//else
		//	int nLen = 256;

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
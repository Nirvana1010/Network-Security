#pragma once
typedef int INT32;

class CDesOperate
{
private:   
	//Whole process of Encryption/Decryption
	void HandleData(char text[64], char key[64], char cipher[65], bool encode); 

	//Data Transfer
	void MakeData(char *left, char *right, char *key); 

	//Generate 16 sub-keys
	void MakeKey(char *key, char (*subkey)[49]);

public:  
	//CDesOperate(); 
	//~CDesOperate();

	//Encryption
	void Encry(char* pPlaintext,  int nPlaintextLength,  char *pCipherBuffer,  
		int nCipherBufferLength,  char *pKey, int nKeyLength); 

	//Decryption
	void Decry(char* pCipher,  int nCipherBufferLength,  char *pPlaintextBuffer,  
		int nPlaintextBufferLength,  char *pKey, int nKeyLength);

	//Generate DES key
	void GenerateDesKey(char *key);
};
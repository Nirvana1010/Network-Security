#pragma once
typedef int INT32;

class CDesOperate
{
private:   
	// completed process of encryption / decryption
	void HandleData(char text[64], char key[64], char cipher[65], bool encode); 

	// transfer data
	void MakeData(char *left, char *right, char *key); 

	// generate 16 sub-keys
	void MakeKey(char *key, char (*subkey)[49]);

public:  
	// encryption
	void Encry(char* pPlaintext,  int nPlaintextLength,  char *pCipherBuffer,  
		int nCipherBufferLength,  char *pKey, int nKeyLength); 

	// decryption
	void Decry(char* pCipher,  int nCipherBufferLength,  char *pPlaintextBuffer,  
		int nPlaintextBufferLength,  char *pKey, int nKeyLength);
};
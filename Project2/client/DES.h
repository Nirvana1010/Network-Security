#pragma once
typedef int INT32;

class CDesOperate
{
private:   
	//一次完整的加密/解密
	void HandleData(char text[64], char key[64], char cipher[65], bool encode); 

	//实现每一轮除去初始置换和逆初始置换的中间操作
	void MakeData(char *left, char *right, char *key); 

	//形成16个密钥中的每一个子密钥
	void MakeKey(char *key, char (*subkey)[49]);

public:  
	//CDesOperate(); 
	//~CDesOperate();

	//加密
	void Encry(char* pPlaintext,  int nPlaintextLength,  char *pCipherBuffer,  
		int nCipherBufferLength,  char *pKey, int nKeyLength); 

	//解密
	void Decry(char* pCipher,  int nCipherBufferLength,  char *pPlaintextBuffer,  
		int nPlaintextBufferLength,  char *pKey, int nKeyLength);

	//生成随机 DES key
	void GenerateDesKey(char *key);
};
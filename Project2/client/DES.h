#pragma once
typedef int INT32;

class CDesOperate
{
private:   
	//һ�������ļ���/����
	void HandleData(char text[64], char key[64], char cipher[65], bool encode); 

	//ʵ��ÿһ�ֳ�ȥ��ʼ�û������ʼ�û����м����
	void MakeData(char *left, char *right, char *key); 

	//�γ�16����Կ�е�ÿһ������Կ
	void MakeKey(char *key, char (*subkey)[49]);

public:  
	//CDesOperate(); 
	//~CDesOperate();

	//����
	void Encry(char* pPlaintext,  int nPlaintextLength,  char *pCipherBuffer,  
		int nCipherBufferLength,  char *pKey, int nKeyLength); 

	//����
	void Decry(char* pCipher,  int nCipherBufferLength,  char *pPlaintextBuffer,  
		int nPlaintextBufferLength,  char *pKey, int nKeyLength);

	//������� DES key
	void GenerateDesKey(char *key);
};
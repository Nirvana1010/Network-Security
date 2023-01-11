#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

class MD5 
{ 
public:  
	MD5();  
	MD5(const string &str); 
	MD5(ifstream &in); 
	//�Ը������ȵ����������� MD5 ���� 
	void Update(const void* input,size_t length); 
	//�Ը������ȵ��ַ������� MD5 ���� 
	void Update(const string &str); 
	//���ļ��е����ݽ��� MD5 ���� 
	void Update(ifstream &in); 
	const BYTE* GetDigest(); //�� MD5 ժҪ���ֽ�������ʽ��� 
	string Tostring(); //�� MD5 ժҪ���ַ�����ʽ��� 
	void Reset(); 
	//���ó�ʼ���� 
 
private: 
	//�Ը������ȵ��ֽ������� MD5 ���� 
	void Update(const BYTE* input,size_t length); 
	void Stop(); //������ֹժҪ������̣����ժҪ 
	void Transform(const BYTE block[64]); 
	//����Ϣ������� MD5 ���� 
	//��˫����ת��Ϊ�ֽ��� 
	void Encode(const DWORD *input, BYTE *output, size_t length); 
	//���ֽ���ת��Ϊ˫���� 
	void Decode(const BYTE *input, DWORD *output, size_t length); 
	//���ֽ�������ʮ�������ַ�����ʽ��� 
	string BytesToHexString(const BYTE *input, size_t length); 
 
private: 
	DWORD state[4]; //���ڱ�ʾ 4 ����ʼ���� 
	DWORD count[2]; //���ڼ�����count[0]��ʾ��λ��count[1]��ʾ��λ 
	BYTE  buffer_block[64]; //���ڱ����������а��黮�ֺ�ʣ�µı����� 
	BYTE  digest[16]; //���ڱ��� 128 ���س��ȵ�ժҪ 
	bool  is_finished; //���ڱ�־ժҪ��������Ƿ����      
	static const BYTE padding[64]; //���ڱ�����Ϣ�����������ݿ� 
	static const char hex[16]; //���ڱ��� 16 ���Ƶ��ַ�
}; 
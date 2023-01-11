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
	//对给定长度的输入流进行 MD5 运算 
	void Update(const void* input,size_t length); 
	//对给定长度的字符串进行 MD5 运算 
	void Update(const string &str); 
	//对文件中的内容进行 MD5 运算 
	void Update(ifstream &in); 
	const BYTE* GetDigest(); //将 MD5 摘要以字节流的形式输出 
	string Tostring(); //将 MD5 摘要以字符串形式输出 
	void Reset(); 
	//重置初始变量 
 
private: 
	//对给定长度的字节流进行 MD5 运算 
	void Update(const BYTE* input,size_t length); 
	void Stop(); //用于终止摘要计算过程，输出摘要 
	void Transform(const BYTE block[64]); 
	//对消息分组进行 MD5 运算 
	//将双字流转换为字节流 
	void Encode(const DWORD *input, BYTE *output, size_t length); 
	//将字节流转换为双字流 
	void Decode(const BYTE *input, DWORD *output, size_t length); 
	//将字节流按照十六进制字符串形式输出 
	string BytesToHexString(const BYTE *input, size_t length); 
 
private: 
	DWORD state[4]; //用于表示 4 个初始向量 
	DWORD count[2]; //用于计数，count[0]表示低位，count[1]表示高位 
	BYTE  buffer_block[64]; //用于保存计算过程中按块划分后剩下的比特流 
	BYTE  digest[16]; //用于保存 128 比特长度的摘要 
	bool  is_finished; //用于标志摘要计算过程是否结束      
	static const BYTE padding[64]; //用于保存消息后面填充的数据块 
	static const char hex[16]; //用于保存 16 进制的字符
}; 
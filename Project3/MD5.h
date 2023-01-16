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
	//MD5 digest for keyboard input
	void Update(const void* input,size_t length); 
	//MD5 digest for string
	void Update(const string &str); 
	//MD5 digest for given file
	void Update(ifstream &in); 
	const BYTE* GetDigest(); //Output MD5
	string Tostring(); //Output MD5 as string
	void Reset(); //Reset variables
 
private: 
	//generate MD5 digest
	void Update(const BYTE* input,size_t length); 
	void Stop(); //stop process
	void Transform(const BYTE block[64]); 
	//BYTE -> DWORD
	void Encode(const DWORD *input, BYTE *output, size_t length); 
	//DWORD -> BYTE
	void Decode(const BYTE *input, DWORD *output, size_t length); 
	//output bytes as hex string
	string BytesToHexString(const BYTE *input, size_t length); 
 
private: 
	DWORD state[4]; //initial states
	DWORD count[2]; //counts: 0-low, 1-high
	BYTE  buffer_block[64]; 
	BYTE  digest[16]; 
	bool  is_finished;    
	static const BYTE padding[64]; 
	static const char hex[16]; 
}; 
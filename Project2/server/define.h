#ifndef MAKEWORD		
#define MAKEWORD(l,h) ((WORD)(((BYTE)(l))|(((WORD)(BYTE)(h))<<8)))
#endif

#define WSA_MAJOR_VERSION 1
#define WSA_MINOR_VERSION 1
#define WSA_VERSION MAKEWORD(WSA_MAJOR_VERSION, WSA_MINOR_VERSION)

struct PublicKey
{
	int nE;
	int nN;
};

struct RsaParam
{
	unsigned int p;
	unsigned int q;
	unsigned int n;
	unsigned int f;
	unsigned __int64 d;
	unsigned int e;
	unsigned int s;
};
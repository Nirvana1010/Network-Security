#include "define.h"
#pragma once

class CRSASection
{
private:
	struct RsaParam m_cParam;

	RsaParam RsaGetParam();

	//求最大公约数
	unsigned int Gcd(unsigned int p, unsigned int q);

	//生成私钥
	unsigned __int64 Euclid(unsigned int e, unsigned int t_n);
	
public:
	PublicKey GetPublicKey();
	CRSASection();
	static unsigned __int64 Encry(unsigned short nSorce, PublicKey &cKey);
	unsigned short Decry(unsigned __int64 nSorce);
	
};
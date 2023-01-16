#include "define.h"
#pragma once

class CRSASection
{
private:
	struct RsaParam m_cParam;

	RsaParam RsaGetParam();

	//get greatest common divisor
	unsigned int Gcd(unsigned int p, unsigned int q);

	//geterate private key
	unsigned __int64 Euclid(unsigned int e, unsigned int t_n);
	
public:
	PublicKey GetPublicKey();
	CRSASection();
	static unsigned __int64 Encry(unsigned short nSorce, PublicKey &cKey);
	unsigned short Decry(unsigned __int64 nSorce);
	
};
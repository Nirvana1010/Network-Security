#include "RSA.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

PublicKey CRSASection::GetPublicKey()
{
	PublicKey cTmp;
	cTmp.nN = m_cParam.n;
	cTmp.nE = m_cParam.e;
	return cTmp;
}

CRSASection::CRSASection()
{
	m_cParam = RsaGetParam();
}

RsaParam CRSASection::RsaGetParam()
{
	RsaParam Rsa = {0};
	int t;
	Rsa.p = 211;
	Rsa.q = 307;
	Rsa.n = Rsa.p * Rsa.q;
	Rsa.f = (Rsa.p-1) * (Rsa.q-1);
	
	do
	{
		srand((int)time(NULL));
		Rsa.e = rand() % Rsa.f;
		Rsa.e |= 1;
	}while(Gcd(Rsa.e, Rsa.f) != 1);
	
	Rsa.d = Euclid(Rsa.e, Rsa.f);
	//cout << "d: " << Rsa.d << endl;
	Rsa.s = 0;
	t = Rsa.n >> 1;
	while(t)
	{
		Rsa.s++;
		t >>= 1;
	}
	return Rsa;
}

unsigned int CRSASection::Gcd(unsigned int p, unsigned int q)
{
	unsigned int a = p > q ? p : q;
	unsigned int b = p < q ? p : q;
	unsigned int t;

	if(p == q)
		return p;	//Greatest common divisor is themselves if 2 number equal
	 else     
	 {         
		 while(b)    //Euclidean algorithm, gcd(a,b)=gcd(b,a-qb)         
		 {             
			 a=a % b;             
			 t=a;             
			 a=b;             
			 b=t;         
		 }         
		 return a;     
	}
}

unsigned __int64 CRSASection::Euclid(unsigned int e, unsigned int t_n)
{
	unsigned __int64 Max=0xffffffffffffffff - t_n;  
	unsigned __int64 i=1; 

	while(1)  
	{   
		if(((i*t_n)+1) % e == 0)   
		{    
			return ((i*t_n) + 1) / e;   
		}   
		i++;   
		unsigned __int64 Tmp = (i+1) * t_n;   
		if(Tmp > Max)   
		{    
			return 0;   
		}  
	}  
	return 0; 
}

inline unsigned __int64 MulMod(unsigned __int64 a, unsigned __int64 b, unsigned __int64 n) 
{     
	return (a%n) * (b%n) % n; 
}

unsigned __int64 PowMod(unsigned __int64 base, unsigned __int64 pow, unsigned __int64 n) 
{ 
    unsigned __int64 a=base, b=pow, c=1;     
	while(b)     
	{         
		while(!(b & 1))         
		{             
			b>>=1;                        
			a=MulMod(a, a, n);         
		}         
		b--;                
		c=MulMod(a, c, n);     
	}     
	return c; 
} 

unsigned __int64 CRSASection::Encry(unsigned short nSorce, PublicKey &cKey)
{
	return PowMod(nSorce, cKey.nE, cKey.nN);
}

unsigned short CRSASection::Decry(unsigned __int64 nSorce)  
{   
	unsigned __int64 nRes = PowMod(nSorce, m_cParam.d, m_cParam.n);   
	unsigned short * pRes = (unsigned short*)&(nRes);   
	if(pRes[1]!=0 || pRes[3]!=0 || pRes[2]!=0)   
	{
		//error    
		return 0;   
	}   
	else   
	{    
		return pRes[0];
	}  
} 
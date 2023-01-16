#include "DES.h"
#include "tables.h"
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
using namespace std;

int atoi(char a)
{
	if(a == '1')
		return 1;
	else if(a == '0')
		return 0;
	else 
		return -1;
}

//string -> 7bit ASCII + parity check
void decToBin(int dec, char cha[9])
{
	int result = 0, temp = dec, j = 1;
	int odd = 0;
	int i;
	stringstream ss;
	while(temp)
	{
		result = result + j * (temp % 2);
		temp = temp / 2;
		j = j * 10;
	}
	ss << result;
	ss >> cha;
	for(i = 0; i < 7; i++)
		if(cha[i] == 1)
			odd++;
	if(odd % 2 == 0)
		cha[7] = '1';
	else
		cha[7] = '0';
	return ;
}

char BinToDec(char *cha)
{
	int num = 0;
	char tmp;
	num = atoi(cha[0])*64 + atoi(cha[1])*32 + atoi(cha[2])*16 + atoi(cha[3])*8
		+ atoi(cha[4])*4 + atoi(cha[5])*2 + atoi(cha[6]);
	tmp = (char)num;

	return tmp;
}

void permute(char *ini, char *res, int n, int *PC);

void shift(char *ini, char *res, int n, int bit);

//generate sub-key
void CDesOperate::MakeKey(char *key, char (*subkey)[49])
{
	char left_res[29] = "", right_res[29] = "";
	char tmp[29] = "";
	char LS_res[57] = "";
	char key_res[57] = "";
	int i = 0, j = 0;

	permute(key, key_res, 56, keypermute);//KEY
	key_res[56] = '\0';
	//permute(right, right_res, 28, keyright);//D0

	for(i = 0; i < 28; i++)
	{
		left_res[i] = key_res[i];
		right_res[i] = key_res[i+28];
	}
	left_res[28] = '\0';
	right_res[28] = '\0';

	for(i = 0; i < 16; i++)
	{
		//Ci
		shift(left_res, tmp, 28, lefttable[i]);
		tmp[28] = '\0';
		strcpy(left_res, tmp);
		//Di
		shift(right_res, tmp, 28, lefttable[i]);
		strcpy(right_res, tmp);
		//Ki
		strncpy(LS_res, left_res, 28);
		strncpy(LS_res+28, right_res, 28);
		LS_res[56] = '\0';
		permute(LS_res, subkey[i], 48, keychoose);
		subkey[i][48] = '\0';
	}
}

//permutation
void permute(char *ini, char *res, int n, int *PC)
{
	int i = 0;
	for(i = 0; i < n; i++)
		res[i] = ini[PC[i]-1];
	
	return ;
}

//left shift
void shift(char *ini, char *res, int n, int bit)
{
	int i;
	for(i = 0; i < n; i++)
		res[i] = ini[(i+bit)%n];

	return ;
}

void xor(char *right, char *key, int n, char *res)
{
	int i;
	for(i = 0; i < n; i++)
		right[i] == key[i] ? res[i] = '0' : res[i] = '1';

	return ;
}

//S-box permutation
void S_box(char (*in)[7], char out[33])
{
	int i;
	int line, column;
	int tmp;
	int index;
	for(i = 0; i < 8; i++)
	{
		index = atoi(in[i][0])*32 + atoi(in[i][1])*16 + atoi(in[i][2])*8 + 
			atoi(in[i][3])*4 + atoi(in[i][4])*2 + atoi(in[i][5]);
		tmp = des_S[i][index];

		switch(tmp)
		{
		case 0:
			strncpy(out+(i*4), "0000", 4);
			break;
		case 1:
			strncpy(out+(i*4), "0001", 4);
			break;
		case 2:
			strncpy(out+(i*4), "0010", 4);
			break;
		case 3:
			strncpy(out+(i*4), "0011", 4);
			break;
		case 4:
			strncpy(out+(i*4), "0100", 4);
			break;
		case 5:
			strncpy(out+(i*4), "0101", 4);
			break;
		case 6:
			strncpy(out+(i*4), "0110", 4);
			break;
		case 7:
			strncpy(out+(i*4), "0111", 4);
			break;
		case 8:
			strncpy(out+(i*4), "1000", 4);
			break;
		case 9:
			strncpy(out+(i*4), "1001", 4);
			break;
		case 10:
			strncpy(out+(i*4), "1010", 4);
			break;
		case 11:
			strncpy(out+(i*4), "1011", 4);
			break;
		case 12:
			strncpy(out+(i*4), "1100", 4);
			break;
		case 13:
			strncpy(out+(i*4), "1101", 4);
			break;
		case 14:
			strncpy(out+(i*4), "1110", 4);
			break;
		case 15:
			strncpy(out+(i*4), "1111", 4);
			break;
		}
	}
	out[32] = '\0';
}

void CDesOperate::MakeData(char *left, char *right, char *key)
{
	char extend[49] = "";
	char xor_res[49] = "";
	char S_in[8][7] = {{""}, {""}};
	char S_out[33] = "";
	char res[33] = "";
	char tmp[33] = "";
	int i, j, k;

	permute(right, extend, 48, des_E);
	extend[48] = '\0';
	//cout << "entend: " << extend << endl;
	xor(extend, key, 48, xor_res);
	xor_res[48] = '\0';
	//cout << "xor_res: " << xor_res << endl;

	k = 0;
	for(i = 0; i < 8; i++)
	{
		strncpy(S_in[i], xor_res + i*6, 6);
		S_in[i][6] = '\0';
	}

	S_box(S_in, S_out);
	//cout << "S_box: " << S_out << endl;

	permute(S_out, res, 32, des_P);
	res[32] = '\0';
	//cout << "res: " << res << endl;

	//Li+1, Ri+1
	strcpy(tmp, right);
	xor(left, res, 32, right);
	right[32] = '\0';
	strcpy(left, tmp);
	left[32] = '\0';
	
	return ;
}

void CDesOperate::HandleData(char *text, char *key, char cipher[65], bool encode)
{
	char text_permute[65] = "";
	char left[33] = "", right[33] = "";
	char subKey[16][49];
	char ite_res[65] = "";
	char tmp[49] = "";
	int i;
	char decode_tmp[33] = "";

	if(encode)
		permute(text, text_permute, 64, pc_first);
	else
	{
		permute(text, text_permute, 64, pc_first);
		strncpy(decode_tmp, text_permute, 32);
		strncpy(text_permute, text_permute+32, 32);
		strncpy(text_permute+32, decode_tmp, 32);
	}

	text_permute[64] = '\0';
	strncpy(left, text_permute, 32);
	left[32] = '\0';
	strncpy(right, text_permute+32, 32);
	right[32] = '\0';

	MakeKey(key, subKey);
	
	if(encode == false)
	{
		for(i = 0; i < 8; i++)
		{
			strncpy(tmp, subKey[i], 48);
			strncpy(subKey[i], subKey[15-i], 48);
			strncpy(subKey[15-i], tmp, 48);
		}
	}
	
	for(i = 0; i < 16; i++)
	{
		MakeData(left, right, subKey[i]);
	}

	if(encode)
	{
		strncpy(ite_res, left, 32);
		strncpy(ite_res+32, right, 32);
		permute(ite_res, cipher, 64, pc_last);
	}
	else
	{
		strncpy(ite_res, right, 32);
		strncpy(ite_res+32, left, 32);
		permute(ite_res, cipher, 64, pc_last);
	}
	return ;
}

void CDesOperate::Encry(char* pPlaintext,  int nPlaintextLength,  char *pCipherBuffer,  
		int nCipherBufferLength,  char *pKey, int nKeyLength)
{
	int num = (nPlaintextLength) / 8;
	int remain = (nPlaintextLength) % 8;
	char tmp_text[8] = "";
	char tmp_cha[8] = "";
	char cleartext[65]= "";
	char tmp_key[8] = "";
	char key[65] = "";
	char subkey[16][49] = {{""}, {""}};
	char tmp_cipher[65] = "";
	char nulltext[64] = "";
	int i = 0;
	int j = 0;

	if(nKeyLength != 8)
	{
		cout << "invalid KEY" << endl;
		exit(0);
	}
	
	while(num > 0)
	{
		strncpy(tmp_text, pPlaintext+i*8, 8);
		decToBin((int)tmp_text[0], tmp_cha);
		strncpy(cleartext, tmp_cha, 8);
		for(j = 1; j < 8; j++)
		{
			decToBin((int)tmp_text[j], tmp_cha);
			strncat(cleartext+8, tmp_cha, 8);
		}

		//KEY
		strncpy(tmp_key, pKey, 8);
		decToBin((int)tmp_key[0], tmp_cha);
		strncpy(key, tmp_cha, 8);
		for(j = 1; j < 8; j++)
		{
			decToBin((int)tmp_key[j], tmp_cha);
			strncat(key+8, tmp_cha, 8);
		}
		//cout << "key:" << key << endl;
		
		HandleData(cleartext, key, tmp_cipher, true);
		strncpy(pCipherBuffer + i*64, tmp_cipher, 64);
		
		//cout << pCipherBuffer << endl;

		num--;
		i++;
	}
	
	if(remain != 0)
	{
		memset(key, '\0', sizeof(key));
		memset(cleartext, '\0', sizeof(cleartext));
		num = (nPlaintextLength) / 8;
		strncpy(tmp_text, &pPlaintext[num*8], remain);
		tmp_text[remain] = '\0';
		decToBin((int)tmp_text[0], tmp_cha);
		strncpy(cleartext, tmp_cha, 8);
		for(j = 1; j < remain; j++)
		{
			decToBin((int)tmp_text[j], tmp_cha);
			strncat(cleartext, tmp_cha, 8);
		}
		//memset(cleartext + remain*8, '0', 64 - remain*8);
		memset(nulltext, '0', 64);
		strncat(cleartext, nulltext, 64-remain*8);
		cleartext[64] = '\0';
		//cout << cleartext << endl;

		//KEY
		strncpy(tmp_key, pKey, 8);
		decToBin((int)tmp_key[0], tmp_cha);
		strncpy(key, tmp_cha, 8);
		for(j = 1; j < 8; j++)
		{
			decToBin((int)tmp_key[j], tmp_cha);
			strncat(key+8, tmp_cha, 8);
		}
		
		HandleData(cleartext, key, tmp_cipher, true);
		strncpy(pCipherBuffer + i*64, tmp_cipher, 64);

		//cout << "cleartext: " << cleartext << endl;
	}

	pCipherBuffer[nCipherBufferLength-1] = '\0';
}

void CDesOperate::Decry(char* pCipher,  int nCipherBufferLength,  char *pPlaintextBuffer,  
		int nPlaintextBufferLength,  char *pKey, int nKeyLength)
{
	int num = nCipherBufferLength / 64;
	char tmp_cipher[65] = "";
	char tmp_key[8] = "";
	char key[65] = "";
	char tmp_plain[65] = "";
	char *cleartext;
	char tmp_cha[8] = "";
	char tmp_text[8] = "";
	char tmp_Plaintext[8] = "";
	char single_text;
	int char_count = 0;
	
	cleartext = new char[nCipherBufferLength];
	memset(cleartext, '\0', nCipherBufferLength);
	memset(pPlaintextBuffer, '\0', nPlaintextBufferLength);

	int i = 0, j;

	if(nKeyLength != 8)
	{
		cout << "invalid KEY" << endl;
		exit(0);
	}

	//KEY
	memset(key, '\0', 64);
	strncpy(tmp_key, pKey, 8);
	decToBin((int)tmp_key[0], tmp_cha);
	strncpy(key, tmp_cha, 8);
	for(j = 1; j < 8; j++)
	{
		decToBin((int)tmp_key[j], tmp_cha);
		strncat(key+8, tmp_cha, 8);
	}
	//cout << "key:" << key << endl;

	while(num > 0)
	{
		strncpy(tmp_cipher, pCipher+i*64, 64);
		tmp_cipher[64] = '\0';

		HandleData(tmp_cipher, key, tmp_plain, false);
		strncpy(cleartext, tmp_plain, 64);
		//cout << "docode: " << cleartext << endl;

		for(j = 0; j < 8; j++)
		{
			strncpy(tmp_Plaintext, cleartext+j*8, 7);
			tmp_Plaintext[7] = '\0';
			single_text = BinToDec(tmp_Plaintext);
			pPlaintextBuffer[char_count] = single_text;
			char_count++;
		}

		num--;
		i++;
	}
}

void CDesOperate::GenerateDesKey(char *key)
{
	srand((int)time(0));
	int i;
	int tmp;
	char *alpha = NULL;
	for(i = 0; i < 8; i++)
	{
		tmp = rand() % 26;
		tmp += 65;
		alpha = (char *)&tmp;
		strncat(key, alpha, 1);
	}
	key[8] = '\0';
}
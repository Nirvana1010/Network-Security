#include <iostream>
#include <string>
#include <iomanip>
#include "MD5.h"

using namespace std;

void PrintMD5(const string &str, MD5 &md5) {
	cout << "MD5(\"" << str << "\") = " << md5.Tostring() << endl;
}

int main()
{
	char input;
	MD5 md5;

	cout << "*********���� MD5 �㷨���ļ�������У�����������*********" << endl;
	cout << "Usage��" << endl;
	cout << "[-h]  --help information" << endl;
	cout << "[-t]  --test MD5 application " << endl;
	cout << "[-c]  [file path of the file computed] " << endl;
	cout << "        --compute MD5 of the given file " << endl;
	cout << "[-v]  [file path of the file validated] " << endl;
	cout << "        --validate the integrality of a given file by manual input MD5 value" << endl;
	cout << "[-f]  [file path of the file validated]  [file path of the .md5 file] " << endl;
	cout << "        --validate the integrality of a given file by read MD5 value from .md5 file" << endl;
	cout << "[-q]  --quit the program " << endl;

	while(1)
	{
		cout << "������ѡ�" << endl;
		cin >> input;

		if(input == 't')
		{
			md5.Update("");
			PrintMD5("", md5);

			md5.Update("a");
			PrintMD5("a", md5);

			md5.Update("bc");
			PrintMD5("abc", md5);

			md5.Reset();
			md5.Update("message digest");
			PrintMD5("message digest", md5);

			md5.Reset();
			md5.Update("abcdefghijklmnopqrstuvwxyz");
			PrintMD5("abcdefghijklmnopqrstuvwxyz", md5);

			md5.Reset();
			md5.Update("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
			PrintMD5("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", md5);

			md5.Reset();
			md5.Update("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
			PrintMD5("12345678901234567890123456789012345678901234567890123456789012345678901234567890", md5);
		}
		else if(input == 'c')
		{
			char filename[50];
			cout << "�������ļ���" << endl;
			cin >> filename;

			md5.Reset();
			md5.Update(ifstream(filename));
			cout << "The MD5 value of file (\""  << filename << "\") is" << endl;
			cout << md5.Tostring() << endl;
		}
		else if(input == 'v')
		{
			char filename[50];
			char InputMD5[33];
			cout << "������������ļ���" << endl;
			cin >> filename;

			cout << "�����뱻���ļ���MD5ժҪ��" << endl;
			cin >> InputMD5;
			InputMD5[32] = '\0';

			cout << "�������MD5ժҪΪ��" << endl;
			cout << InputMD5 << endl;

			md5.Reset();
			md5.Update(ifstream(filename));
			string OutputMD5 = md5.Tostring();
			const char* pResult = OutputMD5.c_str();
			cout << "�������MD5ժҪΪ��" << endl;
			cout << pResult << endl;
			if(strcmp(InputMD5, pResult) == 0)
				cout << "�ļ�������" << endl;
			else
				cout << "�ļ����۸ģ�" << endl;
		}
		else if(input == 'f')
		{
			char filename[50];
			char MD5_filename[50];
			char record[33];

			cout << "������������ļ���" << endl;
			cin >> filename;
			cout << "������MD5�ļ���" << endl;
			cin >> MD5_filename;

			ifstream MD5_file(MD5_filename);
			MD5_file.getline(record, 32);
			MD5_file.getline(record, 33);
			record[32] = '\0';
			cout << "��ȡ����MD5�ļ�ժҪΪ��" << endl;
			cout << record << endl;

			md5.Reset();
			md5.Update(ifstream(filename));
			string OutputMD5 = md5.Tostring();
			const char* pResult = OutputMD5.c_str();
			cout << "�������MD5ժҪΪ��" << endl;
			cout << pResult << endl;
			if(strcmp(record, pResult) == 0)
				cout << "�ļ�������" << endl;
			else
				cout << "�ļ����۸ģ�" << endl;
		}
		else if(input == 'q')
			return 0;
	}
}
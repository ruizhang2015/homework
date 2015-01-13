#include "operation.h"
using namespace std;	
using namespace CryptoPP;	
/*dataowner::dataowner(int wordLen,int para_m, int wordNum)
{
	n=wordLen;
	m=para_m;
	l=wordNum;
}*/
void dataowner::read(string inFile)
{
	srand(time(NULL));
}
string dataowner::DEAEncryptString(const char *instr, const char *passPhrase)
{
	string outstr;
	DefaultEncryptorWithMAC encryptor(passPhrase, new HexEncoder(new StringSink(outstr)));
	encryptor.Put((byte *)instr, strlen(instr));
	encryptor.MessageEnd();

	return outstr;
}

string dataowner::DEADecryptString(const char *instr, const char *passPhrase)
{
	string outstr;

	HexDecoder decryptor(new DefaultDecryptorWithMAC(passPhrase, new StringSink(outstr)));
	decryptor.Put((byte *)instr, strlen(instr));
	decryptor.MessageEnd();

	return outstr;
}
void dataowner::generateS()
{
	DefaultAutoSeededRNG rng;
	for(int j=0;j<l;j++)
	{
		unsigned long r=0;
		for(int i=0;i<n-m;i++)
		{	
			r +=rng.GenerateBit();
			r <<1;
		}
		cout<<"next random S_i: "<<r<<endl;
		S.push_back(r);
	}
}
void dataowner::PRF()//pseudorandom function F();
{}
void dataowner::PRf()//pseudorandom function f();
{}

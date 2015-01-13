/*
 *An implementation of algorithm in paper: "practical techniques for searches on encrypted data" by Bin Yao
 * */
#include <string>
#include <vector>
#include <iostream>
#include "aes.h"
#include "hex.h"
#include "default.h"
#include "cryptlib.h"
#include "osrng.h"
#include "misc.h"
#include "smartptr.h"
using namespace std;
class dataowner {	
	
	public:
	//	dataowner(int,int,int);
		void read(string inFile);
		string DEAEncryptString(const char *plaintext, const char *passPhrase);//(D)eterministic (E)ncryption (A)lgorithm for E(W_i);
		string DEADecryptString(const char *ciphertext, const char *passPhrase);
		void generateS();
	
	protected:
	 	vector<unsigned long> W; //keywords;  
	 	vector<string> EW; //E(W_i);  
		void PRF();//pseudorandom function F();
		void PRf();//pseudorandom function f();
    		string k_p;//k'
    		string k_pp;//k"
		vector<unsigned long> k;//k_i
		vector<unsigned long> S;//S_i n-m bit;
		int n,m,l;
};


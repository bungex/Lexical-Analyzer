#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <algorithm>
#include "lex.h"
#include "lex.cpp"

using namespace std;


int main(int argc, char *argv[]){
	
	ifstream myFile;
	vector<string> args;
	vector<LexItem> alltoks;
	vector<string> sconst;
	vector<int> iconst;
	vector<float> rconst;
	vector<string> ident;
	string fileName;
	LexItem tok;
	int lineNumber = 0;

	if(argc > 0){
		for(int i = 1; i < argc; i++){
			args.push_back(argv[i]);
		}
	}
	
	int f_count = 0;
	for(string x : args){
		if(isalpha(x[0])){
			fileName = x;
			f_count++;
		}
		else if((x != "-v") && (x != "-iconst") && (x != "-rconst") && (x != "-sconst") && (x != "-ident")) {
			cout << "UNRECOGNIZED FLAG " << x << endl;
			exit(1);	
		}	
	}
	
	if(f_count > 1){
		cout << "ONLY ONE FILE NAME ALLOWED." << endl;
		exit(1);
	}
	else if(f_count == 0){
		cout << "NO SPECIFIED INPUT FILE NAME." << endl;
		exit(1);
	}

	myFile.open(fileName.c_str());
	if(!myFile.is_open()){
		cout << "CANNOT OPEN the File " << fileName << endl;
		exit(1);
	}
	bool v_flag = false;
	if(find(args.begin(), args.end(), "-v") != args.end()){
		v_flag = true;
	}
	
	int totalTok = 0;
	while((tok = getNextToken(myFile, lineNumber)) != DONE && tok != ERR ) {
		totalTok++;
		alltoks.push_back(tok);
		if(v_flag){
			cout << tok;
		}
		if(tok == SCONST){
			sconst.push_back(tok.GetLexeme());
		}
		if(tok == ICONST){
			iconst.push_back(stoi(tok.GetLexeme()));     //convert string to intger and add it to int vector
		}
		if(tok == RCONST){
			rconst.push_back(stof(tok.GetLexeme()));     //convert string to float and add it to float vector 
		}
		if(tok == IDENT){
			ident.push_back(tok.GetLexeme());
		}
	}
	if(tok == ERR){
		cout << "Error in line " << tok.GetLinenum() << " (" << tok.GetLexeme() << ")" << endl;
		exit(1);
	}
	
	if(lineNumber == 0){
		cout << "Lines: " << lineNumber << endl;
		exit(1);
	}
	
	cout << "Lines: " << lineNumber << endl;
	cout << "Tokens: " << totalTok << endl;
	
	if(find(args.begin(), args.end(), "-sconst") != args.end()){
		cout << "STRINGS:" << endl;
		sort(sconst.begin(),sconst.end());						// sort in alphabetical order

		for(string y : sconst){
			cout << "'" << y << "'" << endl;
		}
	}
	if(find(args.begin(), args.end(), "-iconst") != args.end()){
		
		cout << "INTEGERS:" << endl;
		sort(iconst.begin(), iconst.end());						// sort from lowest to higest
		// delete repeating element with help of unique fucntion
		iconst.erase(unique(iconst.begin(), iconst.end()),iconst.end());			
		
		for(int x : iconst){
			cout << x << endl;
		}
		
	}
	
	if(find(args.begin(), args.end(), "-rconst") != args.end()){
		
		cout << "REALS:" << endl;
		sort(rconst.begin(), rconst.end());						// sort from lowest to higest
		// delete repeating element with help of unique fucntion
		rconst.erase(unique(rconst.begin(), rconst.end()),rconst.end());
		for(float z : rconst){
			cout << z << endl;
		}	
		
	}
	
	if(find(args.begin(), args.end(), "-ident") != args.end()){
		cout << "IDENTIFIERS:" << endl;
		
		sort(ident.begin(), ident.end());
		ident.erase(remove(ident.begin(),ident.end(), "BEGIN"), ident.end());
		ident.erase(unique(ident.begin(), ident.end()), ident.end());
		

		
		for(int i = 0; i < ident.size(); i++){
			if(i == ident.size() -1 ){
				cout << ident[i];
			}
			else {
				cout <<ident[i] << ", ";
			}
		}
		cout << endl;
	}

	

	myFile.close();
	return 0;
}




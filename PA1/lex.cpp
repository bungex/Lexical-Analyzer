#include <string>
#include <iostream>
#include <map>
#include <fstream>
#include <algorithm>
#include "lex.h"

using namespace std;

map<string, Token> strTok = 
{
	{"WRITELN",	WRITELN}, {"PROGRAM", PROGRAM}, {"INTEGER", INTEGER}, {"REAL", REAL}, {"DO", DO},
	{"STRING", STRING}, {"IF", IF}, {"THEN", THEN}, {"END", END}, {"TO", TO}, {"FOR", FOR},  
	{"ELSE", ELSE}, {"DOWNTO", DOWNTO}, {"ICONST", ICONST}, {"SCONST", SCONST}, 
	{"RCONST", RCONST},	{"PLUS", PLUS}, {"MINUS", MINUS}, {"MULT", MULT}, {"DIV", DIV}, 
	{"ASSOP", ASSOP}, {"LPAREN",LPAREN}, {"RPAREN", RPAREN}, {"SEMICOL", SEMICOL}, 
	{"COMMA", COMMA}, {"EQUAL", EQUAL}, {"ERR", ERR}, {"GTHAN", GTHAN},{"LTHAN", LTHAN}, {"COLON", COLON}, {"DONE", DONE}, 
	{"IDENT", IDENT}, {"VAR", VAR}, {"BEGIN", BEGIN}
};
map<Token,string> tokStr = 
{
	{IDENT, "IDENT"}, {ICONST, "ICONST"}, {RCONST, "RCONST"}, {SCONST, "SCONST"}
};

map<string,Token> :: iterator it;
map<Token,string> :: iterator it2;


LexItem id_or_kw(const string& lexeme, int linenum){

	for(it = strTok.begin(); it != strTok.end(); it++){
		if(lexeme == it->first){
			return LexItem(it->second, lexeme , linenum);
		}		
	}
	return LexItem(IDENT, lexeme , linenum);		
}


ostream& operator<<(ostream& out, const LexItem& tok){
		
		
		if(tok.GetToken() == IDENT){
			out << tokStr[IDENT] << "(" << tok.GetLexeme() << ")" << endl;
		}
		else if(tok.GetToken() == ICONST){
			out << tokStr[ICONST] << "(" << tok.GetLexeme() << ")" << endl;
		}
		else if(tok.GetToken() == RCONST){
			out << tokStr[RCONST] << "(" << tok.GetLexeme() << ")" << endl;
		}
		else if(tok.GetToken() == SCONST){
			out << tokStr[SCONST] << "(" << tok.GetLexeme() << ")" << endl;
		}
		else{
			for(it = strTok.begin(); it != strTok.end(); it++){
				if(tok.GetToken() == it->second) {
					out << it->first << endl;
				}
			}	
		}
		

			
	return out;
}


LexItem getNextToken(istream& in, int& linenum){
	
	enum TokState { START, INID, INSTRING, ININT, INREAL, INCOMMENT} 
	lexstate = START;
	string lexeme;
	char ch;
	while(in.get(ch)) {
		switch( lexstate ) {
			case START:
				if( ch == '\n' ){
					linenum++;
					break;	
				}
				if(isspace(ch))
					break;					
				lexeme = "";
				lexeme += toupper(ch);
				if(isalpha(ch) || ch == '_') {
					lexstate = INID;
					continue;
				}
				else if(ch == '\''){
					lexstate = INSTRING;
					continue;
				}
				else if(isdigit(ch)){
					lexstate = ININT;
					continue;
				}
				else if(ch == '.'){
					lexstate = INREAL;
					continue;
				}
				else if(ch == '(' && in.peek() == '*'){
					lexstate = INCOMMENT;
					continue;
				}
				else{
					if (ch == ')'){
						return LexItem(RPAREN, lexeme, linenum);
						break;
					}
					else if (ch == '('){
						return LexItem(LPAREN, lexeme, linenum);
					}
					else if (ch == '+'){
						return LexItem(PLUS, lexeme, linenum);
						break;
					}
					else if (ch == '-'){
						return LexItem(MINUS, lexeme, linenum);
						break;
					}
					else if(ch == '*'){
						return LexItem(MULT, lexeme, linenum);
						break;
					}
					else if(ch == '/'){
						return LexItem(DIV, lexeme, linenum);
						break;
					}
					else if(ch == ':'){
						if(in.peek() == '='){
							in.ignore();
							return LexItem(ASSOP, lexeme, linenum);
							break;
						}
						return LexItem(COLON, lexeme, linenum);
						break;
					}
					else if(ch == '='){
						return LexItem(EQUAL, lexeme, linenum);
						break;
					}
					else if(ch == '>'){
						return LexItem(GTHAN, lexeme, linenum);
						break;
					}
					else if(ch == '<'){
						return LexItem(LTHAN, lexeme, linenum);
						break;
					}
					else if (ch == ';'){
						return LexItem(SEMICOL, lexeme, linenum);
						break;
					}
					else if(ch == ','){
						return LexItem(COMMA, lexeme, linenum);
						break;
					}
					else{
						linenum++;
						return LexItem(ERR, lexeme, linenum);
					}	
				}
				break;
			
			case INID:
				if(isalpha(ch) || isdigit(ch) || ch == '_'){
					lexeme += toupper(ch);
				}
				else{
					in.putback(ch);
					lexstate = START;
					return id_or_kw(lexeme, linenum);
				}		

				break;
			case INSTRING:
				if(ch == '\''){
					lexeme += ch;
					lexstate = START;
					return LexItem(SCONST, lexeme.substr(1,lexeme.size()-2), linenum);
				}
				else if(in.peek() == -1 || in.peek() == '\n'){
					linenum++;
					lexeme += ch;
					return LexItem(ERR, lexeme, linenum);
				}
				else if(ch == '\"'){
					linenum++;
					lexeme += ch;
					return  LexItem(ERR,lexeme,linenum);
				}
				else{
					lexeme += ch;
					break;
				}
			case ININT:				
				if(ch == '.'){
					if(in.peek() == -1){
						return LexItem(ERR, lexeme, linenum);
					}
					else{
						lexeme += ch;
						lexstate = INREAL;
						continue;	
					}
					
				}
				else if(!isdigit(ch)){
					in.putback(ch);
					lexstate = START;
					return LexItem(ICONST,lexeme, linenum);							
				}
				else{
					lexeme += ch;
					if(isspace(in.peek())){
						lexstate = START;
						return LexItem(ICONST, lexeme, linenum);
					}
					else if(in.peek() == '\n'){
						lexstate = START;
						return LexItem(ICONST, lexeme, linenum);
					}
				}
				break;
			case INREAL:
				if(in.peek() == -1){
					linenum++;
					return LexItem(ERR, lexeme, linenum);
				}
				if(isdigit(ch)){
					lexeme += ch;
					if(in.peek() == '.'){
						linenum++;
						in.get(ch);
						lexeme += ch;
						return LexItem(ERR, lexeme, linenum);
					}
					else if(isspace(in.peek())){
						lexstate = START;
						return LexItem(RCONST, lexeme, linenum);
					}
				}
				else if(!isdigit(ch)){
					in.putback(ch);
					lexstate = START;
					return LexItem(RCONST, lexeme, linenum);
				}
				else{
					return LexItem(RCONST, lexeme, linenum);
				}

				break;
			case INCOMMENT:
				if(ch == '\n'){
					linenum++;
				}

				else if(ch == ')'){
					if(lexeme.back() == '*'){
						lexeme = "";
						lexstate = START;
						continue;
					}
				}
				else if(in.peek() == -1){
					linenum++;
					lexeme = "Missing a comment end delimiters '*)' at line ";
					return LexItem(ERR, lexeme, linenum);
				}
				else{
					lexeme += ch;
				}
				break;
		}
	}
	if(in.eof()){
		return LexItem(DONE, "", linenum);
	}
	return LexItem(ERR, "", linenum);
}




#include "params.h"
#include "pictures.h"
#include "outputs.h"
#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <vector>
#define PI 3.141592653589793
using namespace std;

matrix mtrx = matrix();
int depth;
string linein;
stringstream ss;
kvlist vars = kvlist();
vector<string>::iterator iter;
vector<string>::iterator iterend;
vector<string>::reverse_iterator riter;

void process ( string, double[]);
void parse ();
void Tokenize(string& str, vector<string>& tokens, const string& delimiters = " "){
	string alpha = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string::size_type lastPos = str.find_first_not_of(delimiters, 0);
	string::size_type pos = str.find_first_of(delimiters, lastPos);
	while (string::npos != pos || string::npos != lastPos){
		string tmp = str.substr(lastPos, pos - lastPos);
		if(tmp.find("(") != string::npos){
			string::size_type x = tmp.find('(');
			if(x!=0)
				tokens.push_back(tmp.substr(0,x));
			tokens.push_back(tmp.substr(x));
		}else if(tmp.find(")") != string::npos){
			string::size_type x = tmp.find(')');
			string::size_type y = tmp.find_first_of(alpha,x);
			if(x != string::npos){
				tokens.push_back(tmp.substr(0,y-1));
				if(y != string::npos)
					tokens.push_back(tmp.substr(y));
			}
		}else
			tokens.push_back(tmp);
		lastPos = str.find_first_not_of(delimiters, pos);
		pos = str.find_first_of(delimiters, lastPos);
	}
}
double stoi(string s){
	while(s.find(')') != string::npos)
		s.erase(s.find(')'));
	if (vars.exists(s))
		return vars.getval(s);
	else
		return atoi(s.c_str());
}
double eval(){
	string op = *iter++;
	if (op == "(")
		op = *iter++;
	else
		if(op.find('(') != string::npos)
			op.erase(op.find('('),1);
	string a, b;
	double x, y;
	if (op == "+"){
		a = *iter++;
		b = *iter++;
		x = stoi(a);
		y = stoi(b);
		return x + y;
	} else if (op == "-"){
		a = *iter++;
		b = *iter++;
		x = stoi(a);
		y = stoi(b);
		return x - y;
	} else if (op == "*"){
		a = *iter++;
		b = *iter++;
		x = stoi(a);
		y = stoi(b);
		return x * y;
	} else if (op == "/"){
		a = *iter++;
		b = *iter++;
		x = stoi(a);
		y = stoi(b);
		return x / y;
	} else if (op == "sin"){
		a = *iter++;
		x = stoi(a);
		y = x * PI / 180;
		return sin(y);
	} else if (op == "cos"){
		a = *iter++;
		x = stoi(a);
		y = x * PI / 180;
		return cos(y);
	} else {
		return stoi(op);
	}
}
double evalr(){
	double x,y;
	string second = *riter++;
	iterend--;
	// cout<<second<<"<-2\n";
	while (second[0] == ')' || second[0] == ' '){
		second = *riter++ + second;
		iterend--;
	}
	// cout<<second<<"<-2\n";
	string first = *riter;
	iterend--;
		// cout<<second<<"<-2\n";
		// cout<<first<<"<-1\n";
	if(first.find("sin") != string::npos || first.find("cos") != string::npos){
		if(first.find('(') != string::npos)
			first.erase(first.find('('),1);
		if (first == "sin"){
			x = stoi(second);
			y = x * PI / 180;
			riter++;
			iterend--;
			if (*riter == "("){
				riter++;
				iterend--;
			}
			return sin(y);
		} else if (first == "cos"){
			x = stoi(second);
			y = x * PI / 180;
			riter++;
			iterend--;
			if (*riter == "("){
				riter++;
				iterend--;
			}
			return cos(y);
		}
	}
	string op = *(riter + 1);	
	if(op.find('(') != string::npos)
		op.erase(op.find('('),1);
	if (op == "+"){
		x = stoi(first);
		y = stoi(second);
		riter++;
		iterend--;
		riter++;
		iterend--;
		if (*(riter+1) == "(")
			riter++;
			iterend--;
		return x + y;
	} else if (op == "-"){
		x = stoi(first);
		y = stoi(second);
		riter++;
		iterend--;
		riter++;
		iterend--;
		if (*(riter+1) == "(")
			riter++;
			iterend--;
		return x - y;
	} else if (op == "*"){
		x = stoi(first);
		y = stoi(second);
		riter++;
		iterend--;
		if (*(riter+1) == "("){
			riter++;
			iterend--;
		}
		return x * y;
	} else if (op == "/"){
		x = stoi(first);
		y = stoi(second);
		riter++;
		iterend--;
		if (*(riter+1) == "("){
			riter++;
			iterend--;
		}
		return x / y;
	} else {
		return stoi(second);
	}
}
void group(){
	vector<string> vect;
	vector<vector<string> > varr;
	vector<vector<string> >::iterator vviter;
	vector<string>::iterator viter;
	matrix mat = mtrx;
	string s;
	int number = 0;
	for (iter; iter < iterend; iter++){
		s = *iter;
		if(s == "(")
			s = s + *++iter;
		if(s.find("(") != string::npos){
			depth++;
		}	
		if(s.find(")") != string::npos){
			while(s.find(")") != string::npos){
				number++;
				s = s.substr(0,s.find_last_of(')'));
				depth--;
			}
			while(number > 0){
				s += ")";
				number--;
			}	 
		}
		if(depth < 2){
			if(depth == 0)
				vect.push_back(s.substr(0,s.find_last_of(')')));
			else
				vect.push_back(s);
			varr.push_back(vect);
			vect = vector<string>();
		}else
			vect.push_back(s);
	}
	vviter = varr.begin();
	for (vviter; vviter < varr.end(); vviter++){
			vect = *vviter;
			mtrx = mat;
			iter = vect.begin();
			iterend = vect.end();
			riter = vect.rbegin();
			parse();
	}
	mtrx.clear();
}
void parsefor(){
	vector<string> vect;
	vector<vector<string> > varr;
	vector<vector<string> >::iterator vviter;
	vector<string>::iterator viter;
	string i = *iter++;
	string s;
	int number =0 ;
	double l = eval();
	double u = eval();	
	vars.add(i, l);
	for (iter; iter < iterend; iter++){
		s = *iter;
		if(s == "(")
			s = s + *++iter;
		if(s.find("(") != string::npos){
			depth++;
		}	
		if(s.find(")") != string::npos){
			while(s.find(")") != string::npos){
				number++;
				s = s.substr(0,s.find_last_of(')'));
				depth--;
			}
			while(number > 0){
				s += ")";
				number--;
			}	 
		}
		if(depth < 2){
			if(depth == 0)
				vect.push_back(s.substr(0,s.find_last_of(')')));
			else
				vect.push_back(s);
			varr.push_back(vect);
			vect = vector<string>();
		}else
			vect.push_back(s);
	}
	for(int k = l; k < u + 1; k++){
		if(vars.getval(i) > u)
			break;
		vars.add(i, k);
		vviter = varr.begin();
		for (vviter; vviter < varr.end(); vviter++){
			vect = *vviter;	
			iter = vect.begin();
			iterend = vect.end();
			riter = vect.rbegin();
			parse();
		}
	}
}
void parse(){
	// cout<<"startparse\n";
	string func =  *iter++;
	if (func == "(")
		func =  *iter++;
	else
	 func.erase(0,1);
	if(func == "translate"){
		double x[2];
		x[1] = evalr();
		// cout<<x[1]<<"t1\n";
		x[0] = evalr();
		// cout<<x[0]<<"t2\n";
		double tMat[3][3] = {{1, 0, x[0]}, {0, 1, x[1]}, {0, 0, 1}};
		mtrx.combine(tMat);
		parse();
	}else if(func == "rotate"){
		double x = evalr();
		double rad = x * PI / 180;
		double rMat[3][3] = {{cos(rad), -sin(rad), 0}, {sin(rad), cos(rad), 0}, {0, 0, 1}};
		mtrx.combine(rMat);	
		parse();
	}else if(func == "scale"){
		double x = evalr();
		double sMat[3][3] = {{x, 0, 0}, {0, x, 0}, {0, 0, 1}};
		mtrx.combine(sMat);	
		parse();
	}else if(func == "group"){
		depth = 1;
		group();
	}else if(func == ":="){
		string key = *iter++;
		double value = eval();
		vars.add(key, value);
	}else if(func == "for"){
		depth = 1;
		parsefor();
	}else{
		string s;
		double x[4];
		for(int i = 0; i < 4; i++){
			if (iter < iterend){
				x[i] = eval();
			}
		}
		process(func, x);
	}
}
void scan(){
	vector<string> tokens;
	Tokenize(linein,tokens," ");
	iter = tokens.begin();
	iterend = tokens.end();
	// for(iter; iter<iterend;iter++)
		// cout<<" ^"<<*iter;
	riter = tokens.rbegin();
	
	
		// cout<<" "<<*iter;
		// cout<<" "<<*(iterend-1);
		// cout<<" "<<*riter;
		// riter++;
		// iterend--;
		// cout<<" "<<*iter;
		// cout<<" "<<*(iterend-1);
		// cout<<" "<<*riter;
		// cout<<"\n";
		
	parse();
}
void process(string op, double x[4]){
	if (op == "line"){
			line oper = line (x[0], x[1], x[2], x[3]);
			output(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "rect"){
			rect oper = rect (x[0], x[1], x[2], x[3]);
			output(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "filledrect"){
			rect oper = rect (x[0], x[1], x[2], x[3]);
			outputf(mtrx, oper, &ss);
			mtrx.clear();	
		}else if (op == "tri"){
			tri oper = tri (x[0], x[1], x[2]);
			output(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "filledtri"){
			tri oper = tri (x[0], x[1], x[2]);
			outputf(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "square"){
			square oper = square (x[0], x[1], x[2]);
			output(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "filledsquare"){
			square oper = square (x[0], x[1], x[2]);
			outputf(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "penta"){
			penta oper = penta (x[0], x[1], x[2]);
			output(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "filledpenta"){
			penta oper = penta (x[0], x[1], x[2]);
			outputf(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "hexa"){
			hexa oper = hexa (x[0], x[1], x[2]);
			output(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "filledhexa"){
			hexa oper = hexa (x[0], x[1], x[2]);
			outputf(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "ngon"){
			ngon oper = ngon (x[0], x[1], x[2], x[3]);
			output(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "filledngon"){
			ngon oper = ngon (x[0], x[1], x[2], x[3]);
			outputf(mtrx, oper, &ss);
			mtrx.clear();
		}else if (op == "color"){
			color oper = color (x[0], x[1], x[2]);
			oper.output(&ss);
		}else if (op == "linewidth"){
			width oper = width (x[0]);
			oper.output(&ss);
		}
}

// void get(){
	// int lparen=0, rparen=0;
	// for(int i = 0; i < linein.size()-1; i++){
			// if(linein[i] == '('){
				// lparen++;
				// cout<<lparen<<"l\n";}
			// else if(linein[i] == ')'){
				// rparen++;
				// cout<<rparen<<"r\n";}
	// }
	// cout<<"blah";
	// if(rparen != lparen){
		// string s;
		// getline(cin, s, '\n');
		// linein+=s;
		// get();
	// }
	
// }
int main(){
	ss.str("");
	ss << "%!PS-Adobe-3.1\n";	
	while(getline(cin, linein, '\n')){
		scan();
		// cout<<"\n";
	}
	cout << ss.str() << "showpage\n";
	return 0;
}
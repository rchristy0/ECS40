#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "stack.h"
using namespace std;

int main(){
	stack lst;
	stack stk;
	string data[50];
	stringstream ss;
	char tmp[15];
	string temp;
	
	lst.size = 0;
	stk.size = 0;
	int a, b, i, k, val;
	
	i = 0;
	while (scanf("%s", tmp) != EOF){
		data[i] = tmp;
		i++;
	}
	
	k = 0;
	while(data[k] != ""){
		string temp = data[k];
		if (temp == "+"){
			a = atoi(stk.pop().c_str());
			b = atoi(stk.pop().c_str());
			val = a + b;
			ss.str("");
			ss << val;
			stk.push(ss.str());
		}else if (temp == "-"){
			a = atoi(stk.pop().c_str());
			b = atoi(stk.pop().c_str());
			val = b - a;
			ss.str("");
			ss << val;
			stk.push(ss.str());
		}else	if (temp == "*"){
			a = atoi(stk.pop().c_str());
			b = atoi(stk.pop().c_str());
			val = a * b;
			ss.str("");
			ss << val;
			stk.push(ss.str());
		}else if (temp == "/"){
			a = atoi(stk.pop().c_str());
			b = atoi(stk.pop().c_str());
			val = b / a;\
			ss.str("");
			ss << val;
			stk.push(ss.str());
		}else{
			stk.push(temp);
		}
		k++;
	}
	// cout << atoi(stk.pop().c_str()) << "\n";
	cout << stk.pop() << "\n";
	return 0;	
}
#include "params.h"
#include "pictures.h"
#include "outputs.h"
#include <sstream>
#include <iostream>
#include <stdio.h>
using namespace std;
matrix mtrx = matrix();
string linein;
stringstream ss;

void process ( string, double[]);
void scan(){
	while(getline(cin, linein, ')')){
		char func[20] = {'\0'};
		double x[4] = {0};
		sscanf(linein.c_str(), "\n ( %[a-z-A-Z] %lf %lf %lf %lf ", func, &x[0], &x[1], &x[2], &x[3]);
		process(func, x);
	}
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
		}else if (op == "translate"){
			matrix tmp;
			double transMat[3][3] = {{1, 0, x[0]}, {0, 1, x[1]}, {0, 0, 1}};
			int i,j;
			for(i = 0; i < 3; i++){
				for(j = 0; j < 3; j++){
					tmp.data[i][j] = transMat[i][j];
				}
			}
			mtrx.combine(tmp);
			scan();
		}else if (op == "rotate"){
			double rad = x[0] * PI / 180;
			matrix tmp;
			double rotMat[3][3] = {{cos(rad), -sin(rad), 0}, {sin(rad), cos(rad), 0}, {0, 0, 1}};
			int i,j;
			for(i = 0; i < 3; i++){
				for(j = 0; j < 3; j++){
					tmp.data[i][j] = rotMat[i][j];
				}
			}
			mtrx.combine(tmp);	
			scan();
		}else if (op == "color"){
			color oper = color (x[0], x[1], x[2]);
			oper.output(&ss);
		}else if (op == "linewidth"){
			width oper = width (x[0]);
			oper.output(&ss);
		}
}

int main(){
	ss.str("");
	ss << "%!PS-Adobe-3.1\n";	
	scan();
	cout << ss.str() << "showpage\n";
	return 0;
}
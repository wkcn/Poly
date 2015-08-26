#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cstdio>
using namespace std;

typedef pair<int,int> term;
typedef vector<term> poly;

class Reader{
	private:
		string text;
		int p;
	public:
		Reader(){
			p = 0;
		}
		void Write(const string &s){
			text = s;
			p = 0;
		}
		bool empty(){
			return p >= text.size();
		}
		int ReadNum(){
			int res = 0;
			bool minus = false;
			for(;p<text.size();++p){
				if (text[p] == ' ')continue;
				if (text[p] == '-'){
					minus = true;
					++p;
					break;
				}else{
					break;
				}
			}
			for(;p<text.size();++p){
				if (text[p] == ' ')continue;
				if (text[p] >= '0' && text[p] <= '9'){
					res += res *10 + text[p] - '0';
				}else{
					throw "ÊäÈëÊý×Ö´íÎó";
				}
			}
			if(minus)res = -res;
			return res;
		}
		char ReadSym(){
			for(;p<text.size();++p){
				if (text[p] == ' ')continue;
				return text[p];
			}
			return 0;
		}
		int ReadExp(){
			//x^i
			for(;p<text.size();++p){
				if (text[p] == ' ')continue;
			}
			if (text[p]!='x')throw "ÊäÈëx´íÎó";
			++p;
			for(;p<text.size();++p){
				if (text[p] == ' ')continue;
			}
			if (text[p] == '^'){
				++p;
				return ReadNum();
			}else{
				return 1;
			}
		} 
		poly Get(){
			map<int,int> datas;//exp,num
			
		}
};

int main(){
	
	return 0;
}

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

// ABC247 https://atcoder.jp/contests/abc247

string func(int N)
{
	if(N==1) return "1";
	
	string str;
	str = func(N-1) + " " + to_string(N) + " " + func(N-1);
	return str;
}

int main(void)
{
	int N;
	cin >> N;

	string str;
	str = func(N);
	cout << str << endl;

	return 0;
}

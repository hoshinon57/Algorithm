#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC302 https://atcoder.jp/contests/abc302
// next_permutation()を用いて全探索する。
// next_permutation()はvector<string>型も扱えるので、入力Sをソートして使う。

bool solve(void)
{
	int i, j;
	int N, M;
	cin >> N >> M;
	vector<string> s(N);
	for(i = 0; i < N; i++)
	{
		cin >> s[i];
	}
	sort(s.begin(), s.end());

	auto check = [&](string &str1, string &str2) -> bool
	{
		int count = 0;
		for(j = 0; j < M; j++)
		{
			if(str1[j] != str2[j]) count++;
		}
		if(count == 1) return true;
		else return false;
	};

	do
	{
		bool flag = true;
		for(i = 0; i < N-1; i++)
		{
			if(!check(s[i], s[i+1])) flag = false;
		}
		if(flag) return true;
	} while (next_permutation(s.begin(), s.end()));

	return false;
}

int main(void)
{
	YesNo(solve());

	return 0;
}

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

// ABC324 https://atcoder.jp/contests/abc324

// 入力文字列Sごとに判定する。
// 長さが2以上違っていたらそもそもNG.
// それ以外の場合、S,Tを比較していき、1回までは不一致を許可する。

int N;
string T;

bool check(string &s)
{
	int i;
	int tlen = (int)T.size();
	int slen = (int)s.size();
	if(abs(tlen-slen)>=2) return false;
	int cnt = 0;
	if(tlen == slen)
	{
		for(i = 0; i < tlen; i++)
		{
			if(T[i] == s[i]) continue;
			if(++cnt >= 2) return false;
		}
	}
	else if(tlen > slen)
	{
		for(i = 0; i < slen; i++)
		{
			if(T[i+cnt] == s[i]) continue;
			if(++cnt >= 2) return false;
			i--;
		}
	}
	else  // tlen < slen
	{
		for(i = 0; i < tlen; i++)
		{
			if(T[i] == s[i+cnt]) continue;
			if(++cnt >= 2) return false;
			i--;
		}
	}
	return true;
}

int main(void)
{
	int i;
	cin >> N >> T;
	vector<int> ans;
	for(i = 0; i < N; i++)
	{
		string s; cin >> s;
		if(check(s)) ans.push_back(i+1);
	}

	cout << ans.size() << endl;
	for(auto &e : ans) cout << e << " ";
	cout << endl;

	return 0;
}

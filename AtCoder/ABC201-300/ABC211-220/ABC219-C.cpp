#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC219 https://atcoder.jp/contests/abc219

/*
 * 公式解説にある2つの方針
 *   比較関数を実装する
 *   別の数列に置き換えてからソートを行う
 * それぞれを実装した。
 */

char conv[256] = {0};

bool myCompare(const string &a, const string &b)
{
	int len = (int)min(a.size(), b.size());
	for(int i = 0; i < len; i++)
	{
		if(a[i] == b[i]) continue;
		return conv[(int)a[i]] < conv[(int)b[i]];
	}

	return a.size() < b.size();	
}

int main(void)
{
	int i;
	string x; cin >> x;
	int N; cin >> N;
	i = 0;
	for(char c = 'a'; c <= 'z'; c++, i++)
	{
		conv[(int)x[i]] = c;
	}

#if 1  // 公式解説の方針1「比較関数を実装する」
	vector<string> s(N); for(i = 0; i < N; i++) {cin >> s[i];}
	sort(s.begin(), s.end(), myCompare);
	for(auto &e : s) cout << e << endl;
#else  // 公式解説の方針2「別の数列に置き換えてからソートを行う」
	vector<pair<string, int>> p(N);  // {変換後の文字列, Sのidx}
	vector<string> org(N);
	for(i = 0; i < N; i++)
	{
		cin >> p[i].first;
		org[i] = p[i].first;
		p[i].second = i;
		for(auto &e : p[i].first)
		{
			e = conv[(int)e];
		}
	}
	sort(p.begin(), p.end());
	
	for(i = 0; i < N; i++)
	{
		cout << org[p[i].second] << endl;
	}
#endif

	return 0;
}

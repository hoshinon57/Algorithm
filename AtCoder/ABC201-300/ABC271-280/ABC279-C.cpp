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

// ABC279 https://atcoder.jp/contests/abc279

/*
 * 公式解説での方法。
 * 図形S,Tそれぞれ列ごとに文字列として管理する。
 * その文字列をソートしたときに、一致すればYes, 不一致ならばNo.
 */

bool solve(void)
{
	int i, j;
	int H, W; cin >> H >> W;
	vector<string> S(W), T(W);
	for(i = 0; i < H; i++)
	{
		string s; cin >> s;
		for(j = 0; j < W; j++)
		{
			S[j] += s[j];
		}
	}
	for(i = 0; i < H; i++)
	{
		string s; cin >> s;
		for(j = 0; j < W; j++)
		{
			T[j] += s[j];
		}
	}
	sort(S.begin(), S.end());
	sort(T.begin(), T.end());

	return S == T;
}

int main(void)
{
	YesNo(solve());

	return 0;
}

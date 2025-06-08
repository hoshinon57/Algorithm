#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC409 https://atcoder.jp/contests/abc409

/*
 * ちょっと実装が大変だった系。
 * 「辞書順最小は前から貪欲」の典型を元に解く。
 * 
 * 先頭から見ていって、初めてs[i]>s[i+1]となる位置があったときに、
 * l=iとするのがまずよさそう、となる。
 * s[i+1]を手前に持ってくることになる。
 * 
 * では操作にて末尾に移動するs[i]は、どこまで移動させればよいか。
 * これも典型に従って、i<jにてs[i]<s[j]となる部分が見つかったところ、が良い。
 * 
 * 最初に入力SをソートしたものとSが一致するなら、そのままS出力して終了とすると、実装が楽だった。
 * 
 * ※後にsnuke氏の実装を見たら簡潔で感動した。
 *   https://atcoder.jp/contests/abc409/submissions/66588557
**/

void solve(void)
{
	ll i, j;
	ll N; cin >> N;
	string s; cin >> s;
	string sorts = s;
	sort(sorts.begin(), sorts.end());
	if(s == sorts)
	{
		cout << s << endl;
		return;
	}

	string ans;
	for(i = 0; i < N-1; i++)  // s[i]>s[i+1]を探す
	{
		if(s[i] > s[i+1])
		{
			// l=iとし、s[i+1]を前に持ってくる
			// s[i]をどこに持っていくかは、i<jにてs[i]<s[j]となるjの手前
			bool flg = false;
			for(j = i+1; j < N; j++)
			{
				if(s[i] < s[j] && !flg)
				{
					ans.push_back(s[i]);
					flg = true;
				}
				ans.push_back(s[j]);
			}
			if(!flg) ans.push_back(s[i]);  // 最後までs[i]<s[j]が無かった場合
			break;
		}
		else
		{
			ans.push_back(s[i]);
		}
	}
	cout << ans << endl;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		solve();
	}
	return 0;
}

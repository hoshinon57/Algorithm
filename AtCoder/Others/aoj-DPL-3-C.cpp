#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <stack>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_3_C

/*
 * ヒストグラム中の最大正方形問題。
 * 考え方は以下が分かりやすかった。
 *   http://algorithms.blog55.fc2.com/blog-entry-132.html
 * 
 * スタックに {y座標:h[i], x座標} の形で、y座標について単調増加になるよう持たせていく。
 * 今見ているh[i]をスタックに入れようとしたとき、単調増加を満たさないのであれば取り出して面積を計算する。
 * 
 * スタックに登録するとき、取り出した分だけx座標を手前に動かす必要がある点に注意。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> h(N); for(i = 0; i < N; i++) {cin >> h[i];}
	h.push_back(0);  // 末尾番兵

	// [実装方針]
	// stack内は単調増加、先頭から見ていく、stackには高さに加えx座標の情報が必要

	stack<pair<ll,ll>> st;
	st.push({0, -1});  // 先頭番兵
	ll ans = 0;
	for(i = 0; i < (int)h.size(); i++)
	{
		int x = i;
		while(st.top().first > h[i])  // h[i]を入れたときに単調増加を満たすようにするまで
		{
			auto [hh,xx] = st.top();
			st.pop();
			chmax(ans, (i-xx)*hh);
			x = xx;
		}
		st.push({h[i], x});
	}
	cout << ans << endl;

	return 0;
}

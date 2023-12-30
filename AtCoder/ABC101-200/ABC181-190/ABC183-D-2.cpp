#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC183 https://atcoder.jp/contests/abc183

// イベントソートで、「同じ時刻にイベントがあっても適切に処理できる」ような実装の練習。
// 
// ・イベント種類をキューにいれ、キューを1つ1つ処理しても矛盾しないような種類の値を設定する
// ・同じ時刻なら変化量が少ない方が優先されるので、キューを1つ1つ処理しても問題ない
// などの方法があるが、今回は
// ・時刻が前回から変化したタイミングで、最大値をチェックする
//   そのためには末尾に番兵を入れておき、最後のイベントも処理されるようにする
// という実装とした。
// 他の問題でも汎用的に使えると考える。

bool solve(void)
{
	int i;
	ll N, W; cin >> N >> W;
	using pll = pair<ll,ll>;  // {時刻,変化量}
	priority_queue<pll, vector<pll>, greater<pll>> que;
	for(i = 0; i < N; i++)
	{
		ll s, t, p; cin >> s >> t >> p;
		que.push({s, p});
		que.push({t, -p});
	}
	que.push({INF32, 0});  // 番兵

	ll mx = 0;
	ll crrT = -1, crrVal = 0;
	while(!que.empty())
	{
		auto [t,v] = que.top();
		que.pop();
		if(crrT != t)  // 時刻が変化したときに最大値を更新
		{
			chmax(mx, crrVal);
		}
		crrT = t;
		crrVal += v;
	}

	return mx <= W;
}

int main(void)
{
	YesNo(solve());

	return 0;
}

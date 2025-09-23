#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC424 https://atcoder.jp/contests/abc424

/*
 * コンテスト時に解けず、解説を見た。二分探索で解く。
 * 以下が近かったかな。
 *   https://qiita.com/Shizen_Tota/items/4e9109d76d119f2fbc36
 * 
 * 操作を行っていって、全ての棒が長さx以下になったとき、操作回数はK回以上か？
 * の判定問題を考えると、true,true,...,true,false,false,... と切り替わる。
 * 長さをdoubleで管理しているので、回数決め打ちでtrue,falseの境界を求める。
 * 
 * 境界のfalse側を使ってそこまでの操作を「いわゆるワープ」させ、
 * ちょうどK回になるまで map[長さ]=個数 のmapで求め、
 * X番目を求める。
 * 詳細は実装を参照。
 * 
 * ユーザー解説にある、いわゆるシミュレーション的な解法は、
 *   https://atcoder.jp/contests/abc424/editorial/13905
 * こちらの記述がしっくり来たかな。
 *   https://merom686.hatenablog.com/entry/2025/09/21/014348
**/

void solve(void)
{
	ll i;
	ll N, K, X; cin >> N >> K >> X;
	vector<double> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	map<double,ll> m;
	// 全て長さx以下になるまでやったときの操作回数を返す
	// flg=trueなら結果をmに反映
	auto chk = [&](double x, bool flg=false) -> ll
	{
		ll cnt = 0;
		for(auto &e : a)
		{
			double t = e;
			ll now = 1;
			while(t > x)
			{
				t /= 2.0;
				cnt += now;
				now *= 2;
			}
			if(flg) m[t] += now;
		}
		return cnt;
	};
	double ok = 0.0;
	double ng = INF32;
	for(ll _ = 0; _ < 70; _++)
	{
		double mid = (ok+ng)/2;
		if(chk(mid) >= K) ok = mid;
		else ng = mid;
	}

	K -= chk(ng, true);

	// K回になるまで操作
	while(K > 0)
	{
		auto itr = prev(m.end());
		auto [t,now] = *itr;
		ll c = min(K, now);  // 操作回数
		K -= c;
		itr->second -= c;
		if(itr->second == 0) m.erase(itr);
		m[t/2] += c*2;
	}
	
	// X番目を取得
	while(true)
	{
		auto itr = prev(m.end());
		auto [t,now] = *itr;
		X -= now;
		if(X <= 0)
		{
			cout << std::fixed << std::setprecision(20);
			cout << t << endl;
			return;
		}
		m.erase(itr);
	}
}

int main(void)
{
	int t; cin >> t;
	while(t > 0)
	{
		t--;
		solve();
	}

	return 0;
}

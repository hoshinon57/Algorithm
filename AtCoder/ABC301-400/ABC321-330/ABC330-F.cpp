#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC330 https://atcoder.jp/contests/abc330

/*
 * 二分探索で解く。
 * 
 * 「長さLの正方形で囲めるか？」を考えたときに、
 * Lで囲めるならL+1でも囲める。またLで囲めないならL-1でも囲めない。
 * よってLを増やしていったときに、false,false,...,false,true,true,... とどこかで切り替わる。
 * この切り替わり部分を二分探索で求める。
 * 
 * 上記の判定問題は、長さLの正方形で囲むときの操作回数を求め、それがK以下かどうかで判定する。
 * x,y独立して考えられるので、それぞれの操作回数を求め、その和となる。
 * 
 * 各xの値を正方形の左辺に設定し、xの左側とx+Lの右側にある頂点から、操作回数を求められる。
 * 自分はここで累積和を用いた。
 * x+Lの右側について、Σ(a[k]-(a[i]+L)) (k=idx～N-1) なので、範囲は[idx,N).
 * (a_sum[N]-a_sum[idx]) - (a[i]+L)*(N-idx) で求められる。
 * xの左側も同様。
 * 
 * 各xの値を正方形の右辺に設定するケースも考慮必要で、これはxを-1倍したものについて同じ判定をすればよい。
 * 
 * 座標の最大値をMとすると、判定問題にO(N), 二分探索でO(logM)なので全体でO(NlogM)となる。
 * 最初にソートO(NlogN)が必要なので、正確にはO(N*(logN+logM)).
 * 
 * [ACまでの思考の流れ]
 * ・最小値を求める、また長さLがOKならL+1もOK, などを踏まえると二分探索っぽい。
 * ・判定問題は、ある点の値を左辺にしたときの操作回数を、全ての点について計算すれば解けそう。
 */

int main(void)
{
	int i;
	ll N, K;  cin >> N >> K;
	vector<ll> x(N), x_inv(N), y(N), y_inv(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
		x_inv[i] = -x[i];
		y_inv[i] = -y[i];
	}
	sort(x.begin(), x.end());
	sort(x_inv.begin(), x_inv.end());
	sort(y.begin(), y.end());
	sort(y_inv.begin(), y_inv.end());

	vector<ll> x_sum(N+1), x_inv_sum(N+1), y_sum(N+1), y_inv_sum(N+1);
	for(i = 0; i < N; i++)
	{
		x_sum[i+1] = x_sum[i] + x[i];
		x_inv_sum[i+1] = x_inv_sum[i] + x_inv[i];
		y_sum[i+1] = y_sum[i] + y[i];
		y_inv_sum[i+1] = y_inv_sum[i] + y_inv[i];
	}

	// 各a[]の値を正方形(長さlen)の左辺側にして、操作回数を計算する
	// その最小値を返す
	auto cal = [&](vector<ll> &a, vector<ll> &a_sum, ll len)
	{
		ll ret = INF64;
		for(i = 0; i < N; i++)  // a[i]を左端とする
		{
			// [idx,N) 正方形の右辺より右側にある点 左側へずらしていく
			int idx = upper_bound(a.begin(), a.end(), a[i]+len) - a.begin();
			ll tmp1 = (a_sum[N] - a_sum[idx]) - (a[i]+len)*(N-idx);

			// [0,i) 正方形の左辺より左側にある点 右側へずらしていく
			ll tmp2 = a[i]*(i-0) - (a_sum[i] - a_sum[0]);  // -0, -a_sum[0] は不要だが、上記の対比として

			chmin(ret, tmp1+tmp2);
		}
		return ret;
	};

	ll ng = -1, ok = INF32;  // ng:囲めない側, ok:囲める側
	while(ok - ng > 1)
	{
		ll mid = (ng+ok)/2;
		ll cnt = 0;
		ll tmp1, tmp2;
		tmp1 = cal(x, x_sum, mid);
		tmp2 = cal(x_inv, x_inv_sum, mid);  // 各点を正方形の右辺側に置く場合 入力を-1することで判定を流用できる
		cnt += min(tmp1, tmp2);  // 小さい方を採用

		tmp1 = cal(y, y_sum, mid);
		tmp2 = cal(y_inv, y_inv_sum, mid);
		cnt += min(tmp1, tmp2);
		if(cnt <= K) ok = mid;
		else ng = mid;
	}
	cout << ok << endl;

	return 0;
}

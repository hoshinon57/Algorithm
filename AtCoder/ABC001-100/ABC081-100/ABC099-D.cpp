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

// ABC099 https://atcoder.jp/contests/abc099

/*
 * 各種解説と同じ。(どこまで前処理を行うかが異なるが)
 *   公式解説PDF
 *   https://blog.hamayanhamayan.com/entry/2018/06/16/091039
 *   https://drken1215.hatenablog.com/entry/2018/07/22/133200
 * 
 * (x+y)%3=0,1,2である3種のマスについて、何色に塗るかを全探索する。
 * まずここでO(C^3).
 * 元の色が何マスあるかを事前にカウントしておけば、違和感の総和の計算がO(C)で済む。
 * 以上、O(C^4)で十分に間に合う。
 * 
 * [ACまでの思考の流れ]
 * ・どの色に塗るかを全探索するっぽい。
 * ・違和感の計算をN*N愚直にやるのは間に合わない。
 *   各マスの細かい位置はあまり重要では無くて、(x+y)%3の結果だけが重要っぽいので、
 *   前処理でその総和を求めておけば計算量を落とせそう。
**/

int main(void)
{
	ll i, j, k;
	ll N, C; cin >> N >> C;
	vector<vector<ll>> d(C, vector<ll>(C, 0));
	for(i = 0; i < C; i++)
	{
		for(j = 0; j < C; j++) cin >> d[i][j];
	}
	vector<vector<ll>> cs(3, vector<ll>(C, 0));  // cs[idx][c]:idx=(x+y)%3で、色cであるマスの数
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			ll c; cin >> c;
			c--;
			cs[(i+j)%3][c]++;
		}
	}

	// (x+y)%3=idxであるマスを色cで塗るときの、違和感の総和を返す
	auto calc = [&](ll idx, ll c) -> ll
	{
		ll ret = 0;
		ll ii;
		for(ii = 0; ii < C; ii++)  // 元の色iiを色cへ
		{
			ret += d[ii][c] * cs[idx][ii];
		}
		return ret;
	};

	ll ans = INF64;
	for(i = 0; i < C; i++)  // (x+y)%3=0のマスを色iで塗る
	{
		ll sum1 = 0;
		sum1 = calc(0, i);
		for(j = 0; j < C; j++)
		{
			if(i == j) continue;
			ll sum2 = 0;
			sum2 = calc(1, j);
			for(k = 0; k < C; k++)
			{
				if(i == k || j == k) continue;
				ll sum3 = 0;
				sum3 = calc(2, k);
				chmin(ans, sum1+sum2+sum3);
			}
		}
	}
	cout << ans << endl;

	return 0;
}

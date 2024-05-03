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

// ABC041 https://atcoder.jp/contests/abc041

/*
 * 自力で解けず、解説を見た。
 * トポロジカルソートの数え上げ問題。bitDPで解く。
 *   https://ikatakos.com/pot/programming_algorithm/graph_theory/topological_sort
 *   https://tutuz.hateblo.jp/entry/2018/07/19/234447
 * 
 * dp[S]を以下のように定義する。
 *   うさぎを各bitに割り当てて、
 *   1であるうさぎについて考えたときの、着順の総数
 * dp[(1<<N)-1]が答となる。
 * 
 * 「同着はいない」「うさぎx,yの前後の情報が与えられる」などから、トポロジカルソートが可能。
 * この総数を求める問題となる。
 * func(S)について以下が成り立つ。
 *   Sについてトポロジカルソートしたときの、末尾となる頂点をvとしたときに、各vについて
 *   func(S) = Σ(func(Sからvを取り除いたもの))
 * 
 * トポロジカルソートの末尾を判定するためのテクニックとして、
 *   nxt[i]:頂点iの次頂点(のbit)を1にしたもの
 * として事前計算しておくと、
 * S&nxt[i]=0 ならiは末尾の頂点である。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・トポロジカルソートの数え上げ、は知識問題な気がする。覚えておこう。
 * ・頂点iがトポロジカルソートの末尾になるか、の判定はテクニックとして覚えておく。
 */

vector<ll> dp;
vector<int> nxt;

// dp[S]を返す
ll func(int S, int N)
{
	int i;
	ll &d = dp[S];
	if(d != -1) return d;  // メモ化済み
	if(__builtin_popcount(S) == 1) { return (d=1); }  // ベースケース

	d = 0;
	for(i = 0; i < N; i++)
	{
		if(((S>>i)&1) == 0) continue;
		if((S & nxt[i]) == 0)  // 頂点iがトポロジカルソートの末尾
		{
			// iビット目を落として再帰し、答に加算
			int nxtS = S & (~(1<<i));
			d += func(nxtS, N);
		}
	}

	return d;
}

int main(void)
{
	int i;
	ll N, M; cin >> N >> M;
	dp.resize(1<<N, -1);
	nxt.resize(N);
	for(i = 0; i < M; i++)
	{
		int x, y; cin >> x >> y;  // x->y
		x--; y--;
		nxt[x] |= 1<<y;
	}

	cout << func((1<<N)-1, N) << endl;

	return 0;
}

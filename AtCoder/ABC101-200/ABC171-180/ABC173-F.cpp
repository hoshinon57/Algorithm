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

// ABC173 https://atcoder.jp/contests/abc173

/*
 * 自力で解けず、解説を見た。
 * (解法の一部に)主客転倒を用いて解く。
 *   https://blog.hamayanhamayan.com/entry/2020/07/05/235609
 *   https://x.com/kyopro_friends/status/1279776575773274113
 * 
 * まず、木や森の問題のように閉路(サイクル)が存在しない場合に、以下が成り立つ。[典型]
 *   (頂点数) - (辺数) = (連結成分数)
 * 
 * 求めたいのは全組み合わせの連結成分数なので、
 *   ΣΣf(L,R) = ΣΣ{(1:L～Rの頂点数) - (2:L～Rの頂点を両端に持つ変数)}
 * となり、また1,2は独立に計算できる。
 * 
 * 以下、0-indexedで記載する。
 * 
 * 1:L～Rの頂点数は、i=Lとして各iごとに
 * i=0:1+2+3+...+N
 * i=1:1+2+3+...+(N-1)
 * i=2:1+2+3+...+(N-2)
 * i=k:1+2+3+...+(N-k) = (N-k)*(N-k+1)/2
 * となるので、O(N)で求められる。
 * 
 * 2:L～Rの頂点を両端に持つ変数には、主客転倒を用いる。
 * 各辺についてその両端を集合Sに含むような選び方を求める。
 * これは辺の両端の頂点をu,v(u<v)としたときに、L<=u, v<=Rを満たすL,Rの組となるので、
 *   (u+1)*(N-v)
 * で求められる。これを辺ごとに計算して引けばよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・この類の数え上げは主客転倒だろう、とは思ったが、それ以上の考察に進めなかった。
 * ・木や森にて、 (頂点数) - (辺数) = (連結成分数) の公式。
 */

int main(void)
{
	// 0-indexed
	ll i;
	ll N; cin >> N;
	vector<ll> u(N-1), v(N-1);
	for(i = 0; i < N-1; i++) 
	{
		cin >> u[i] >> v[i];
		u[i]--; v[i]--;
		if(u[i] > v[i]) swap(u[i], v[i]);  // u<vにする
	}

	ll ans = 0;
	// 1:L～Rの頂点数
	for(i = 0; i < N; i++)
	{
		ans += (N-i)*(N-i+1)/2;
	}
	// 2:L～Rの頂点を両端に持つ変数
	for(i = 0; i < N-1; i++)
	{
		ans -= (u[i]+1)*(N-v[i]);
	}
	cout << ans << endl;

	return 0;
}

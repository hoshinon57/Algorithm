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

// ABC386 https://atcoder.jp/contests/abc386

/*
 * コンテスト時に解けず、解説を見た。
 * 
 * nCk(N,K)<=10^6という制約より、DFSによる全探索でいけそう、と思った。
 * しかしTLEが取り切れなかった。
 * 
 * 解説によると、Kが大きいとdfs()が呼び出される回数が大きくなってしまうとのこと。
 * ※ここが理解しきれてない。要勉強。
 * 
 * よって、Kが大きいとき、正確には「選ばない個数(N-K)の方が、選ぶ個数(K)より少ない」のであれば
 * 選ばないものを全探索する方針とする。
 * これはdfs()のxor初期値を全A[*]のxorにし、KをN-Kに置き換えるだけで良い。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・いやーこれ計算量の見積もりが難しい…
 *   Kが大きいとdfs()呼び出し回数が多くなるのが理解しきれてない。要勉強だなあ。
 *   ※再上位陣もWA率が高く、コードを見るとmin(K,N-K)をやっていない人ばかりだった。
 * ・「K個選ぶ」は「選ばない(N-K)について考えて、同じ結果を出す」という方針を覚えておく。
 * ・今回、TLEしたのは全48ケース中3つとかだった。
 *   これなら「どういう入力で時間がかかるのか」を試行錯誤すべきだった。
 */

ll N, K;
vector<ll> a;
ll ans = 0;

// idx:次に見るa[idx], k個選び済み, xr:現在のxor値
void dfs(ll idx, ll k, ll xr)
{
	if(k == K)  // K個選んだ
	{
		chmax(ans, xr);
		return;
	}
	if(idx == N) return;  // K個選んでないが、最後まで到達した

	// a[idx]を使うケースと、使わないケース
	dfs(idx+1, k+1, xr ^ a[idx]);
	dfs(idx+1, k, xr);
}

int main(void)
{
	int i;
	cin >> N >> K;
	a.resize(N);
	for(i = 0; i < N; i++) cin >> a[i];

	ll st = 0;  // xor初期値
	if(chmin(K, N-K))
	{
		for(auto &e : a) st ^= e;  // 「選ばない」個数の方が少ないので、xor初期値を全要素のxorにしておく
	}
	dfs(0, 0, st);
	cout << ans << endl;
	return 0;
}

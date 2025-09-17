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

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

// 解説はそのままで、実装は2025/9に再解きしたものに置き換えた。

// ABC272 https://atcoder.jp/contests/abc272

/*
 * コンテスト中に解けず、解説を見て実装した。
 * 以下、1-indexedで記載する。
 *  
 * 「Aに含まれない」「非負の最小整数」という条件から、答は0～Nの範囲になる。
 * 
 * ・A[1]が0～Nの範囲になるのは何回目から何回目までの操作か
 * ・A[2]も同様に何回目から何回目までの操作か
 * ・A[3]～A[N]も同様に…
 * と事前に求めるのは、計算量がO(N^2)で間に合わないように一見思える。
 * ただしA[x]に対して0～Nの範囲になるのは N/x 回である。
 * すると N/1 + N/2 + N/3 + ... + N/N = N*(1/1 + 1/2 + 1/3 + ... + 1/N) であり、
 * これは調和級数により O(N*logN) 相当になる。よって間に合う。
 * ※A[x]の初期値が負の場合、何回目の操作で0以上になるかをO(1)で求めるのが前提。
 * 
 * 実装方針としては、まず各回の操作ごとに0～Nの範囲で含まれる数をnum_list[x]にて列挙する。
 * その後、num_list[]ごとに0から順に見ていき、存在しない値が見つかればそれが答となる。
 * (例えば num_list[2]={1,2,0,5} であれば、2回目の操作後にて非負の最小整数は3である)
 * 
 * [補足]
 * num_list[]をvectorではなくsetで保持する場合、setの操作はO(logN)のため全体ではO(N*logN*logN)となる。
 * ただこの場合でもTLEにはならなかったことを確認した。
 *   vectorでの実装：498ms
 *   setでの実装：1109ms
 */

int main(void)
{
	// 1-indexed
	ll i, t;
	ll N, M; cin >> N >> M;
	vector<ll> a(N+1); for(i = 1; i <= N; i++) {cin >> a[i];}

	vector<vector<ll>> k(M);  // k[t]: tターン目のMEX候補
	for(i = 1; i <= N; i++)
	{
		t = 0;
		if(a[i] < 0)
		{
			t = ceil_div(-a[i], i) - 1;  // tターン目の操作からAi>=0になる
			a[i] += t*i;  // tは0始まりなので、事前のaddはt*i
		}
		for( ; t < M; t++)  // tターン目
		{
			a[i] += i;
			if(a[i] > N) break;
			k[t].push_back(a[i]);
		}
	}

	for(t = 0; t < M; t++)
	{
		// 元データをソートし、重複を削除する
		sort(k[t].begin(), k[t].end());
		k[t].erase(unique(k[t].begin(), k[t].end()), k[t].end());  // unique()で隣り合う重複を削除し、erase()で末端までのゴミを削除する
		for(i = 0; i <= N; i++)
		{
			if(i >= (int)k[t].size() || k[t][i] != i)
			{
				cout << i << endl;
				break;
			}
		}
	}

	return 0;
}

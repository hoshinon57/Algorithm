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

// ABC367 https://atcoder.jp/contests/abc367

/*
 * ダブリングで解く。
 * 
 * AiをA(Xi)で置き換える操作は、i->Xiへの辺を張るグラフで考えてみる。[典型]
 * Kの制約より、ダブリングっぽい、となる。
 * 
 * 単純に実装した内容から、以下2点の実行時間改善を行ってみた。
 * ・db[]をvectorから二次元配列に変更 (効果：小)
 * ・事前計算を終わらせてK回移動後を求めるとき、キャッシュが効くようループの順序を変更 (効果：大)
 * 変更前後は以下。
 * https://atcoder.jp/contests/abc367/submissions/59468013
 * https://atcoder.jp/contests/abc367/submissions/59468149
 */

const int LOG_K = 60;
ll db[LOG_K+1][200005];  // db[i][j]:頂点jから2^i先

int main(void)
{
	ll i, k;
	ll N, K; cin >> N >> K;
	vector<ll> x(N); for(i = 0; i < N; i++) {cin >> x[i]; x[i]--;}
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	// db[0][*]を計算
	for(i = 0; i < N; i++)
	{
		db[0][i] = x[i];
	}
	// db[k][*]を計算
	for(k = 1; k <= LOG_K; k++)
	{
		for(i = 0; i < N; i++)
		{
			// iの2^k個先は、
			// "iの2^(k-1)個先" の要素からさらに "2^(k-1)個先" の要素となる
			db[k][i] = db[k-1][db[k-1][i]];
		}
	}

#if 0
	vector<ll> ans;
	for(i = 0; i < N; i++)  // i番目の要素について答える
	{
		auto K_bk = K;
		ll p = i;
		for(k = 0; k <= LOG_K; k++)
		{
			if((K&1) == 1)
			{
				p = db[k][p];
			}
			K >>= 1;
		}
		ans.push_back(a[p]);
		K = K_bk;
	}
#else
	vector<ll> ans(N);
	for(k = 0; k <= LOG_K; k++)
	{
		for(i = 0; i < N; i++)  // i番目の要素
		{
			if(k == 0) ans[i] = i;
			if((K&1) == 1)
			{
				ans[i] = db[k][ans[i]];
			}
		}
		K >>= 1;
	}
	for(i = 0; i < N; i++) ans[i] = a[ans[i]];
#endif
	int sz_ = (int)ans.size();
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}

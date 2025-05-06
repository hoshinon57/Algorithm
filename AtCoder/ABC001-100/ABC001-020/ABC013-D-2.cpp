#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC013 https://atcoder.jp/contests/abc013

/*
 * 公式解説を参考に、ダブリングで解いてみた版。
**/

int main(void)
{
	ll i, j, k;
	ll N, M, D; cin >> N >> M >> D;

	// nx[i]=x 元の左からx列目は、後でi列目に行く x->i
	vector<ll> nx(N);
	iota(nx.begin(), nx.end(), 0LL);
	for(i = 0; i < M; i++)
	{
		ll a; cin >> a;
		a--;
		// 列aと列(a+1)をswap
		swap(nx[a], nx[a+1]);
	}
	// nx[i] -> i
	const ll LOG_K = 30;
	vector<vector<ll>> db(LOG_K+1, vector<ll>(N));
	// db[0][*]を計算
	for(i = 0; i < N; i++)
	{
		db[0][nx[i]] = i;
	}
	// db[k][*]を計算
	for(k = 1; k <= LOG_K; k++)
	{
		for(i = 0; i < N; i++)
		{
			db[k][i] = db[k-1][db[k-1][i]];
		}
	}

	vector<int> ans(N);
	for(i = 0; i < N; i++) ans[i] = i;  // 「あみだくじ0回実施後は、i->iにいる」というイメージ
	for(i = 0; i <= LOG_K; i++)
	{
		if((D&1) == 1)
		{
			for(j = 0; j < N; j++)
			{
				ans[j] = db[i][ans[j]];
			}
		}
		D >>= 1;
	}
	for(auto &e : ans) cout << e+1 << endl;

	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:87 https://atcoder.jp/contests/typical90/tasks/typical90_ci

/*
 * ワーシャルフロイド法と二分探索で解く。
 * 以下、問題文の"交通費"を距離として記載する。
 * 
 * まず、正の整数Xを決めたときの各街間の距離を求める必要がある。
 * これはワーシャルフロイド法で求められる。
 * 
 * 次に「横軸をX, 縦軸を距離がP以下である組数」のグラフを考えると、これは広義単調減少になりそう。
 * すると「要件を満たす組がZZ個"以上"であるXの範囲」は二分探索で求められる。
 * ちょうどK個については、
 *   ZZ=K+1
 *   ZZ=K
 * の2パターンで二分探索することで、「ちょうどK個」の範囲を求めることができる。
 * 
 * [ACまでの思考の流れ]
 * ・Xを変えていったときに、距離がP以下であるような数がどうなるかを考えると、どうも広義単調減少になりそう。
 *   ⇒二分探索が使えそう。
 * ・K+1個以上とK個以上とで2回二分探索すれば、ちょうどK個の範囲が求められそう。
 */

ll N, P, K;

// ワーシャルフロイド法を用いて全点対間最短経路を解く。
// dist:
//   頂点数をNとしたとき、サイズはdist[N][N]で、"INF64"で初期化していること。
//   a->bの距離をdist[a][b]として設定しておくこと。
//   (呼び出し元で、dp[i][i]=0にすることを忘れないよう注意)
// 負閉路があればtrueを返す。
bool Warshall_Floyd(vector<vector<ll>> &dist)
{
	int i, j, k;
	int N_ = dist.size();

	for(k = 0; k < N_; k++)
	{
		for(i = 0; i < N_; i++)
		{
			for(j = 0; j < N_; j++)
			{
				if(dist[i][k] != INF64 && dist[k][j] != INF64)  // i-k, k-j間がどちらも通れる場合
				{
					dist[i][j] = min(dist[i][j], dist[i][k]+dist[k][j]);
				}
			}
		}
	}

	// dist[v][v] < 0 が存在する場合、負閉路がある
	bool exist_negative_cycle = false;
	for(i = 0; i < N_; i++)
	{
		if(dist[i][i] < 0) exist_negative_cycle = true;
	}

	return exist_negative_cycle;
}

// 要件を満たすような組がborder個以上/未満の境界を求める
// {border個以上となるXの右端, border個未満となるXの左端}のペアを返す
pair<ll,ll> bsearch(ll border, vector<vector<ll>> &a_org)
{
	int i, j;
	ll l = 0, r = INF64;
	// 判定問題：X=xとしたとき、距離がP以下であるものがborder個以上であるか？
	auto check = [&](ll x) -> bool
	{
		auto a = a_org;
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				if(a[i][j] == -1) a[i][j] = x;
			}
		}
		Warshall_Floyd(a);
		int cnt = 0;
		for(i = 0; i < N; i++)
		{
			for(j = i+1; j < N; j++)
			{
				if(a[i][j] <= P) cnt++;
			}
		}
		return cnt >= border;
	};
	while(r - l > 1)
	{
		ll mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}

	return {l, r};
}

int main(void)
{
	int i, j;
	cin >> N >> P >> K;
	vector<vector<ll>> a_org(N, vector<ll>(N, INF64));  // a[N][N]
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			cin >> a_org[i][j];
		}
	}

	ll L = bsearch(K+1, a_org).second;  // "K+1個以上のr", つまりK個の左端
	ll R = bsearch(K, a_org).first;  // "K個以上のl", つまりK個の右端
	ll ans = R-L+1;
	if(ans >= INF64/2) cout << "Infinity" << endl;
	else cout << ans << endl;
	
	return 0;
}

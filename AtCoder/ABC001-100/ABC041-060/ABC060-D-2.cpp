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

// ABC060 https://atcoder.jp/contests/abc060

/*
 * 公式解説やdrken氏の解法で実装したもの。
 *   https://drken1215.hatenablog.com/entry/2024/10/19/042434
 * 
 * 重さの種類が4つしかない点に着目する。
 * 例えば、重さXの物が3つ、価値がそれぞれv0,v1,v2だったとすると、価値が高い順に貪欲に選ぶのが最適である。
 * 
 * val[i][*]を
 *   重さが(w[0]+i)である物の価値一覧
 * として定義する。
 * iは0～3の4通り、
 * jはサイズNで、降順ソートしておく。(Nに足りない分は0で埋めておく)
 * またこの累積和も求めておく。
 * 
 * 重さが w[0], (w[0]+1]), (w[0]+2), (w[0]+3) の4つについて、それぞれ何個選ぶかを全探索する。
 * N=100であり、枝刈りを含めてO(N^4)は十分に間に合う。
 * 
 * 例えばw[0]のものをx個選ぶ場合、累積和にて sum[x]-sum[0] が価値総和となる。
 * sum[0]は常に0なので、sum[x]だけで良い。
**/

int main(void)
{
	ll i, j, k, h;
	ll N, W; cin >> N >> W;
	vector<vector<ll>> val(4);  // val[i][*]: 重さが(w[0]+i)である物の価値一覧
	vector<ll> w(N), v(N); for(i = 0; i < N; i++) {cin >> w[i] >> v[i];}
	for(i = 0; i < N; i++)
	{
		val[w[i]-w[0]].push_back(v[i]);
	}
	for(auto &e : val)
	{
		e.resize(N);  // 0で埋める
		sort(e.begin(), e.end(), greater<ll>());
	}
	vector<vector<ll>> sum(4, vector<ll>(N+1, 0));  // sum[4][N+1]
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < N; j++)
		{
			sum[i][j+1] = sum[i][j] + val[i][j];
		}
	}

	ll ans = 0;
	for(i = 0; i < N+1; i++)  // 重さw[0]をi個
	{
		for(j = 0; j < N+1; j++)  // 重さ(w[0]+1)をj個
		{
			if(i+j > N) break;  // 枝刈り
			for(k = 0; k < N+1; k++)
			{
				if(i+j+k > N) break;
				for(h = 0; h < N+1; h++)
				{
					if(i+j+k+h > N) break;
					ll ws = i*w[0] + j*(w[0]+1) + k*(w[0]+2) + h*(w[0]+3);
					if(ws > W) continue;

					ll tmp = sum[0][i] + sum[1][j] + sum[2][k] + sum[3][h];
					chmax(ans, tmp);
				}
			}
		}
	}
	cout << ans << endl;

	return 0;
}

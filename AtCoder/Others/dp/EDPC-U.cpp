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

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * bitDPで解く。
 * 
 * dp[S]を以下のように定義する。
 *   うさぎごとにbitに割り当てて、
 *   bitが1であるうさぎをグループ分けするときの、得点の最大値
 * 
 * dp[S]は、Sについて
 * (1)これ以上グループ分けしないケース
 * (2)さらにグループ分けするケース
 * をそれぞれ求めて、その最大値となる。
 * dp[(1<<N)-1]が答。
 * 
 * (1)は単純、実装を参照。
 * (2)は S=T1∪T2 (T1∩T2={}) となるT1,T2について dp[T1]+dp[T2] となる。
 * T1,T2を全探索する必要があり、これはSの部分集合を全探索してT1とし、その残りをT2とする。
 * T1の簡単な全探索方法は、実装および以下解説を参照。
 *   https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106
 * 
 * 最初、dp[S][n]で n:グループ分けする数 としたがN=16のサンプル4でTLEとなった。
 * グループ分けの個数は情報として不要のため、dp[S]で十分だった。
 * dp[S][n]にて、本問と典型90-45を踏まえると
 *   N=16:間に合わない
 *   N=15:間に合うが、枝刈りが必要
 * となるっぽい。
 */

int N;
vector<ll> dp;
vector<vector<ll>> a;

// dp[S]を返す
ll func(int S)
{
	int i, j;
	ll &d = dp[S];
	if(d != -INF64) return d;  // メモ化済み
	d = 0;

	// まず、これ以上グループ分けしないケースを計算
	/*
	 * [memo]
	 * (1)Sにてbitが立っている部分を事前計算し、Sに応じてループ回数を抑えるケース
	 * (2)Sに関わらずN^2で計算するケース
	 * の両方を試したが、実行時間はほとんど変わらなかった。((1)が194ms, (2)が195ms)
	 * テストケースごとの実行時間も比べたが、2～3%程度の差だった。
	 * 本問に関しては(2)で実装した方が手っ取り早い。
	 */
#if 0
	int sz = __builtin_popcount(S);
	vector<int> r;  // Sに含まれるうさぎの番号
	for(i = 0; i < N; i++)
	{
		if((S>>i)&1) r.push_back(i);
	}
	for(i = 0; i < sz; i++)
	{
		for(j = i+1; j < sz; j++)
		{
			d += a[r[i]][r[j]];
		}
	}
#else
	for(i = 0; i < N; i++)
	{
		if(((S>>i)&1) == 0) continue;
		for(j = i+1; j < N; j++)
		{
			if(((S>>j)&1) == 0) continue;
			d += a[i][j];
		}
	}
#endif

	// Sをグループ分けするケースについて計算
	int T;
	for(T = S; T >= 0; T--)  // T:Sの部分集合
	{
		T &= S;
		int T2 = S & (~T);  // T∪T2=S
		ll tmp = func(T) + func(T2);
		chmax(d, tmp);
	}

	return d;
}

int main(void)
{
	int i, j;
	cin >> N;
	dp.resize(1<<N, -INF64);
	a.resize(N, vector<ll>(N));
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++) cin >> a[i][j];
	}

	cout << func((1<<N)-1) << endl;

	return 0;
}

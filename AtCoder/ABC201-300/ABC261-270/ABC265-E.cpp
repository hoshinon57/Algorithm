#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC265 https://atcoder.jp/contests/abc265

/*
 * 動的計画法で解く。
 * 
 * 空間が広いので、dp[x][y]のように持つとメモリが足りない。
 * 連想配列を使って dp[{x,y}]=(経路の数) のように保持する。
 * 
 * 便宜上、以下のように命名する。
 *   ワープ1：+A,+B方向のワープ
 *   ワープ2：+C,+D方向
 *   ワープ3：+E,+F方向
 * 単純な動的計画法で考えて、
 *   前のフェーズで到達可能な座標全てについて、
 *   そこからワープ1,2,3それぞれの先に行けるか
 * を判定、処理していく。
 * 
 * ただし連想配列を
 *   vector<map<pair<ll, ll>, ll>> dp;    または
 *   map<pair<ll, ll>, ll> dp[300+1];
 * のように全フェーズ持つと一部のテストケースでTLEとなった。
 *   map<pair<ll, ll>, ll> dp, new_dp;
 * のように今回と前回のみを保持するとAC(1600ms程度)であった。
 * 原因は不明だが、メモリ使用量が前者は300MB近く、後者は10MB程度であり、
 * vector<map<...>>で大量のメモリを消費する解法は、AtCoderでは避けた方が良いのかもしれない。
 */
const ll MOD = 998244353;

int main(void)
{
	ll i;
	ll N, M;
	cin >> N >> M;
	ll A, B, C, D, E, F;
	cin >> A >> B >> C >> D >> E >> F;
	map<pair<ll, ll>, bool> wall;  // {x,y} -> その座標に障害物があればtrue
	for(i = 0; i < M; i++)
	{
		ll x, y;
		cin >> x >> y;
		wall[{x,y}] = true;
	}
	// ここまで入力

	// dp[300+1]のように配列では持たず、今回と前回のみを保持することでメモリ使用量を抑える。
	// (ソース先頭の方針に書いたが、dp[300+1]のように全て持つとTLEとなった)
	// DPの遷移は dp->new_dp の方向。
	map<pair<ll, ll>, ll> dp;
	dp[{0,0}] = 1;  // 初期値
	for(i = 1; i <= N; i++)
	{
		map<pair<ll, ll>, ll> new_dp;
		// 配るDP
		for(auto &e : dp)
		{
			ll x = e.first.first;
			ll y = e.first.second;
			if(wall.find({x+A, y+B}) == wall.end())  // 行き先に障害物が無ければ、ワープ可能
			{
				new_dp[{x+A, y+B}] += e.second;
				new_dp[{x+A, y+B}] %= MOD;
			}
			if(wall.find({x+C, y+D}) == wall.end())
			{
				new_dp[{x+C, y+D}] += e.second;
				new_dp[{x+C, y+D}] %= MOD;
			}
			if(wall.find({x+E, y+F}) == wall.end())
			{
				new_dp[{x+E, y+F}] += e.second;
				new_dp[{x+E, y+F}] %= MOD;
			}
		}
		swap(dp, new_dp);  // dpとnew_dpの2つでやりくりする
	}

	ll answer = 0;
	for(auto &e : dp)
	{
		answer += e.second;
		answer %= MOD;
	}
	cout << answer << endl;

	return 0;
}

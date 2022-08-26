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
 * メモ化再帰で解く。
 * 
 * 便宜上、以下のように命名する。
 *   ワープ1：+A,+B方向のワープ
 *   ワープ2：+C,+D方向
 *   ワープ3：+E,+F方向
 * 例としてワープ1を2回、ワープ2を1回行う場合を考えたときに、
 *   ワープ1,1,2
 *   ワープ1,2,1
 *   ワープ2,1,1
 * は経路が異なる一方、最終的な行き先は同じとなる。
 * 
 * よってDPをワープの使用回数として、以下のように持つ。
 *   dp[i][j][k]
 *   ワープ1をi回  i:0～N(MAX:300)
 *   ワープ2をj回  j:0～N
 *   ワープ3をk回  k:0～N
 *   行う場合の経路の数
 * するとDPの遷移は以下になる。
 *   dp[i][j][k] = dp[i-1][j][k] + dp[i][j-1][k] + dp[i][j][k-1]
 * これをメモ化再帰で解いていく。
 */

const ll MOD = 998244353;

// dp[i][j][k]
// (+A,+B)のワープをi回  i:0～N(MAX:300)
// (+C,+D)のワープをj回  j:0～N
// (+E,+F)のワープをk回  k:0～N
// 行う場合の経路の数
ll dp[301][301][301] = {0};
int dp_memo[301][301][301] = {0};  // 1ならメモ化済
map<pair<ll, ll>, bool> wall;  // {x,y} -> その座標に壁があればtrue
ll A, B, C, D, E, F;

// 3種のワープをそれぞれi回、j回、k回行う場合の経路の数を返す
ll func(int i, int j, int k)
{
	// ベースケース
	if(i == 0 && j == 0 && k == 0) return 1;
	
	// メモ済みならそれを返す
	if(dp_memo[i][j][k] == 1) return dp[i][j][k];

	// 3種のワープをそれぞれi回、j回、k回行った先に、障害物があるか
	ll x = i*A + j*C + k*E;
	ll y = i*B + j*D + k*F;
	if(wall.find({x,y}) != wall.end())
	{
		// 障害物があるなら行けない
		dp_memo[i][j][k] = 1;
		dp[i][j][k] = 0;
		return 0;
	}

	// もらうDPにて、DPの遷移は以下式で表すことができる
	// dp[i][j][k] = dp[i-1][j][k] + dp[i][j-1][k] + dp[i][j][k-1]
	// 再帰を使って解いていく
	ll answer = 0;
	if(i > 0)
	{
		answer += func(i-1, j, k);
		answer %= MOD;
	}
	if(j > 0)
	{
		answer += func(i, j-1, k);
		answer %= MOD;
	}
	if(k > 0)
	{
		answer += func(i, j, k-1);
		answer %= MOD;
	}

	// メモ化
	dp_memo[i][j][k] = 1;
	dp[i][j][k] = answer;
	
	return answer;
}

int main(void)
{
	ll i, j, k;
	ll N, M;
	cin >> N >> M;
	cin >> A >> B >> C >> D >> E >> F;
	for(i = 0; i < M; i++)
	{
		ll x, y;
		cin >> x >> y;
		wall[{x,y}] = true;
	}
	// ここまで入力

	ll answer = 0;
	// N回のジャンプを、3種のワープに振り分ける
	for(i = 0; i <= N; i++)  // i:(+A,+B)のワープ回数
	{
		for(j = 0; j <= N-i; j++)  // j:(+C,+D)のワープ回数
		{
			k = N-i-j;  // k:(+E,+F)のワープ回数
			answer += func(i, j, k);
			answer %= MOD;
		}
	}
	cout << answer << endl;

	return 0;
}

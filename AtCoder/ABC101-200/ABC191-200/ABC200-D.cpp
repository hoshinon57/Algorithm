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

// ABC200 https://atcoder.jp/contests/abc200

/*
 * DPおよびDPの復元で解く。
 * 
 * まず、数列Aの値は200で割った余りに置き換えてしまって良い。
 * dp[i][j]を以下のように定義する。
 *   Aiまで見たときに、  (i:1～N)
 *   それまで選んだ和を200で割った余りがjとなるような  (j:0～199)
 *   選び方の数
 * これは典型的なDPで解ける。
 * ただし選び方の総数が2^nで増加していくため、上限クリップしておく。
 * 
 * その後DPの復元を行う。
 * dp[N][*]が2以上となるような要素を探す。
 * (ただし「1つも選ばない」ケースがあるため、dp[N][0]については3以上とする)
 * 見つけたら、そこから先頭方向へDFS. 選んだものを復元していく。
 * 先頭まで到着したらans[]に入れ、2つ以上復元したら終了。
 * 
 * [ACまでの思考の流れ]
 * ・「好きな個数選ぶ」「和を200で割った余りで考える」などからDPを思いついた。
 * ・DPした後、200で割った余りが2つ以上あるケースにて、復元すれば良さそう。
 * ※dp[i][j]の計算時、上限判定を入れてなくて1WA.
 *   また上限判定を2にしていて1WA. (割った余りが0のときは上限3まで必要)
 */

const int MOD = 200;
int N;
vector<int> a;
vector<vector<ll>> dp;

vector<vector<int>> ans;
// DP結果の復元
// list[]に条件を満たす数列を入れていき、先頭まで戻ったらans[]にコピー
// 2つansに入れたら終了
bool dfs(int idx, int n, vector<int> &list)
{
	if(idx == 0)
	{
		if(list.size() == 0) return false;  // 1つも選ばなかった
		ans.push_back(list);
		if(ans.size() == 2) return true;
		return false;
	}

	// a[idx]を選ばないケース
	if(dp[idx-1][n] != 0)
	{
		if(dfs(idx-1, n, list)) return true;
	}
	// a[idx]を選ぶケース
	int prev = (n-a[idx]+MOD) % MOD;
	if(dp[idx-1][prev] != 0)
	{
		list.push_back(idx);
		if(dfs(idx-1, prev, list)) return true;
		list.pop_back();
	}
	return false;
}

int main(void)
{
	int i, j;
	cin >> N;
	a.resize(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> a[i];
		a[i] %= MOD;
	}
	dp.resize(N+1, vector<ll>(MOD));
	dp[0][0] = 1;
	// もらうDP
	for(i = 1; i <= N; i++)
	{
		for(j = 0; j < MOD; j++)
		{
			// a[i]を選ばない場合と選ぶ場合
			dp[i][j] += dp[i-1][j];
			int prev = (j-a[i]+MOD) % MOD;
			dp[i][j] += dp[i-1][prev];
			// 数が爆発的に増加するためリミット
			// 除外すべき「全て選ばない」ケースがあるため2+1を上限
			chmin(dp[i][j], 3LL);
		}
	}

	// DP復元
	// dp[N][*]で2以上のものを選ぶ ただしdp[N][0]なら3以上
	vector<int> list;
	for(i = 0; i < 200; i++)
	{
		if(i == 0 && dp[N][i] >= 3)
		{
			dfs(N, i, list);
			break;
		}
		else if(i > 0 && dp[N][i] >= 2)
		{
			dfs(N, i, list);
			break;
		}
	}

	if(ans.size() != 2)
	{
		cout << "No" << endl;
	}
	else
	{
		cout << "Yes" << endl;
		for(i = 0; i < 2; i++)
		{
			reverse(ans[i].begin(), ans[i].end());  // 末尾から積んでいったので反転
			cout << ans[i].size() << " ";
			for(auto &e : ans[i]) cout << e << " ";
			cout << endl;
		}		
	}

	return 0;
}

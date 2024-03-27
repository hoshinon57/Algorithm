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

// 競プロ典型90問:88 https://atcoder.jp/contests/typical90/tasks/typical90_cj

/*
 * DP復元にて解こうとしたが、TLEが取れず断念。
 * 供養として残しておく。
 * 
 * dp[i][j]を、
 *   カードi枚目まで見て、 (i=1～N)
 *   そこまでの総和がj  (j=0～8888)
 *   の選び方の総数
 * と定義すると、一般的なDPで求められる。
 * これをdp[N][*]>=2となるものについて復元していく…という考え方。
 * 
 * TLEとなったのは、「NGとなる組み合わせのカード」を復元時にチェックしたからだろうか。
 */

int N, Q;
vector<int> a;
vector<vector<bool>> ng;  // ng[i][j]=trueならカードi,jの組み合わせはNG
vector<vector<int>> ans;

vector<vector<ll>> dp;

// idx:今見ているカードの番号
bool dfs(int idx, int sum, vector<int> &list)
{
	if(idx == 0)
	{
		ans.push_back(list);
		return (ans.size() == 2);
	}

	// カードを選ばない
	if(dp[idx-1][sum] > 0)
	{
		if(dfs(idx-1, sum, list)) return true;
	}

	// カードを選ぶ
	auto check = [&](void) -> bool  // idxを選べるか
	{
		for(auto &e : list)
		{
			if(ng[idx][e]) return false;
		}
		return true;
	};
	if(sum-a[idx] >= 0 && dp[idx-1][sum-a[idx]] > 0)
	{
		if(check())
		{
			list.push_back(idx);
			if(dfs(idx-1, sum-a[idx], list)) return true;
			list.pop_back();
		}
	}

	return false;
}

int main(void)
{
	// 1-indexed
	int i, j;
	cin >> N >> Q;
	a.resize(N+1);
	for(i = 1; i <= N; i++) cin >> a[i];
	ng.resize(N+1, vector<bool>(N+1, false));
	for(i = 0; i < Q; i++)
	{
		int x, y; cin >> x >> y;
		ng[x][y] = ng[y][x] = true;
	}
	int asum = 0;
	for(auto &e : a) asum += e;
	dp.resize(N+1, vector<ll>(asum+1, 0));  // dp[N+1][asum_1]
	dp[0][0] = 1;
	// もらうDP
	for(i = 1; i <= N; i++)
	{
		// カードa[i]
		for(j = 0; j <= asum; j++)
		{
			dp[i][j] = dp[i-1][j];
			if(j-a[i] >= 0)
			{
				dp[i][j] += dp[i-1][j-a[i]];
				// 選び方の総数が2^nで増えていくため、上限クリップ
				// (これが無いとTLEに加え、REもあった)
				chmin(dp[i][j], (ll)100*100);
				/*
				if(dp[i][j] < 0)
				{
					cout << "hogehoge" << endl;
					return 0;
				}
				*/
			}
		}
	}

	for(j = asum; j >= 0; j--)
	{
		if(dp[N][j] < 2) continue;
		ans.clear();
		vector<int> list;
		if(dfs(N, j, list)) break;
	}

	reverse(ans[0].begin(), ans[0].end());
	reverse(ans[1].begin(), ans[1].end());
	for(i = 0; i < 2; i++)
	{
		cout << ans[i].size() << endl;
		for(j = 0; j < (int)ans[i].size(); j++)
		{
			cout << ans[i][j];
			if(j != (int)ans[i].size()-1) cout << " ";
		}
		cout << endl;
	}

	return 0;
}

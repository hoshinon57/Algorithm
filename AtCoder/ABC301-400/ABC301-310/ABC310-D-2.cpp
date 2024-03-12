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

// ABC310 https://atcoder.jp/contests/abc310

/*
 * bitDPで解く。
 * 参考：典型90-45(Simple Grouping)
 * 
 * dp[S][t]を以下のように定義する。
 *   Sの各bitを選手に割り当て、
 *   bitが1になっている選手を用いてtチームを作るときの、組み合わせの数
 *   ただしこの状態ではチーム番号は区別する、つまり {1},{2,3} と{2,3},{1} は別カウントとする。
 * 
 * dp[(1<<N)-1][T]が答。
 * ただし前述の通り重複カウントがある。入力Tについて T! だけ重複しているので、T!で割ったものが答となる。
 * 
 * dp[S][t]を求めるには、S=S1∪S2となるようなSの部分集合S1,S2について
 *   dp[S1][t-1] * dp[S2][1]
 * を加算していく。
 * グループを1つだけ取り出すイメージ。
 * S1,S2の全組み合わせは、Sにて立っているbitを元にbit全探索を用いた。
 */

int N, M, T;
const int MAX_N = 10;
const int MAX_T = 10;
ll dp[1<<MAX_N][MAX_T+1];
vector<vector<bool>> t_ng;  // t_ng[i][j]:選手i,jが不仲ならtrue

ll bitDP(int S, int t)
{
	ll &d = dp[S][t];
	if(d != -1) return d;  // メモ化済み

	int i, j;
	int scnt = __builtin_popcount(S);
	if(scnt < t) { return (d = 0); }

	vector<int> sid;  // Sにて1が立っているbitの番号 つまりSに属する選手の一覧
	for(i = 0; i < N; i++)
	{
		if((S>>i)&1) sid.push_back(i);
	}
	if(t == 1)  // ベースケース
	{
		bool ok = true;
		// 不仲チェック
		for(i = 0; i < scnt; i++)
		{
			for(j = i+1; j < scnt; j++)
			{
				if(t_ng[sid[i]][sid[j]]) ok = false;
			}
		}
		if(ok) d = 1;
		else d = 0;
		return d;
	}

	d = 0;
	for(i = 1; i < (1<<scnt); i++)  // bit全探索
	{
		int S1 = 0, S2 = 0;  // S1とS2に振り分ける
		for(j = 0; j < scnt; j++)
		{
			if((i>>j)&1)
			{
				S1 |= (1<<sid[j]);
			}
			else
			{
				S2 |= (1<<sid[j]);
			}
		}
		ll d1 = bitDP(S1, t-1);
		ll d2 = bitDP(S2, 1);
		d += d1*d2;
	}
	return d;
}

int main(void)
{
	int i, j;
	cin >> N >> T >> M;
	t_ng.resize(N, vector<bool>(N, false));
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		t_ng[a][b] = true;
		t_ng[b][a] = true;
	}
	for(i = 0; i < 1<<N; i++)
	{
		for(j = 0; j <= T; j++)
		{
			dp[i][j] = -1;  // 未探索
		}
	}

	ll ans = bitDP((1<<N)-1, T);
	// チーム番号の区別は無いため、T! だけ重複している
	for(i = 1; i <= T; i++)  ans /= i;
	cout << ans << endl;

	return 0;
}

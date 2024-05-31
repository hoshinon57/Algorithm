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

// ABC187 https://atcoder.jp/contests/abc187

/*
 * bitDPで解く。各種解説と同じ考え方だった。
 *   https://x.com/kyopro_friends/status/1345366167750205446
 *   https://atcoder.jp/contests/abc187/editorial/488
 *   https://drken1215.hatenablog.com/entry/2021/01/02/224300
 * 
 * dp[S]を
 *   Sのbitを頂点に対応させ、bitが1である頂点について見たときの求める値
 * と定義する。
 * dp[(1<<N)-1]が答。
 * 
 * 上記を再帰で解く。
 * Sが完全グラフなら1を返し、
 * そうでなければ部分集合Tについて、T∪T2=SとなるT,T2についてdp[T]+dp[T2]の最小値がdp[S]となる。
 * 
 * 「部分集合の部分集合」問題であり、計算量はO(3^N).
 * ただし自分のは再帰関数内で「Sが完全グラフか」を確認しているのでもっと多いか？
 * ※とはいえ、メモ化により各Sについて高々1回までしか計算しないので、
 *   事前計算する O(3^N+2^N*N^2) と同じになるか？
 * 
 * [補足]
 * ・入力例4だと手元環境で4.3secとかかかるが、ぐっと祈って出すと通る。
 * ・引数Sが完全グラフかを事前計算する実装も試したが、手元での実行時間は変わらなかった。
 * 
 * [ACまでの思考の流れ]
 * ・制約的にbitDPっぽい。
 */

int N;
vector<vector<bool>> edge;  // edge[i][j]:頂点i,j間に辺があればtrue
vector<int> dp;  // dp[S]

// dp[S]を返す
int bitDP(int S)
{
	int &d = dp[S];
	if(d != -1) return d;

	// Sが完全グラフかをチェック (ベースケース的な)
	int i, j;
	vector<int> v;
	for(i = 0; i < N; i++)
	{
		if((S>>i)&1) v.push_back(i);
	}
	bool ok = true;
	int sz = (int)v.size();
	for(i = 0; i < sz; i++)
	{
		for(j = i+1; j < sz; j++)
		{
			if(!edge[v[i]][v[j]]) ok = false;
		}
	}
	if(ok) return (d=1);

	// Sの部分集合についてチェック
	d = INF32;
	for(int T = S-1; T >= 0; T--)  // T=Sは見ない
	{
		T &= S;
		int T2 = S&(~T);  // T∪T2=S
		int tmp1 = bitDP(T);
		int tmp2 = bitDP(T2);
		chmin(d, tmp1+tmp2);
	}

	return d;
}

int main(void)
{
	int i;
	int M;
	cin >> N >> M;
	edge.resize(N, vector<bool>(N, false));
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		edge[a][b] = edge[b][a] = true;
	}
	dp.resize(1<<N, -1);

	cout << bitDP((1<<N)-1) << endl;

	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// bitDPのメモ、ライブラリ

/*
 * ★順列を全探索すれば解ける問題にて、計算量を削減したいときにbitDPが使える。
 *   https://atcoder.jp/contests/abc319/editorial/7117
 * 
 * [参考記事]
 * https://algo-logic.info/bit-dp/
 * 
 * [巡回セールスマン問題について]
 * dp[S][v]を以下のように定める。
 *   Sの各ビットをそれぞれの都市に割り当てて、
 *   1になっているビットに対して好きな順に辿ったときの、最小距離
 *   ただしスタート地点を0とし、最後にvに行く、という制約
 *   未探索であれば-1
 * 
 * 例えばS=b'1101, v=2であれば、
 *   頂点0,2,3を好きな順に辿ったときの、最小距離
 *   ただし最後に行くのは頂点2
 * となる。
 * 
 * dpの遷移式は以下となる。
 *   dp[S][v] = min(dp[S-{v}][u] + dist[u][v])
 *   ※ただしuは、S-{v}に含まれる頂点iそれぞれについて計算する
 * 頂点vを除いた集合の移動距離(終点u) + u->vの距離 という考え方。
 * 
 * 以下、原点に該当する頂点番号を0として、
 * ゴール地点は呼び出し側で制御するイメージ。
 *   原点に戻る場合：bitDP((all 1), 0) が答になる
 *   ゴールが自由：for(i) bitDP((all 1), i) の最小値が答になる感じかな (実績なし)
 * スタート地点は再帰内のベースケース内(S=0)の処理を制御するイメージ。
 *   原点スタートの場合：v=0のときdp=0. v!=0のときdp=INF.
 *   スタートが自由：vによらず、dp=0かな (実績なし)
 * 
 * [O(3^N)の"部分集合の部分集合"問題について]
 * dp[S][n]を「集合Sをn個のグループに分ける」とする場合、
 *   N=16:間に合わない (EDPC-U)
 *   N=15:間に合うが、枝刈りが必要 (典型90-45)
 * となる感じ。dp[S]で情報として十分かを考える必要がある。
 * 
 * またSの部分集合を全列挙したい場合、以下のテクニックがある。 (https://kyopro-friends.hatenablog.com/entry/2019/01/12/231106)
 *   for(T = S; T >= 0; T--)  // T:Sの部分集合
 *   {
 *     T &= S;
 *     int T2 = S & (~T);  // 必要に応じて使う T∪T2=S, T∩T2={}
 *     if(T == 0 || T2 == 0) continue;  // 必要に応じて 空集合を除外したい場合
 *     (処理)
 *   }
 * 
 * [ゲームのDPについて]
 * ある状態からの遷移を全て試して、
 *   「相手が負け」のケースが1つでもあれば、自分の勝ち。
 *   「相手が負け」のケースが1つも無ければ、相手の勝ち。
 * となる。
 * 実装イメージは以下。
 *   bool win = false;  // 自分が勝ちならtrue
 *   for( (全ての遷移) )
 *   {
 *     if(func(次の状態) = lose) win = true;  // この遷移で相手が負けるなら、自分はそれを選べばよいので、自分の勝ち
 *   }
 * 
 * [関連問題 / 巡回セールスマン問題]
 * AOJ DPL_2_A https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_2_A&lang=ja
 * ABC180-E
 * ABC190-E  スタートとゴールの考え方が、一般的な巡回セールスマン問題とは少し異なる
 * ABC274-E
 * ABC301-E
 * ABC338-F
 * 
 * [関連問題 / その他のbitDP]  ★再帰を使わない場合、もらうDPより配るDPの方が解きやすいかも？
 * ABC402-E 期待値DP&bitDP的な
 * ABC381-F
 * ABC354-E
 * ABC332-E
 * ABC318-D
 * ABC310-F 確率DP&bitDP的な
 * ABC310-D
 * ABC278-F
 * ABC232-F
 * ABC215-E 実装大変だった もらうDPで解いた
 * ABC199-E
 * ABC187-F 部分集合の部分集合
 * ABC142-E
 * ABC041-D トポロジカルソートの数え上げ
 * EDPC-O
 * EDPC-U 部分集合の部分集合
 * 典型90-45 部分集合の部分集合
 */

const int MAX_N = 15;  // 頂点数の最大
int dist[MAX_N][MAX_N];  // dist[i][j]:頂点i->jへの距離
// dp[S][v]:
//   Sの各ビットをそれぞれの都市に割り当てて、
//   1になっているビットに対して好きな順に辿ったときの、最小距離
//   ただし最後にvに行く、という制約
//   未探索であれば-1
int dp[1<<MAX_N][MAX_N];

// Sの各ビットをそれぞれの都市に割り当てて、
// 1になっているビットに対して好きな順に辿ったときの、最小距離を返す
// ただしスタート地点を0とし、最後にvに行く、という制約
// N:頂点数
// 呼び出し元での事前準備：
//   dist[][]に頂点間の距離を設定する
//   dp[][]を未探索の-1にする
int bitDP(int S, int v, int N)
{
	// メモ化再帰で解く
	if(dp[S][v] != -1) return dp[S][v];  // メモ化済み

	if(S == 0)  // ベースケース
	{
		// 頂点0をスタートとするので、dp[0][0]は0, dp[0][*]はありえないためINF.
		if(v == 0) dp[S][v] = 0;
		else dp[S][v] = INF32;

		return dp[S][v];
	}

	if(((S >> v) & 1) == 0) {dp[S][v] = INF32; return dp[S][v];}  // Sに頂点vが含まれていない
	int S_prev = S & (~(1<<v));  // vに相当するビットを落とす

	int &d = dp[S][v];
	d = INF32;
	for(int u = 0; u < N; u++)
	{
		// S_prev内の頂点uを決めて、u->vへの遷移を考える
		if(dist[u][v] == INF32) continue;  // u->vへの辺が無ければスルー
		
		// dp[S][v] = dp[S-{v}][u] + dist[u][v]
		d = min(d, bitDP(S_prev, u, N) + dist[u][v]);
	}
	return d;
}

int main(void)
{
	// 以下はAOJ DPL_2_Aを解く内容
	// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_2_A&lang=ja
	int i, j;
	int N, M; cin >> N >> M;
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			dist[i][j] = INF32;  // 辺で結ばれていない頂点間は距離をINFにする
		}
	}
	for(i = 0; i < M; i++)
	{
		int s, t, d; cin >> s >> t >> d;
		dist[s][t] = d;
	}
	// dpテーブル初期化
	for(i = 0; i < 1<<N; i++)
	{
		for(j = 0; j < N; j++)
		{
			dp[i][j] = -1;  // 未探索
		}
	}

	int ans = bitDP((1<<N)-1, 0, N);
	if(ans == INF32) ans = -1;
	cout << ans << endl;

	return 0;
}

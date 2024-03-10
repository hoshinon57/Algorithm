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

// 競プロ典型90問:83 https://atcoder.jp/contests/typical90/tasks/typical90_ce

/*
 * 平方分割で解く。
 * 
 * クエリにおける「隣接する頂点の色を塗り替える」を愚直に実装すると、
 * スター型の頂点にてTLEしてしまう。
 *   https://twitter.com/e869120/status/1411508237522767873
 * 
 * よって次数が多い頂点については愚直に塗り替えるのではなく、
 * 隣接頂点に対して「この色で塗り替えるから見に来てね」という形式にする。
 * ※前者はpush型、後者はpull型のイメージ
 * 
 * ただしpull型でも、逆にスター型グラフの中心頂点にとっては
 * 見に行くリストが多すぎてTLEしてしまう。
 * 
 * よって、
 *   次数がXより少ない頂点：push型
 *   次数がXより多い頂点：pull型
 * として処理を使い分ける。
 * ではXはどう決めるかというと、実装では「えいやっ」で√Mとした。
 * ※公式解説では√(2M)だった
 * 
 * 実装は以下のようにしている。
 *   col[i] = {クエリ番号, iの色}  頂点iの色
 *   pull[i] = {クエリ番号, iの色} 頂点iの色で、これは欲しい頂点がクエリ時に見に行く
 *   pull_list[i][*]  頂点iが見に行くpullリスト
 * 
 * [ACまでの思考の流れ / 考え方]
 * ・頂点の次数によっては愚直に色を配るとTLE.
 *   次数が少ない場合は良いとして、多い場合はどうしようか。
 *   ⇒相手から取りに来てもらうのが良いのでは。
 *   ABC335-F(Hop Sugoroku) より、次数に応じて処理を使い分ける / ボーダーは平方分割 を思いついた。
 * ・「複数の方法を考えておいて, 状況に応じて使い分ける」 という考え方。
 */

using Graph = vector<vector<int>>;
const int N_BRD = 500;  // おおよそ√N これ未満は配り、以上はpull

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	Graph g(N);
	for(i = 0; i < M; i++)
	{
		int a, b; cin >> a >> b;
		a--; b--;
		g[a].push_back(b);
		g[b].push_back(a);
	}
	vector<pair<int,int>> col(N), pull(N);  // {クエリ番号,色}
	vector<vector<int>> pull_list(N);
	for(i = 0; i < N; i++)
	{
		col[i] = {-1, 1};  // 色1
		pull[i] = {-1, 1};
		if(g[i].size() < N_BRD) continue;
		// 次数が多い頂点は、それと繋がっている頂点に対して「配らないから見に来てね」を登録する
		for(auto &e : g[i])
		{
			pull_list[e].push_back(i);  // 頂点eに対するクエリでは、まず頂点iの色を見に行く
		}
	}

	int Q; cin >> Q;
	for(i = 0; i < Q; i++)
	{
		int x, y; cin >> x >> y;
		x--;
		pair<int,int> c = col[x];
		// pullを見に行って、より新しいクエリ番号があれば更新
		for(auto &e : pull_list[x])
		{
			if(c.first < pull[e].first) c = pull[e];
		}
		cout << c.second << endl;
		col[x] = {i, y};

		// 次数が少なければ隣接頂点を直接更新、多ければ配信リスト
		if(g[x].size() < N_BRD)
		{
			for(auto &e : g[x])
			{
				col[e] = col[x];
			}
		}
		else
		{
			pull[x] = col[x];
		}
	}

	return 0;
}

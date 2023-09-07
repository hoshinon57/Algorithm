#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC203 https://atcoder.jp/contests/abc203

/*
 * 自力で解けず、解説を見た。
 * in-place DPで解く。
 * 
 * まず、黒ポーンを{x,y}の形で保持し、xで昇順ソートする。
 * あるx座標について白ポーンを置けるy座標をsetで管理することを考えると、
 * この集合の更新について、黒ポーンが無い行(x)については変化しないことが分かる。
 * つまり黒ポーンが存在する行のみについて考えればよい。
 * 
 * また、白ポーンを置けるy座標(myとする)をx座標ごとに個別に管理すると、
 * 1行更新辺りワーストでO(MlogM)かかってしまいそう。 ⇒ TLEになる
 * よって、ある行と次の行とでmyはほとんど変化しないことから、myを各行で使いまわすことで計算量を削減する。
 * 
 * 行ごとの遷移については、黒ポーンのy座標ごとに
 *   手前の行のy-1,y+1に白ポーンを置けるなら、myにyを追加
 *   手前の行のy-1,y+1に白ポーンを置けず、かつyに白ポーンを置けるなら、myからyを削除
 * とする。
 * ここで即座に追加/削除を行うと、myを使いまわしている関係でバグる。
 * 追加/削除すべき値を保持し、後で処理する。
 * ※以下のような単純なケースでNGとなる
 *   2 3
 *   1 3
 *   1 1
 *   1 2
 * 
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・操作の条件を踏まえて、X座標が同じものでまとめて計算する、という考え方。
 * ・setで管理する場合、前回⇒今回へ毎度コピーすると
 *   1回あたりワーストでO(MlogM)かかってしまうかも、これをM回行うとTLEするかも。というイメージを持つこと。
 * ・前回と今回でDPの差分が少ないなら、in-placeで計算を省く、という考え方。
 * ・in-placeの場合、追加/削除という相反する操作があれば
 *   まずフラグだけ立てておき後に処理、というひと手間が必要かを考える。
 */

int main(void)
{
	int i, j;
	int N, M; cin >> N >> M;
	vector<pair<int,int>> blk(M);
	for(i = 0; i < M; i++)
	{
		cin >> blk[i].first >> blk[i].second;
	}
	sort(blk.begin(), blk.end());  // xでソート
	set<int> my;  // 自ポーンが置けるy座標の集合
	my.insert(N);

	i = 0;
	while(i < M)
	{
		// 黒ポーンのX座標が同じものについて処理する
		for(j = i; j < M; j++)
		{
			if(blk[i].first != blk[j].first) break;
		}
		int end = j;  // X座標が同じ範囲は、[i,end)

		vector<int> ad, dl;  // 追加する分、削除する分
		for(j = i; j < end; j++)
		{
			int y = blk[j].second;  // 黒ポーンのy座標
			// ★ここでmyに追加/削除を行うと、行ごとにmyを使いまわしている関係で、バグる。
			//   追加/削除すべき値を保持し、後で処理する。
			if(my.count(y+1) || my.count(y-1)) ad.push_back(y);  // 手前のy±1どちらかに自ポーンを置けるなら、ここにも置ける
			else if(my.count(y)) dl.push_back(y);  // 手前のy±1ともに自ポーンを置けず(else)、かつyに自ポーンが置ける場合、ここには置けない
		}
		for(auto &e : ad) my.insert(e);
		for(auto &e : dl) my.erase(e);
		i = j;
	}
	cout << my.size() << endl;

	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC176 https://atcoder.jp/contests/abc176

/*
 * 自力で解けず、@kyopro_friends氏の解説を見た。
 * https://twitter.com/kyopro_friends/status/1297498257203978240
 * 
 * 【考えたこと】
 * ・横方向で最も爆破対象が多いところと、縦方向で最も爆破対象が多いところを組み合わせる。
 *   ただしその交点に対象があれば、重複カウントになるので-1必要。
 * ・よって、交点に対象が無いような組み合わせを探せばよい。
 *   そのような組み合わせが無ければ、-1したものが答。
 * ・ただし上記の計算量を考えたときに、
 *   「横方向で最も爆破対象が多いところと、縦方向で最も爆破対象が多いところの組み合わせ」
 *   にO(M^2)かかってしまい、TLEしそう。
 *   何か別アルゴリズムがある…？
 * ～～～ここで断念～～～
 * 
 * 【解法】
 * ・実は、上記を全探索して問題なかった。
 * ・知りたいのは「交点に対象が無い組み合わせが有るか無いか」となる。
 *   「横方向で最も爆破対象が多いところと、縦方向で最も爆破対象が多いところの組み合わせ」を単純に2重ループさせたときに、
 *   (M+1)回目までには必ず「交点に対象が無い組み合わせ」が出現する。
 *   ※出現しない場合、2重ループがM回以内に終了するので、これもまた制限時間に間に合う。
 * 
 * 以下URLにあった考え方も覚えておく。
 * https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2020/0823_abc176
 * ＞探索候補は多いけど、1つでも条件に当てはまるものが見つかったらよくて、条件に当てはまらないものは限られている
 */
int main(void)
{
	// 0-indexed
	int i;
	int H, W, M;
	cin >> H >> W >> M;
	vector<int> H_count(H), W_count(W);  // 行ごと/列ごとの対象の数
	int h_max = -1, w_max = -1;  // 行ごと/列ごとの対象の最大数
	set<pair<int,int>> bomb_list;  // 爆弾があるマス{h,w}を登録
	for(i = 0; i < M; i++)
	{
		int h, w;
		cin >> h >> w;
		h--; w--;  // 0-indexedに変換
		H_count[h]++;
		h_max = max(h_max, H_count[h]);
		W_count[w]++;
		w_max = max(w_max, W_count[w]);
		bomb_list.insert({h, w});
	}

	vector<int> h_max_list, w_max_list;  // h_maxとなる行を登録 (w側も同様)
	for(i = 0; i < H; i++)
	{
		if(H_count[i] == h_max) h_max_list.push_back(i);
	}
	for(i = 0; i < W; i++)
	{
		if(W_count[i] == w_max) w_max_list.push_back(i);
	}

	auto calc = [&](void) -> int
	{
		// 「交点に対象が無い組み合わせ」が存在するか、全探索する。
		// この2重ループの処理回数は最大でもM.
		for(auto &h : h_max_list)
		{
			for(auto &w : w_max_list)
			{
				if(bomb_list.count({h, w}) == 0)
				{
					// 「交点に対象が無い組み合わせ」が見つかった
					return h_max + w_max;
				}
			}
		}

		// 「交点に対象が無い組み合わせ」が見つからなかった
		// -1したものが答となる
		return h_max + w_max - 1;
	};
	cout << calc() << endl;

	return 0;
}

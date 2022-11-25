#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 100000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題88
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2008ho/tasks/joi2008ho_a

/*
 * 碁石を1つずつ配列に保持する方法では、
 * 入力で同じ色の石が連続するような偏ったケースでO(N^2)となりTLE.
 * 
 * よって、データの持ち方として
 *   {碁石の色, その連続した数}
 * のpairを配列で持つ形を考える。
 * 例えば碁石が順に 1,1,0,0,1,1,1,1 であれば、 {1,2},{0,2},{1,4}となる。
 * 
 * 次に、石を指定個数置く関数put()を用意する。
 * 置く際に、末尾と同じ色であれば個数を増やす。 例：{1,2}->{1,3}
 * 違う色であれば、要素を末尾に追加する。 例：{1,2}->{1,2},{0,1}
 * 
 * (1)奇数ターンの場合
 *   put()を用いて1個置く。
 *   末尾と同じ色か違う色かは気にせず、put()が適切に処理する。
 * (2)偶数ターンの場合
 *   末尾と同じ色なら、put()で1個置く。
 *   違う色なら、末尾の要素を色をひっくり返した上で、1個置く。
 *   ->末尾の要素をまず消して、反転した色を置く。
 *     これにより、そのさらに手前の石と合体できる。
 * 
 * 以上は先頭に番兵を用意することで、処理を簡易化できる。
 * 
 * 計算量はO(N).
 * N回のターンがあり、各ターンごとO(1)で処理できるため。
 */

int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<pair<int,int>> stone;  // {石の色, その連続した数} を配列で持つ
	stone.push_back({-1, 1});  // 番兵として、先頭に白黒どちらでもない色の石を置く

	// 石をnum個置く (置くだけで、ひっくり返す処理はしない)
	auto put = [&](int color, int num) -> void
	{
		int idx = (int)stone.size()-1;

		// 末尾と同じ色なら、個数を増やす
		// 末尾と異なる色なら、要素を末尾に追加する
		if(stone[idx].first == color)
			stone[idx].second += num;
		else
			stone.push_back({color, num});
	};

	for(i = 1; i <= N; i++)
	{
		int st;
		cin >> st;
		if(i%2 == 1)  // 単に置くだけのターン
		{
			put(st, 1);  // 1個置く
		}
		else  // ひっくり返せるターン
		{
			int idx = stone.size()-1;
			if(stone[idx].first == st)  // 末尾と同じ色なら、置くだけ
			{
				put(st, 1);
			}
			else  // 末尾と違う色なら、ひっくり返した上で、1個置く
			{
				// 末尾の石を消した上で、反転した色を置く
				// そうすることで、そのさらに手前の石と合体できる
				// 
				// 対象が番兵であってもそのまま処理してOK.
				// なぜなら、"1-col"により -1->+2->-1->+2->... と白黒以外の色に結局なるため
				int col = stone[idx].first;
				int num = stone[idx].second;
				stone.pop_back();
				put(1-col, num);

				put(st, 1);  // 1個置く
			}
		}
	}

	int answer = 0;
	for(auto &e : stone)  // 白の数を数える
	{
		// cout << e.first << ":" << e.second << endl;  // debug print
		if(e.first == 0)
			answer += e.second;
	}
	cout << answer << endl;

	return 0;
}

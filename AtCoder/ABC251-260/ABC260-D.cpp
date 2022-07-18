#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC260 https://atcoder.jp/contests/abc260

/*
 * コンテスト中に解ききれず、解説を見て実装した。
 * 表向きになっているカードをsetで管理して、後はゴリゴリ実装する。
 * 参考：https://twitter.com/kyopro_friends/status/1548664994094804992
 * 
 * 表向きになっているカードをsetで管理する。
 * これにより、カードを引いたときに重ねるか、重ねないで新規の山に置くか、が二分探索で判定できる。
 * 
 * また、以下の情報を管理する。
 *   ・(表向きになっている)i番目の山のカードリスト
 *   ・これまで作った山の数
 *   ・数値iが書かれたカードは、何番目の山に積まれたか(逆引き用)
 * 
 * 引いたカードより大きい数値のカードが、場に無い場合：
 *   カードを新規で山に置く。
 * 引いたカードより大きい数値のカードが、場にある場合：
 *   そのうち最小のカードの上に置く。
 *   カードを重ねるので、最小のカードはsetから削除する。
 * 
 * 山に置いたとき、カードの枚数がK枚となった場合：
 *   K枚重なったので「食べる」。
 *   turn[N]を用意しておき、現在のターン数を設定すればよい。
 *   (今引いたカードも食べるので、setには加えない)
 * 山に置いたとき、カードの枚数がK枚未満の場合：
 *   今引いたカードをsetに加える。
 * 
 * 以上をゴリゴリと実装すればよい。
 */
int main(void)
{
	// 1-indexed
	int i;
	int N, K;
	cin >> N >> K;
	vector<int> p(N+1);
	for(i = 1; i <= N; i++)
	{
		cin >> p[i];
	}

	vector<vector<int>> card_list(N);  // 0-indexed. card_list[i]:i番目の山のカードリスト
	int yama_id = 0;  // 引いたカードを重ねず新規で場に置く際、yama_id番目の山となる
	vector<int> id_index(N+1);  // id_index[i]:数値iが書かれたカードは、何番目のidの山に積まれたか いわゆる逆引き
	vector<int> turn(N+1, -1);  // turn[i]:数値iが書かれたカードが何ターン目に食べられるか

	set<int> s;	 // 山にあるカードのうち、見えている数値の一覧
	for(i = 1; i <= N; i++)
	{
		auto it = s.lower_bound(p[i]);
		int id;
		if(it == s.end())
		{
			// 引いたカードより大きい数値のカードが、場に無い
			// ->カードをどの山にも重ねず、新規で山に置く
			id = yama_id;
			card_list[id].push_back(p[i]);  // 新規の山のカードリストに追加
			id_index[p[i]] = id;  // 逆引き用
			yama_id++;
		}
		else
		{
			// 引いたカードより大きい数値のカードが、場にある
			// ->そのうち最小のカードの上に重ねる
			id = id_index[*it];
			card_list[id].push_back(p[i]);  // 既存の山のカードリストに追加
			id_index[p[i]] = id;  // 逆引き用
			s.erase(it);  // カードを重ねるので*itは見えなくなる。よって削除
		}

		if(card_list[id].size() == K)
		{
			// K枚重なったので食べる
			for(auto &e : card_list[id])
			{
				turn[e] = i;  // 食べられたのはiターン目
			}
		}
		else
		{
			// 食べないので、sに追加
			s.insert(p[i]);
		}
	}

	for(i = 1; i <= N; i++)
	{
		cout << turn[i] << endl;
	}

	return 0;
}

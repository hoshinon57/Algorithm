#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題94
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1149&lang=jp

/*
 * カットされたケーキの一覧を、{幅w, 高さd}の要素を持つcake_list[]に追加していく。
 * このとき、カットにより無くなったケーキは要素を削除せず、w=-1として除外する。
 * 識別番号pのケーキをカットする際は、cake_list[]にてw≠-1の要素を先頭から探していく。
 * ⇒N回のカットで追加される要素は高々2*Nであり、Nは最大で100回のため、十分間に合う。
 * 
 * カットしたいケーキのサイズを{cw,cd}, カット位置をsとすると、
 * 4つあるカット辺のうち上下と左右は同一視できる。
 * s = s%(cw+cd) とすることで、カット辺と上と右の2つに限定でき、処理を簡易にできる。
 * 
 * 出力は、cake_listからw≠-1を除外したものの面積を列挙し、
 * それを小さい方から出力していけばよい。
 * 
 * 計算量は、カット部分がO(N^2), 出力時のソート部分がO(NlogN)であり、
 * O(N^2)になるかと考える。
 */

int main(void)
{
	int i;
	int n, w, d;
	while(true)
	{
		cin >> n >> w >> d;
		if(w == 0) break;

		// カットされたケーキの一覧 {w,d}を「先に生まれた」順に列挙
		// カットで無くなったものはw=-1とする
		vector<pair<int,int>> cake_list;
		cake_list.push_back({w, d});  // 初期値

		for(i = 0; i < n; i++)  // i回目のカット
		{
			int p, s;
			cin >> p >> s;

			// 識別番号pのケーキを探す
			// -> cake_list[]にて w≠-1のものをカウントしていく
			int cw, cd;  // カットするケーキのw,d
			for(auto &e : cake_list)
			{
				if(e.first != -1) p--;
				if(p == 0)
				{
					cw = e.first;
					cd = e.second;
					e.first = -1;  // このケーキはカットにより無くなる
					break;
				}
			}

			// 要素追加は範囲forの外で行う。"イテレータ破壊"でしばらくバグに悩んだ。
			// https://qiita.com/izmktr/items/0c95aff5ba7554afcaa7
			// https://dixq.net/forum/viewtopic.php?t=11823

			// {cw,cd}サイズのケーキを位置sでカット
			s = s%(cw + cd);  // 下と左の辺は、それぞれ上と右の辺に置き換えて良い
			if(s < cw)  // 上の辺を切る
			{
				// 長さはsとcw-sになる
				cake_list.push_back({min(cw-s, s), cd});  // 面積が小さい方が先
				cake_list.push_back({max(cw-s, s), cd});
			}
			else  // 右の辺を切る
			{
				s -= cw;
				// 長さはsとcd-sになる
				cake_list.push_back({cw, min(cd-s, s)});
				cake_list.push_back({cw, max(cd-s, s)});
			}
		}

		// 出力
		vector<int> answer;
		// まずはcake_listにて w≠-1 の要素を探し、面積をanswerに入れていく
		for(auto &e : cake_list)
		{
			if(e.first == -1) continue;
			answer.push_back(e.first*e.second);
		}
		// 面積を小さい順に出力
		sort(answer.begin(), answer.end());
		for(i = 0; i < (int)answer.size(); i++)
		{
			cout << answer[i];
			if(i != (int)answer.size()-1) cout << " ";  // AOJは末尾に空白があると"Presentation Error"だった
		}
		cout << endl;
	}

	return 0;
}

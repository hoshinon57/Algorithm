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

// 「分野別 初中級者が解くべき過去問精選 100問」の問題81
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/joi2011ho/tasks/joi2011ho1

/*
 * イベントソートで解く。
 * 参考：https://qiita.com/pata_/items/e7cfdbbb4414cf13e715
 * 
 * 以下、絵の具の濃さを"時刻"と表記する。
 * pair{時刻, イベント} のイベント配列を用意し、入力a,bに対して
 *   {a,   0}  // a<=x<=bの線分に入るタイミング
 *   {b+1, 1}  // 線分から出るタイミング
 * とする。
 * これをソートすると、pairの第1要素、つまり時刻でソートされる。
 * 
 * その後、このイベントを先頭から見ていって、
 *   イベントが0ならカウントをインクリメント
 *   1ならデクリメント
 * を行っていき、カウントの最大値が答となる。
 * 
 * ただし同時刻にイベント0,1が発生すると、先にインクリメントされるので、
 * 最大値の更新は「時刻が前回要素から変化したとき」とする。
 * ※{t=10, 0}, {t=10, 1} の2要素があったときに、トータルでは±0だが、
 *   先にインクリメントすることで、誤って最大値の更新がされてしまうことを防ぐため。
 * ※もしくは、イベントの種別を0,1逆にすれば、先にデクリメントされるので、
 *   都度最大値チェックを行っても正しく計算できる。
 * 
 * 計算量はソート部分でO(NlogN).
 */

int main(void)
{
	int i;
	int N;
	cin >> N;
	vector<pair<int,int>> event;  // {time, event} event=0:a, 1:b
	for(i = 0; i < N; i++)
	{
		int a, b;
		cin >> a >> b;
		event.push_back({a,   0});
		event.push_back({b+1, 1});  // 買うのはa<=x<=bのため、b+1で落とす
	}
	sort(event.begin(), event.end());  // ソートすることで、pairの第1要素である時刻でソートされる

	int count = 0, count_max = 0;
	int prev_time = -1;  // 前回のeventのtime.変化したときにcount_maxを更新する
	for(auto &e : event)
	{
		// 同時に「買う」「買わない」のイベントが発生した場合を踏まえ、
		// count_maxの更新は時刻が前回要素から変化したときに行う
		if(prev_time != e.first)
		{
			count_max = max(count_max, count);
		}

		if(e.second == 0)  // a ここからは買う
		{
			count++;
		}
		else  // b ここからは買わない
		{
			count--;
		}
		prev_time = e.first;
	}
	cout << count_max << endl;

	return 0;
}

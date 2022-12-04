#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題82
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=2013

/*
 * イベントソートで解く。
 * 
 * pair{時刻, イベント} のイベント配列を用意し、発車時刻をイベント=0, 到着時刻をイベント=1とする。
 * これをソートすると、pairの第1要素、つまり時刻でソートされる。
 * 
 * あとはこのイベントを先頭から見ていって、
 *   イベント=0ならカウンタをインクリメント
 *   イベント=1ならデクリメント
 * を行っていき、カウンタの最大値が答となる。
 * 
 * 計算量はソート部分でO(nlogn).
 */

int main(void)
{
	int i;
	int n;
	vector<pair<int,int>> ev;  // {time, event(0:発車, 1:到着)}

	auto input = [&](void) -> void
	{
		for(i = 0; i < n; i++)		
		{
			for(int j = 0; j < 2; j++)  // j=0:発車, 1:到着
			{
				// 時刻12:34:56を123456に変換する
				string str;
				cin >> str;
				int num = stoi(str.substr(0, 2) + str.substr(3, 2) + str.substr(6, 2));
				ev.push_back({num, j});
			}
		}
		sort(ev.begin(), ev.end());  // 時刻順にソート
	};
	
	while(true)
	{
		cin >> n;
		if(n == 0) break;
		ev.clear();

		input();

		int answer = 0;
		int count = 0;
		int prev_time = -1;
		for(auto &e : ev)
		{
			// 前回のイベントから時刻が変化したら、ここまでのcountをanswerへ
			if(e.first != prev_time)
			{
				answer = max(answer, count);
				prev_time = e.first;
			}

			if(e.second == 0) count++;  // 発車
			else count--;  // 到着
		}
		// answerは時刻変化時にのみ更新のため、最後の分は別途更新してあげる必要がある
		// (もしくは、イベントの末尾に番兵を入れる案もある)
		answer = max(answer, count);

		cout << answer << endl;
	}

	return 0;
}

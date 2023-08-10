#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC195 https://atcoder.jp/contests/abc195

/*
 * 貪欲法(と思われる)で解く。
 * 
 * 公式解説の以下方針で実装した。
 * ＞大きさの小さな箱から順に、その箱に入れることができる価値最大の荷物を入れる
 * 
 * クエリごとに以下を実施する。
 * (1)使える箱を集めて、大きさで昇順ソート
 * (2)箱を先頭から見ていって、箱iごとに
 *   (2-1)箱iのサイズ以下となる荷物について、優先度付きキューに価値をpushしていく
 *   (2-2)キューから1つ取り出して答に加算
 *    
 * ※価値最大となる荷物を取得するために優先度付きキューを使ったが、
 *   使わなくても問題なく解けるみたい。
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int N, M, Q; cin >> N >> M >> Q;
	vector<pair<int,int>> lug(N);  // 荷物の{大きさW, 価値V}
	for(i = 0; i < N; i++)
	{
		cin >> lug[i].first >> lug[i].second;
	}
	sort(lug.begin(), lug.end());  // 大きさ順にソート
	vector<int> box_org(M); for(i = 0; i < M; i++) {cin >> box_org[i];}

	while(Q > 0)
	{
		int l, r; cin >> l >> r;
		l--; r--;
		vector<int> box;  // 本クエリにて使える箱の、大きさ一覧
		for(i = 0; i < M; i++)
		{
			if(l <= i && i <= r) continue;  // 使えない箱
			box.push_back(box_org[i]);
		}
		sort(box.begin(), box.end());

		priority_queue<int> que;  // 大きい方が優先度が高い
		int lug_idx = 0;
		int ans = 0;
		for(i = 0; i < (int)box.size(); i++)
		{
			// 箱iごとに、その箱iに入れられる荷物の価値をキューに積んでいく
			for(j = lug_idx; j < N; j++)  // 同じ荷物を2度キューに積まないよう、idx管理
			{
				if(box[i] >= lug[j].first)
				{
					que.push(lug[j].second);
				}
				else break;
			}
			lug_idx = j;

			// キューから1つ取り出す
			// 価値最大のものが取得できる
			if(!que.empty())
			{
				ans += que.top();
				que.pop();
			}
		}
		cout << ans << endl;
		Q--;
	}
	return 0;
}

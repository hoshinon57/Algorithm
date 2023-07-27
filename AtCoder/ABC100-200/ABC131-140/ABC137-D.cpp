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

// ABC137 https://atcoder.jp/contests/abc137

/*
 * 後ろ(終了日の方/制約が厳しい方)から決めていく。
 * 
 * 例えばM-1日目に実施できるアルバイトは、A=1日後に報酬がもらえるもののみ。
 * M-2日目に実施できるアルバイトは、A=1,2日後に報酬がもらえるもののみ。
 * 
 * よって、M-i日目について、
 *   A=i日後に報酬がもらえるアルバイトを、新たにバイトリストに追加し、
 *   この日はリスト内の報酬最大のものを選んでアルバイトする。
 * とすればよい。
 * priority_queueを使えば実現できる。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<vector<int>> work(100005);  // work[x]:x日後に報酬が得られるバイト一覧
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		work[a].push_back(b);
	}

	priority_queue<int> queue;  // 報酬について降順ソート
	ll ans = 0;
	for(i = 1; i <= M; i++)  // M日後まで、残りi日
	{
		for(auto &e : work[i])  // 新たにできるアルバイトをキューに追加する
		{
			queue.push(e);
		}
		if(queue.empty()) continue;  // この日、可能なアルバイトが無い

		// 1つキューから取り出して、それをアルバイトする
		ans += queue.top();
		queue.pop();
	}
	cout << ans << endl;

	return 0;
}

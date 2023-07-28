#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC146 https://atcoder.jp/contests/abc146

/*
 * 後ろから順に(貪欲に)見ていく。
 * 
 * あるマスiを見たときに、ここに来るための最善の出目xは、
 *   マス(i-x)がゲームオーバーマスでないもののうち、xが最大のもの
 * と判断してよい。
 * よってマスNから順に、大きい出目を出しながら手前に遷移していく。
 * ※「後ろから」「なるべく大きい出目」とすることで、先頭から見ると辞書順で最小になる、という考え方。
 * 
 * このときに出目を保持していき、最後に反転させたものが答となる。
 * 
 * 解説を見ると多いのはセグ木だったが、以下の解説が自分の実装と近かった。
 * https://ikatakos.com/pot/programming_algorithm/contest_history/atcoder/2019/1124_abc146
 * 
 */
int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	string s; cin >> s;

	int cur = N;
	vector<int> ans;
	while(cur > 0)
	{
		int prev = cur;
		for(i = M; i > 0; i--)  // 出目i
		{
			// マスcur-iから出目iにて、マスcurへ移動する
			int nxt = cur-i;
			if(nxt < 0) continue;
			if(s[nxt] == '1') continue;  // 行けないマス
			cur = nxt;
			ans.push_back(i);
			break;
		}
		if(prev == cur)  // 移動できなかった
		{
			cout << -1 << endl;
			return 0;
		}
	}
	reverse(ans.begin(), ans.end());
	for(auto &e : ans) cout << e << " ";
	cout << endl;

	return 0;
}

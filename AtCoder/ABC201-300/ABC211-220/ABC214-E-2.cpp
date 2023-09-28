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

// ABC214 https://atcoder.jp/contests/abc214

/*
 * 公式解説(YouTube)をほぼそのまま引用した形。（自分の実装よりもずっとスマート…）
 * https://www.youtube.com/watch?v=RCB7EZaddEQ
 * 
 * 基本はソース内のコメントを参照。
 * xを「次に入れる箱の番号」として、lの手前までボールを処理していく形。
 * 
 * ※もちろんACはするものの、
 *   今の自分ではこの実装は思いつかず、また細かい部分で「なぜこれで上手く動作するの？」が理解あやしい…
 *   将来見返したときに理解できるよう、精進していく。
 */

bool solve(void)
{
	int i;
	int N; cin >> N;
	vector<pair<ll,ll>> ev;
	for(i = 0; i < N; i++)
	{
		ll L, R; cin >> L >> R;
		ev.push_back({L, R});
	}
	ev.push_back({INF32, INF32});  // 末尾のボールを処理する用
	sort(ev.begin(), ev.end());

	priority_queue<ll, vector<ll>, greater<ll>> p_que;
	ll x = 1;  // 次に入れる箱の番号
	for(auto [l, r] : ev)
	{
		// lの手前まで、ボールを処理する
		while(x < l)
		{
			if(p_que.empty()) break;
			if(p_que.top() < x) return false;  // キューの先頭のボールが、R以下の箱に入れられなかった
			p_que.pop();
			x++;
		}
		x = l;  // 箱に余裕があっても、次は番号lから入れていく

		// キューに積むのはこのタイミングにて
		// xがl以上にならないと、箱には入れられないため
		p_que.push(r);
	}

	// p_queに番兵分は残るのは意図通り
	return true;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		YesNo(solve());
		T--;
	}

	return 0;
}

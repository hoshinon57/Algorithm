#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC333 https://atcoder.jp/contests/abc333

/*
 * シミュレーションで解く。
 * コンテスト時と考え方は同じだが、解説を見てよりスマートな実装にしなおした。
 *   https://www.youtube.com/watch?v=XGS11Qc1X7Y
 *   https://atcoder.jp/contests/abc333/submissions/48599329  上記YouTube解説の提出コード
 *   https://atcoder.jp/contests/abc333/editorial/7981
 * 
 * イベントの後ろから見ていく。
 * 登場したモンスターの種類をカウントしておき、
 * ポーションを見つけたら対応するモンスターと出会ってれば取得、そうでなければ無視する。
 * 最終的にモンスターが残っていればNG.
 * 
 * 出力すべきKminについては、「まだ倒していないモンスターの数」の最大値となる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> ev;
	for(i = 0; i < N; i++)
	{
		int t, x; cin >> t >> x;
		if(t == 1) ev.push_back(x);  // ポーション
		else ev.push_back(-x);  // モンスターは-1して区別する
	}
	reverse(ev.begin(), ev.end());  // 後ろから見ていく

	vector<int> cnt(N+1);  // cnt[i]:まだ倒していないi種類目のモンスターの数
	vector<int> ans;  // ポーションを取る/取らない
	int mx = 0, now = 0;  // now:現在、出会ったがまだ倒していないモンスターの数
	for(auto &e : ev)
	{
		if(e > 0)  // ポーション
		{
			if(cnt[e] > 0)  // 対象モンスターがいるなら、ポーションを取る
			{
				cnt[e]--;
				now--;
				ans.push_back(1);
			}
			else ans.push_back(0);
		}
		else  // モンスター
		{
			cnt[-e]++;
			now++;
		}
		mx = max(mx, now);
	}
	if(now > 0)
	{
		cout << -1 << endl;
		return 0;
	}
	cout << mx << endl;
	reverse(ans.begin(), ans.end());
	for(auto &e : ans) cout << e << " ";
	cout << endl;

	return 0;
}

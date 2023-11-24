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

// ABC221 https://atcoder.jp/contests/abc221

/*
 * イベントソートで解く。公式解説と同じ考え方。
 *   https://atcoder.jp/contests/abc221/editorial/2722
 * 日程が最大10^9のため、(座標圧縮などしないと)いもす法は使えない。
 * 
 * 入力a,b,cに対して、
 *   a日目にプレイヤー数が+1
 *   a+b日目にプレイヤー数が-1
 * となるようなイベントを考える。
 * 
 * これを日でソートして、小さい方の要素から順に処理していく。
 * 要素を順に見ていき、
 *   日が前回から変化したら、前回の日からの期間ぶん ans[(プレイヤー数)] を加算する
 *   プレイヤー数を増減する
 * を繰り返していく。
 * 
 * 最後はプレイヤー数=0で終わることから、
 * 末尾に番兵を入れて個別に判定…といったのは不要。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<pair<int,int>> ev;  // {日付, 増減}
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		ev.push_back({a, 1});
		ev.push_back({a+b, -1});
	}
	sort(ev.begin(), ev.end());

	vector<int> ans(N+1);
	int nowNum = 0, nowDay = 0;
	for(auto &[d, n] : ev)
	{
		if(d != nowDay)  // 日が変化したら、直前の日からの期間ぶん ans[(プレイヤー数)] を足す
		{
			ans[nowNum] += (d-nowDay);
			nowDay = d;
		}
		nowNum += n;
	}

	for(i = 1; i <= N; i++) { cout << ans[i] << " "; }
	cout << endl;

	return 0;
}

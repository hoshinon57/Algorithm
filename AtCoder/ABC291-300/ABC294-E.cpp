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

// ABC294 https://atcoder.jp/contests/abc294

/*
 * ブロック単位で左から順に見ていく。
 * evima氏の解説が近い。
 *   https://atcoder.jp/contests/abc294/editorial/6008
 *   https://www.youtube.com/watch?v=GI1ByCyJQ0M
 * 
 * 2行のうち「進みが遅い方」を進める。
 *   A:進みが遅い方
 *   B:進みが早い方
 * と以下記載する。
 * Bの最後の値を保持しておき、Aの進めたぶんの値と一致すれば、
 * 重複する数は min(Bの今の終端, Aの進めた部分までの終端)-Aの今の終端 となる。
 */
int main(void)
{
	int i, j;
	ll L, N[2]; cin >> L >> N[0] >> N[1];
	vector<vector<ll>> v(2), l(2);  // v[2][]
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < N[i]; j++)
		{
			ll v_, l_; cin >> v_ >> l_;
			v[i].push_back(v_);
			l[i].push_back(l_);
		}
	}

	ll idx[2] = {0, 0};  // 次に処理するブロックの番号
	ll pos[2] = {0, 0};  // 先頭から何個処理したか(=次は何個目から処理するか)
	ll last_num[2] = {-1, -2};  // 最後の値
	ll ans = 0;
	while(true)
	{
		if(pos[0] == L && pos[1] == L) break;

		// 遅れている方を進める
		int t;  // target
		if(pos[0] < pos[1]) t = 0;
		else t = 1;

		// 遅れている方の次の値が、もう片方の最後の値と等しいか
		if(v[t][idx[t]] == last_num[1-t])
		{
			// 重複部分を計算して答に加算
			ll r = min(pos[1-t], pos[t]+l[t][idx[t]]);  // 相手と自分、手前の方まで
			ans += (r - pos[t]);
		}
		// もろもろ更新
		last_num[t] = v[t][idx[t]];
		pos[t] += l[t][idx[t]];
		idx[t]++;
	}
	cout << ans << endl;

	return 0;
}

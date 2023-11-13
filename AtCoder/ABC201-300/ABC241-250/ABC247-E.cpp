#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC247 https://atcoder.jp/contests/abc247

/*
 * 自力で解けず、解説を見た。
 * 尺取り法で解く。
 *   https://atcoder.jp/contests/abc247/editorial/3736
 *   https://twitter.com/kyopro_friends/status/1513152479214850052
 * 
 * 尺取り法で考えようにも、Xより大きくYより小さい値がジャマとなる。
 * まず部分問題に分けて、数列Aを
 *   要素がY以上X以下である連続部分列
 * に分割する。
 * 次にこの連続部分列について
 *   要素にXとYがともに含まれる連続部分列 [l,r)
 * を求めると、rより右の要素は全て条件を満たすので、 (部分列の要素数)-r+1 が答に加算できる。
 * これはlを1つずつずらしつつ、rについて尺取り法のように実装すればよい。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・尺取り法で考えようにも、Xより大きくYより小さい値がジャマとなる。
 * ・ジャマならば、その値を除外して考えられないか。
 *   ⇒部分問題に分割する。
 * ・要素が全て「Y以上X以下」であれば、尺取り法で求められそう。
 */

ll N, X, Y;
ll ans = 0;

// arr:要素がY以上X以下である連続部分列
// arrについて問題の条件を満たす部分列を、尺取り法を用いて計算する
void check(vector<ll> &arr)
{
	ll sz = (int)arr.size();
	int l;
	int r = 0;
	map<ll,ll> mp;  // {値,その出現回数}
	for(l = 0; l < sz; l++)
	{
		while((mp[X] == 0 || mp[Y] == 0) && (r < sz) )
		{
			mp[arr[r]]++;
			r++;
		}
		if(mp[X] > 0 && mp[Y] > 0)
		{
			// 半開区間[l,r)にて、最大値がXかつ最小値がYになる
			ans += sz - r + 1;
		}
		else break;  // rが右端まで来ても条件を満たさないなら終了
		mp[arr[l]]--;
	}
}

int main(void)
{
	int i;
	cin >> N >> X >> Y;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	a.push_back(INF64);  // 番兵

	vector<ll> arr;
	for(auto &e : a)
	{
		// 要素がY以上X以下である連続部分列に分割する
		if(e > X || e < Y)
		{
			// 範囲外の値がきたら、ここまでの連続部分列で計算
			check(arr);
			arr.clear();
		}
		else
		{
			arr.push_back(e);
		}
	}
	cout << ans << endl;

	return 0;
}

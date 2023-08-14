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

// ABC160 https://atcoder.jp/contests/abc160

/*
 * まず、赤りんごを美味しい順に並べたとき、X個目より後を選ぶ必要は無い。
 * 同様に緑リンゴもY個目より後は不要。
 * 
 * X+Y個のりんごをまとめて降順ソートしたものを考えてみる。
 * 例として {10, 5, 3, 1}.
 * ここに白りんご {20, 4, 1} を追加することを考えると、
 *   20：美味しさ1と交換  1のりんごは捨てる
 *   4：美味しさ3と交換  3のりんごは捨てる
 *   1：交換しない
 * という順に操作するのが良さそう。
 * (このとき、捨てたりんごの色を白りんごに塗ればよい)
 * つまり白りんごもリストに追加して降順ソートし、上からX+Y個の総和が答になる。
 */

int main(void)
{
	int i;
	int x, y; cin >> x >> y;
	int a, b, c; cin >> a >> b >> c;

	vector<ll> p(a); for(i = 0; i < a; i++) {cin >> p[i];}
	vector<ll> q(b); for(i = 0; i < b; i++) {cin >> q[i];}
	sort(p.begin(), p.end(), greater<ll>());
	sort(q.begin(), q.end(), greater<ll>());
	vector<ll> mix;
	// 赤の大きい方からX個、緑の大きい方からY個、白全て、を一緒くたにまとめて降順ソート
	for(i = 0; i < x; i++) mix.push_back(p[i]);
	for(i = 0; i < y; i++) mix.push_back(q[i]);
	for(i = 0; i < c; i++)
	{
		int tmp; cin >> tmp;
		mix.push_back(tmp);
	}
	sort(mix.begin(), mix.end(), greater<ll>());

	// 上からX+Y個の総和が答
	ll ans = 0;
	for(i = 0; i < x+y; i++)
	{
		ans += mix[i];
	}
	cout << ans << endl;

	return 0;
}

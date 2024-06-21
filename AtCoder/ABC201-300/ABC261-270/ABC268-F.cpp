#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC268 https://atcoder.jp/contests/abc268

/*
 * 自力で解けず、解説を見た。
 *   https://atcoder.jp/contests/abc268/editorial/4788
 *   https://scrapbox.io/procon-kirokuyou/%E3%83%A6%E3%83%8B%E3%83%BC%E3%82%AF%E3%83%93%E3%82%B8%E3%83%A7%E3%83%B3%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%82%B3%E3%83%B3%E3%83%86%E3%82%B9%E3%83%882022_%E5%A4%8F_(AtCoder_Beginner_Contest_268)_F_-_Best_Concatenation_(500)
 *   https://x.com/kyopro_friends/status/1568600133771853826
 * 
 * 文字列Si,Sjの2つがあったとき、どちらを先に配置するかを考える。
 * Sに含まれる数値の和をT, 記号Xの数をXとすると
 *   (1)SiSjの順：Xi*Tj
 *   (2)SjSiの順：Xj*Ti
 * が並び順によって変わるスコアとなる。
 * ※ここで、Si単体で得られるスコアは大小比較に無関係のため無視している
 * これはSi,Sjの間に他のSが入っても影響は無いため、(1)(2)の大小関係とSi,Sjの前後関係は常に一致する。
 * つまりSの全要素を上記の判定でソートしたものが、スコア最大値のときの並び順となる。
 * 
 * 実装では、S単体で得られるスコアを事前にansに加算しておくことで、処理を簡易にしている。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・2要素の前後関係は、実は簡単な判定で一意に決まる。これに気付けるかどうか。
 *   DPの前処理として「最適な順番へ事前にソートする」という考え方がある。
 *     ex:EDPC-X(Tower) https://atcoder.jp/contests/dp/tasks/dp_x
 * 
 * ※本問は手前にXがある個数が問題であり、その距離は気にしない。
 *   値とXの距離もスコアに影響するとしたら、また話は変わってきそう。
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	using pll = pair<ll,ll>;
	vector<pll> s(N);  // {Sに含まれる数値の和, Sに含まれるXの数}
	ll ans = 0;
	for(i = 0; i < N; i++)
	{
		string t; cin >> t;
		ll sum = 0, cnt = 0;
		for(auto &e : t)
		{
			if(e == 'X')
			{
				cnt++;
			}
			else
			{
				int d = e - '0';
				ans += cnt * d;  // S内だけのスコアを事前に足しておく
				sum += d;
			}
		}
		s[i] = {sum, cnt};
	}

	auto mycomp = [&](pll l, pll r) -> bool
	{
		ll d1 = l.second * r.first;  // lrの順
		ll d2 = r.second * l.first;  // rlの順
		return d1 > d2;
	};
	sort(s.begin(), s.end(), mycomp);

	ll cnt = 0;  // Xの数
	for(auto &e : s)
	{
		ans += e.first * cnt;
		cnt += e.second;
	}
	cout << ans << endl;

	return 0;
}

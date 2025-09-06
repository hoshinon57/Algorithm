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

// ABC421 https://atcoder.jp/contests/abc421

/*
 * 重実装。コンテスト時に解ききれなかった。
 * 
 * 以下の衝突判定が天才。
 *   https://x.com/deuteridayodayo/status/1961791983783153712
 *   https://x.com/kari_math/status/1731217844929696083
 * 
 * 合計の移動回数が10^14と多く、また「途中で方向転換する」のを考えるのは大変なので、何らか考える必要がある。
 * ABC294-E(2xN Grid)を参考に、「高橋くんと青木くんが同じ方向に移動している」のを一括りにして考える。
 * 「今の方向のまま動く残り回数」が少ない方に合わせるイメージ。
 * 詳細は実装のwhile()部分を参照。
 * 
 * 以下2つの関数を用意する。
 *   mov(): (y,x)から動いた結果を返す
 *   chk(): 2人が今の位置からm回動いたときに、何回一致するかを返す
 * これを用意しておくと、while()内では
 *   今回のループで何回動くか -> m
 *   chk(m)をansに加算
 *   mov()を用いて2人を実際に動かす
 *   入力の移動回数であるa[]とb[]をmだけ減らす。必要に応じてidxを次へ
 * という実装で良い。
 * 
 * mov()の実装はそのままなので省略。
 * 
 * chk()が厄介。
 * まず、初期状態で2人が同じ位置にいるか、いないか、で場合分けをする。
 * 同じ位置にいる場合、移動方向が同じならm回一致し、そうでないなら0回。
 * 以降、別の位置にいる場合を考える。
 * 最初に載せたURLが天才解法で、
 *   2人は直線で移動するので、もし出会うならば (2人の距離/2)ターン後 しかありえない。
 *   これをttターン後とすると、試しに2人をttターン移動してみて、一致するかどうか判定すればよい。
 * でうまくいく。
 * tt>m のケースに注意。動くのはm回までなので。
**/

int main(void)
{
	ll i;
	ll rt, ct, ra, ca; cin >> rt >> ct >> ra >> ca;
	ll N, M, L; cin >> N >> M >> L;
	vector<char> s(M); vector<ll> a(M); for(i = 0; i < M; i++) {cin >> s[i] >> a[i];}
	vector<char> t(L); vector<ll> b(L); for(i = 0; i < L; i++) {cin >> t[i] >> b[i];}
	ll tidx=0, aidx=0;
	ll ans = 0;

	// (y,x)から動いた結果を返す
	auto mov = [&](ll y, ll x, char d, ll cnt) -> pair<ll,ll>
	{
		ll dx=0, dy=0;
		if(d == 'U') dy = -1;
		if(d == 'D') dy = 1;
		if(d == 'L') dx = -1;
		if(d == 'R') dx = 1;
		pair<ll,ll> ret;  // {y,x}
		ret.first = y + dy*cnt;
		ret.second = x + dx*cnt;
		return ret;
	};

	// m回動くときの一致回数を返す
	auto chk = [&](ll m) -> ll
	{
		if(rt == ra && ct == ca)
		{
			if(s[tidx] == t[aidx]) return m;
			else return 0;
		}
		else
		{
			ll tt = (abs(rt-ra) + abs(ct-ca)) / 2;  // 距離の半分 会うならこのターンのみ
			if(tt > m) return 0;
			auto[y1,x1] = mov(rt, ct, s[tidx], tt);
			auto[y2,x2] = mov(ra, ca, t[aidx], tt);
			if(y1 == y2 && x1 == x2) return 1;
			else return 0;
		}
	};

	while(tidx < M && aidx < L)
	{
		ll m = min(a[tidx], b[aidx]);  // このループではm回動く
		ans += chk(m);

		{
			auto[yy,xx] = mov(rt, ct, s[tidx], m);
			rt = yy;
			ct = xx;
		}
		{
			auto[yy,xx] = mov(ra, ca, t[aidx], m);
			ra = yy;
			ca = xx;
		}
		a[tidx] -= m;
		if(a[tidx] == 0) tidx++;
		b[aidx] -= m;
		if(b[aidx] == 0) aidx++;
	}

	cout << ans << endl;

	return 0;
}

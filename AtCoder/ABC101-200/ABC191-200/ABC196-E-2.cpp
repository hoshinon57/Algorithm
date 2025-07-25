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

// ABC196 https://atcoder.jp/contests/abc196

/*
 * 公式解説を元に実装した内容。
 * こちらの方がやりやすいかな。
 * 
 * 関数fは ＿／￣ こんな形になる。というのはABC196-E.cppと同じ。
 * yの下限と上限、そして"／"が無限に伸びていると仮定したときのx=0でのyの値（いわゆる切片）を管理する。(この3つからグラフは一意に決まる)
 * これらをそれぞれymi,yma,syとする。
 * ※グラフがx=0のとき"＿"や"￣"であっても、syを同じように管理して上手くいく。
 * 
 * t=1:
 * ymi,yma,syをそれぞれ+aする。
 * 
 * t=2:
 * 下限が上がるので、chmax(ymi,a)とする。
 * ymi>=ymaとなったら、グラフは横一直線になる。この場合yma=ymiとしておく。(syは更新せずとも上手くいく)
 * 
 * t=3:
 * 上限が下がるので、chmin(yma,a)とする。
 * ymi>=ymaとなったら、グラフは横一直線になる。この場合はt=2と逆にymi=ymaとしておく。
 * 
 * クエリ:
 * 傾きが1であるxの区間は [-(sy-ymi), yma-sy] となる。
 * この区間内かその外かで場合分けすればよい。
**/

int main(void)
{
	ll i;
	ll N; cin >> N;
	ll yma = INF64, ymi = -INF64, sy = 0;
	for(i = 0; i < N; i++)
	{
		ll a, t; cin >> a >> t;
		if(t == 1)
		{
			sy += a;
			yma += a;
			ymi += a;
		}
		if(t == 2)
		{
			chmax(ymi, a);
			if(ymi >= yma)
			{
				yma = ymi;
				// syは更新しなくてもOK
			}
		}
		if(t == 3)
		{
			chmin(yma, a);
			if(ymi >= yma)
			{
				ymi = yma;
			}
		}
	}

	ll xl = -(sy-ymi);
	ll xr = yma-sy;  // [xl,xr]が傾き1の区間
	ll Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll xq; cin >> xq;
		ll ans = 0;
		if(xl <= xq && xq <= xr)	ans = xq+sy;
		else if(xq < xl)	ans = ymi;
		else				ans = yma;
		cout << ans << endl;
	}

	return 0;
}

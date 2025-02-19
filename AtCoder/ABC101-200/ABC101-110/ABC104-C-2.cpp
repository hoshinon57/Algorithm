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

// ABC104 https://atcoder.jp/contests/abc104

/*
 * 各種解説の方針で実装した内容。bit全探索で解く。
 * 
 * 1～Dの問題カテゴリについて、
 * 1)0問解く
 * 2)[1,p[i]) 問、つまり「中途半端に」解く
 * 3)全完する
 * の3パターンがある。
 * 
 * このうち(2)は2カテゴリ以上でやるのは無駄。
 * なぜなら、中途半端に解いたカテゴリが2つあれば、高得点の方に寄せるとか、
 * コンプリートボーナスを狙いに行くか、といった方針の方が良いため。
 * 
 * よって(2)は最大で1つ、他は(1),(3)となる。
 * ここで(3)とするカテゴリをbit全探索で全探索する。
 * 各探索について、bitが1になっているカテゴリを全て解いてもG点に達しない場合、
 * bitが0になっているカテゴリのうち最高得点のものを1つ選び、G点に達するまで解く。
 * ⇒このときコンプリートボーナスの考慮は不要。
 *   ここでコンプするのは、別の全探索で計算できるため。
**/

// a/b以上の最小の整数(天井関数) ceil(5,2)=3, ceil(-5,2)=-2
template <typename T> T ceil_div(T a, T b){ if(b<0) {a=-a; b=-b;} if(a>0){return (a+b-1)/b;} else {return a/b;}}

int main(void)
{
	ll i, j;
	ll D, G; cin >> D >> G;
	vector<ll> p(D), c(D); for(i = 0; i < D; i++) {cin >> p[i] >> c[i];}

	ll ans = INF64;
	for(i = 0; i < (1<<D); i++)  // iのうちbitが1のものを全完する
	{
		ll sc = 0, cnt = 0;
		// bitが1のものを集計
		for(j = 0; j < D; j++)
		{
			if( ((i>>j)&1) == 0 ) continue;
			sc += (j+1)*p[j]*100 + c[j];
			cnt += p[j];
		}

		if(sc >= G)
		{
			chmin(ans, cnt);
		}
		else
		{
			for(j = D-1; j >= 0; j--)  // 高得点の方から
			{
				if( ((i>>j)&1) == 1 ) continue;  // 全完済み

				ll zan = ceil_div(G-sc, (j+1)*100);  // 解く必要がある問題数
				if(zan > p[j]) continue;
				chmin(ans, cnt+zan);
				break;  // 高得点の方から1つだけ採用するので
			}
		}
	}
	cout << ans << endl;

	return 0;
}

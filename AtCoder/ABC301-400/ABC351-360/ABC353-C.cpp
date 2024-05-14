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

// ABC353 https://atcoder.jp/contests/abc353

/*
 * むずい。苦戦した。
 * 全体のMODではなく、各f(x,y)についてのみMODを取るのが厄介。
 * 
 * 以下、MOD=10^8として記載する。
 * 
 * いったんMODは無視して考えると、
 * 二重Σでjはi+1から始まるので、各A[i]は(N-1)回加算されることが分かる。
 * 
 * またA[i]<MODより、a[i]+a[j]>=MOD のとき f(a[i],a[j]) = a[i]+a[j]-MOD となる。
 * このMODによる減算回数を求め、総和から引くことを考える。
 * f(x,y)=f(y,x)なので、Aを昇順ソートしても良くて、
 * lower_bound()にてf(a[i],X)となるXの要素数を求めることができる。
 * 
 * ただし減算回数の注意点として以下2点がある。
 * ・f(a[X],a[Y])とf(a[Y],a[X])でダブリカウントされる
 * ・f(a[X],a[X])は1回のみのカウントで済むが、i<jの条件により除外すべき
 * 前者は最後にカウントを/2すればよい。
 * 後者はa[i]+a[i]>=MODならカウントを-1して除外すればよい。
 * 
 * [ACまでの思考の流れ]
 * ・各Aiを(N-1)倍して最後にMOD... いやこれ違うじゃん！
 * ・f(x,y)は「x+y>=MODならMODを引く」と読み替えられる。制約により x+y>=MOD*2 にはならないので。
 * ・ならMODを引く回数を計算する形になるかな。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end());

	const ll MOD = 1e8;
	ll ans = 0;
	ll cnt = 0;  // f(i,j)>=10^8 となり後から引く回数
	for(i = 0; i < N; i++)
	{
		ans += a[i] * (N-1);

		ll tmp = a.end() - lower_bound(a.begin(), a.end(), MOD-a[i]);
		if(a[i]+a[i] >= MOD) tmp--;
		cnt += tmp;
	}
	cnt /= 2;
	ans -= cnt*MOD;
	cout << ans << endl;

	return 0;
}

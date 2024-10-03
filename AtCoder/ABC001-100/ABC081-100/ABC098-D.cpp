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

// ABC098 https://atcoder.jp/contests/abc098

/*
 * 尺取り法で解く。
 * 
 * 数列のxorとsumが等しくなる条件は、各bitごとに1が1個以下であること。
 * 2個以上あると加算にて繰り上がりが発生し、不一致となる。
 * 
 * という条件を踏まえると、尺取り法が使える以下条件を満たしていることが分かる。
 * ⇒区間 [l,r) が「条件」を満たすなら、それに含まれる区間、例えば [l+1,r) や [l,r-1) も「条件」を満たす。
 * 
 * 各bitにて1が立っている個数が1個以下である間、[l,r)を伸ばしていく。
 * これは[l,r-1)までのORとa[r-1]のANDを取り、0であれば[l,r)も合法と言える。
 * ⇒check()を参照。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	int c = 0;  // 今見ている区間のbit or
	ll ans = 0;
	// 区間の右をidxまで伸ばしたときに、まだ合法ならtrue
	auto check = [&](int idx) -> bool
	{
		if(c & a[idx]) return false;  // bitごとに見て、1が重複したらNG (区間に1が2つ以上存在することになり、xor!=sumになる)
		else return true;
	};
	
	int l,r;
	r = 0;
	for(l = 0; l < N; l++)  // [l,r)
	{
		while(r < N && check(r))
		{
			c |= a[r];
			r++;
		}
		ans += r-l;  // lを固定したときの合法な[l,r)の個数
		if(l==r) r++;
		else c &= ~a[l];  // a[l]で立てているbitを落とす
	}
	cout << ans << endl;

	return 0;
}

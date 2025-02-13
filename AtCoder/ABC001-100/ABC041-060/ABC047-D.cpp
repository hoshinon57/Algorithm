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

// ABC047 https://atcoder.jp/contests/abc047

/*
 * しばらくウンウンうなったが、やっぱりTは答に無関係だった。
 * けんちょん氏と同じ感想になった。
 *   https://drken1215.hatenablog.com/entry/2024/09/21/162249
 *   https://img.atcoder.jp/data/arc/063/editorial.pdf
 * 
 * 各iについて、ai-min(a0,a1,...,a(i-1)) が最大になる箇所で売買すればよい、となる。
 *   mi:これまでのaiの最小値
 *   ma:ai-min()の最大値
 * を管理していく。
 * 
 * 「最大になる組」は複数存在することがあり、その場合はそれぞれ補正しなければならない。
 * それをcntで管理する。
 * 
 * 「最大になる組」にて大きい側を1円安くすればよいので、cntがそのまま答。
**/

int main(void)
{
	int i;
	int N, T; cin >> N >> T;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	int mi = a[0];
	int ma = 0;
	int cnt = 0;  // 最大利益となる組数
	for(i = 1; i < N; i++)
	{
		int tmp = a[i] - mi;
		if(tmp > ma)  // 最大利益を更新
		{
			ma = tmp;
			cnt = 1;
		}
		else if(tmp == ma)  // 最大利益となる組が複数
		{
			cnt++;
		}
		chmin(mi, a[i]);
	}
	cout << cnt << endl;

	return 0;
}

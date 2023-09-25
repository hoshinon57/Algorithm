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

// ABC312 https://atcoder.jp/contests/abc312

/*
 * 二分探索で解く。
 * 
 * まず、Ai,Biはソートしても問題ない。
 * 
 * 売買価格をX円としたときに、
 * (1)売ってくれる人数：AiがX円以下の値の人数
 * (2)買ってくれる人数：BiがX円以上の値の人数
 * にて(1)>=(2)であれば条件を満たす。
 * 
 * Xを上げていくにつれ、(1)は単調増加、(2)は単調減少となるため
 * どこかで条件を満たさない/満たすが切り替わる。
 * これを二分探索で求めればよい。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<int> b(M); for(i = 0; i < M; i++) {cin >> b[i];}
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());

	int l = 0;
	int r = INF32;
	auto check = [&](int m) -> bool
	{
		int n1 = upper_bound(a.begin(), a.end(), m) - a.begin();  // AiがX円以下の値の人数
		int n2 = b.end() - lower_bound(b.begin(), b.end(), m);  // BiがX円以上の値の人数
		return n1 >= n2;
	};
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(mid)) r = mid;
		else l = mid;
	}
	cout << r << endl;

	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC094 https://atcoder.jp/contests/abc094

/*
 * comb(n,r)とcomb(n+1,r)とでは後者の方が大きい。
 * よってnはa[]の最大値を選べばよい。
 * 
 * rは、n/2に近い要素を選べばよい。
 * nを除いた要素について、条件を指定してソートする実装とした。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	sort(a.begin(), a.end());
	auto x = a.back();

	// 最後の要素を除いてソート (ai>ajという制約のため)
	// |x/2-a1|と|x/2-a2|の比較なので、2倍して計算
	sort(a.begin(), a.begin()+N, [&](int a1, int a2){ return abs(x-a1*2) < abs(x-a2*2); });
	cout << x << " " << a[0] << endl;

	return 0;
}

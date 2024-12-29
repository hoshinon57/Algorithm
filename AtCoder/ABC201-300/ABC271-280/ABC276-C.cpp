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

// ABC276 https://atcoder.jp/contests/abc276

/*
 * prev_permutation()を使わず自力で実装してみた版。
 * (prev使用は#ifの#else側に記述している)
 * 
 * {1,2,3,4}の順列を途中一部抜き出すと、以下のようになる。下の方に行くとnext_permutaion側。
 * 1,3,2,4
 * 1,3,4,2
 * 1,4,2,3
 * 1,4,3,2
 * 2,1,3,4
 * 2,1,4,3
 * 2,3,1,4
 * 
 * これをぐっと睨むと、next_permutaionの手順は以下となる。
 * (1)末尾から手前に見ていって、初めてa[j]>a[j+1]の単調増加が崩れるjを見つける。
 *   (実装ではj=N-2を初期値とする)
 * (2)また末尾から手前に見ていって、初めてa[j]<a[x]となるxを見つける。
 * (3)swap(a[j],a[x]).
 * (4)j+1から末尾までをreverse.
 * 
 * prev_permutaionは手順を少し変えて、
 * (1)にて単調減少が崩れる点を見つける。
 * (2)にて初めてa[j]>a[x]となるxを見つける。
 * とすればよい。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

#if 1
	// (1)末尾から手前に見ていって、初めてa[j]<a[j+1]の単調減少が崩れるjを見つける。
	int j = N-2;
	while(a[j] < a[j+1]) j--;

	// (2)また末尾から手前に見ていって、初めてa[j]>a[x]となるxを見つける。
	int x = N-1;
	while(a[j] < a[x]) x--;

	// (3)swap(a[j],a[x]).
 	// (4)j+1から末尾までをreverse.
	swap(a[j], a[x]);
	reverse(a.begin()+j+1, a.end());
#else
	prev_permutation(a.begin(), a.end());
#endif

	int sz_ = (int)a.size();
	for(i = 0; i < sz_; i++) {
		cout << a[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}

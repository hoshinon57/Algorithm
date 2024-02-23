#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC197 https://atcoder.jp/contests/abc197

/*
 * bit全探索で解く。公式解説とほぼ同じ実装だった。
 *   https://atcoder.jp/contests/abc197/editorial/997
 * 
 * 区間を区切る位置は(N-1)個あり、区切り方は2^(N-1)通りある。
 * これをbit全探索でそれぞれ計算していけばよい。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	int ans = INF32;
	for(i = 0; i < (1<<(N-1)); i++)  // iのjビット目が1なら、Ajの次で区切る
	{
		int xor_tot = 0, or_now = 0;
		for(j = 0; j < N; j++)  // Aj
		{
			or_now |= a[j];
			if( ((i>>j)&1) || j == N-1)  // Ajの次で区切る or Aの末尾なので区切る
			{
				xor_tot ^= or_now;
				or_now = 0;
			}
		}
		chmin(ans, xor_tot);
	}
	cout << ans << endl;

	return 0;
}

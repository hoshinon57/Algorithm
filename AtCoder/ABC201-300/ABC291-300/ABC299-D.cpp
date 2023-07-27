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

// ABC299 https://atcoder.jp/contests/abc299

/*
 * 二分探索で解く。
 * 
 * 例えばN=9として、S1=0, S9=1であることから、真ん中のS5が
 *   1 -> S1～S5の間に、0/1の切り替わりが必ず存在する
 *   0 -> S5～S9の間に、0/1の切り替わりが必ず存在する
 * となる。
 * l=1, r=Nを初期値として二分探索で範囲を狭めていけばよい。
 * (2*10^5 < 2^20 より、質問20回で十分に解ける)
 * 
 * l+1=rとなったときの、lが答。
 */
int main(void)
{
	int N;
	cin >> N;

	int l = 1, r = N;
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		int c;
		// ? i
		cout << "? " << mid << endl;
		cin >> c;
		if(c == 1)
		{
			r = mid;
		}
		else
		{
			l = mid;
		}
	}
	// ! p
	cout << "! " << l << endl;

	return 0;
}

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

// ABC169 https://atcoder.jp/contests/abc169

// Bをdouble型で受け取って100倍する実装は1WAだった。
// 例えばB=2.51のとき、(int)(B*100)=250になる模様。
//   https://twitter.com/kyopro_friends/status/1267093871957164032
// 
// よって入力もdouble型を使わずに文字列とし、数値部分を抜き出して計算した。

int main(void)
{
	ll A; cin >> A;
	string B; cin >> B;
	ll B2 = (B[0]-'0')*100 + (B[2]-'0')*10 + (B[3]-'0');
	cout << A*B2/100 << endl;

	return 0;
}

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

// ABC035 https://atcoder.jp/contests/abc035

// いもす法で解く版。
// 加算結果が偶数なら黒、奇数なら白。

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	vector<int> imos(N+5);
	for(i = 0; i < Q; i++)
	{
		int l, r; cin >> l >> r;
		imos[l]++;
		imos[r+1]--;
	}
	for(i = 1; i <= N; i++)
	{
		imos[i] += imos[i-1];
		cout << imos[i]%2;
	}
	cout << endl;

	return 0;
}

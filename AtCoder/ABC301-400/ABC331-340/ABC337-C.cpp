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

// ABC337 https://atcoder.jp/contests/abc337

// コンテスト時にあまり良い実装ができなかったので、簡易な実装を調べた。
// nxt[x]を人xの次の人の番号とする。入力としてはnxt[Ai]=iとなる。
// Ai=-1を先頭とし、現在の番号が-1になるまでnxt[]を使って辿っていく。

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N+1), nxt(N+1, -1);
	int top;
	for(i = 1; i <= N; i++)
	{
		cin >> a[i];
		if(a[i] == -1) top = i;
		else nxt[a[i]] = i;
	}
	int now = top;
	while(now != -1)
	{
		cout << now << " ";
		now = nxt[now];
	}
	cout << endl;

	return 0;
}

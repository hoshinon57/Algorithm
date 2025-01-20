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

// https://atcoder.jp/contests/tenka1-2019-beginner/tasks/tenka1_2019_c

// 最終形を考えると、 ...#### のように前半白、後半黒となる。
// 各iについて[0,i)と[i,N)に区切り、前者を白、後者を黒にする操作回数を求め、その最小値が答。
// 累積和を用いると各iごとにO(1)で計算できる。

int main(void)
{
	int i;
	int N; cin >> N;
	string s; cin >> s;
	vector<int> sum(N+1);  // 黒の個数の累積和
	for(i = 0; i < N; i++)
	{
		sum[i+1] = sum[i];
		if(s[i] == '#') sum[i+1]++;
	}

	int ans = INF32;
	for(i = 0; i <= N; i++)
	{
		// [0,i) と [i,N) で区切る
		// 前者を白に、後者を黒にする

		// [0,i)を黒⇒白へ
		int tmp1 = sum[i]-sum[0];

		// [i,N)を白⇒黒へ
		int tmp2 = (N-i) - (sum[N]-sum[i]);

		chmin(ans, tmp1+tmp2);
	}
	cout << ans << endl;

	return 0;
}

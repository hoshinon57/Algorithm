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

// Educational DP Contest / DP まとめコンテスト https://atcoder.jp/contests/dp

/*
 * 以下の解説と同じ考え方。
 *   https://kyopro-friends.hatenablog.com/entry/2019/01/12/231000
 *   https://qiita.com/drken/items/4e1bcf8413af16cb62da
 * 
 * dp[i]を
 *   残りの石がi個で自分の手番になったとき、勝てるならtrue.
 * と定義する。
 * そこから遷移できる dp[i-a1], dp[i-a2], ..., dp[i-aN] のうち、1つでもfalse, つまり相手の負けがあれば
 * それを選択すれば勝てる。
 */

int main(void)
{
	int i, j;
	int N, K; cin >> N >> K;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<bool> dp(K+1, false);
	for(i = 0; i <= K; i++)
	{
		for(j = 0; j < N; j++)
		{
			// dp[i] -> dp[i-a[j]] への遷移
			if(i-a[j] < 0) continue;
			if(!dp[i-a[j]]) dp[i] = true;
		}
	}
	string ans = "First";
	if(!dp[K]) ans = "Second";
	cout << ans << endl;

	return 0;
}

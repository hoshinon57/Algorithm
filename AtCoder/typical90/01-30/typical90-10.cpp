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

// 競プロ典型90問:10 https://atcoder.jp/contests/typical90/tasks/typical90_j

// クラス1,2でそれぞれ累積和を取る

int main(void)
{
	// 1-indexed
	int i;
	int N; cin >> N;
	vector<vector<int>> p(2, vector<int>(N+5, 0));  // p[2][N+5]
	vector<vector<int>> sum(2, vector<int>(N+5, 0));  // sum[2][N+5]
	for(i = 1; i <= N; i++)
	{
		int c, p_; cin >> c >> p_;
		c--;
		p[c][i] = p_;
	}
	for(i = 1; i <= N; i++)
	{
		sum[0][i+1] = sum[0][i] + p[0][i];
		sum[1][i+1] = sum[1][i] + p[1][i];
	}
	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int l, r; cin >> l >> r;
		r++;
		cout << sum[0][r]-sum[0][l] << " " << sum[1][r]-sum[1][l] << endl;
	}

	return 0;
}

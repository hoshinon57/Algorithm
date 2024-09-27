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

// ABC371 https://atcoder.jp/contests/abc371

/*
 * 公式解説の方針。主客転倒＋余事象で解く。
 *   https://atcoder.jp/contests/abc371/editorial/10922
 * 
 * 公式解説の通りで、1が何回含まれるか、2が何回含まれるか…をカウントする。
 * これを直接カウントするのは難しいので、余事象で考えて全体から引いてみる。
 * 
 * 例えばA=(2,1,3,3,3,1)のとき、1が含まれないのは区間[0,0]と[2,4]となる。
 * [0,0]の選び方は1通り、[2,4]の選び方は6通り。
 * idx[x]として値xが登場するidxを事前に求めておくと、全体でO(N)で計算できる。
 * このときidx[]の先頭と末尾に番兵を入れておくと処理が楽。
 * 
 * 区間の選び方は N*(N+1)/2 通りなので、Aの取りうる1～Nそれぞれについて上記の余事象を引き、
 * その総和が答。
 */

int main(void)
{
	int i, j;
	ll N; cin >> N;
	// idx[x]={...} 値xが登場するidx
	vector<vector<int>> idx(N+5, vector<int>(1, -1));  // 初期値に番兵として-1を入れておく
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		idx[a].push_back(i);
	}
	for(auto &e : idx) e.push_back(N);  // 末尾に番兵

	ll ans = 0;
	for(i = 1; i <= N; i++)  // idx[i]
	{
		ans += N*(N+1)/2;
		for(j = 1; j < (int)idx[i].size(); j++)
		{
			ll d = idx[i][j] - idx[i][j-1] - 1;  // 値iが含まれない区間の長さ
			ans -= d*(d+1)/2;
		}
	}
	cout << ans << endl;

	return 0;
}

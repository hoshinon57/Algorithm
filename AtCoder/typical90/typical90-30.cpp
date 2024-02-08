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

// 競プロ典型90問:30 https://atcoder.jp/contests/typical90/tasks/typical90_ad

/*
 * エラトステネスの篩っぽく解く。
 * 考え方や実装は公式解説の通り。
 *   https://twitter.com/e869120/status/1389349360866009090
 * 
 * cnt[i]をiの素因数の数とすると、
 * iを昇順に見ていったときにcnt[i]=0ならiは素数である。
 * 素数であれば、 i,i*2,i*3,... にはiが素因数として存在するので、cnt[]を+1する。
 * 計算量はO(NloglogN).
 * 
 * 最終的にcnt[i]>=K であるものの数が答。
 * 
 * 最初はN以下の素数を列挙したが、その必要は無かった。
 * 素数列挙の"気持ち"を理解していれば不要だと分かるはず、反省。
 */

int main(void)
{
	ll i;
	ll N, K; cin >> N >> K;
	vector<int> cnt(N+1);  // cnt[i]:素因数の数
	for(i = 2; i <= N; i++)
	{
		if(cnt[i] != 0) continue;
		for(int j = i; j <= N; j+=i) cnt[j]++;
	}
	int ans = 0;
	for(auto &e : cnt)
	{
		if(e >= K) ans++;
	}
	cout << ans << endl;

	return 0;
}

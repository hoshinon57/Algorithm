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

// 競プロ典型90問:58 https://atcoder.jp/contests/typical90/tasks/typical90_bf

// 周期性を用いて解く

int main(void)
{
	const int MOD = 1e5;
	ll N, K; cin >> N >> K;
	vector<bool> used(MOD, false);  // 探索済みか
	vector<int> path;  // 初期値Nから辿る値を順に格納する
	int cur = N;
	while(!used[cur])
	{
		path.push_back(cur);
		int d = cur, nx = d;
		while(d > 0)
		{
			nx += d%10;
			d /= 10;
		}
		nx %= MOD;
		used[cur] = true;
		cur = nx;
	}

	ll bef_loop = find(path.begin(), path.end(), cur) - path.begin();
	ll loop_num = path.size() - bef_loop;
	ll ans;
	if(K < bef_loop)  // ループに入る前
	{
		ans = path[K];
	}
	else
	{
		// ループに入る前の余分な要素を引いて、それを周期で割った余り -> ループ中の何番目か
		// それとbef_loopを加算すれば、何番目の要素かが分かる (<N)
		ll tmp = ((K-bef_loop)%loop_num) + bef_loop;
		ans = path[tmp];
	}
	cout << ans << endl;

	return 0;
}

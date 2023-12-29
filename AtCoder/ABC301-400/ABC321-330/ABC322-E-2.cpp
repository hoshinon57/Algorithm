#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC322 https://atcoder.jp/contests/abc322

/*
 * 公式解説にて
 *   ＞もしくは、C++ などの高速な言語だとaをvector で管理して、dpにmapを使うなどをしても良いです。
 * という記載があったので試してみた版。
 * 実行時間は309msと、かなり余裕があった。
 * 
 * これはこれでmapのkey/valが取っ散らかって混乱するかな…
 * 自分の実装力が低いからこう感じるのかも。
 * 何にせよ練習になった。
 */

int main(void)
{
	int i, j;
	int N, K, P; cin >> N >> K >> P;
	vector<ll> c(N);
	vector<vector<int>> a(N, vector<int>(K, 0));  // a[N][K]
	for(i = 0; i < N; i++)
	{
		cin >> c[i];
		for(j = 0; j < K; j++) cin >> a[i][j];
	}
	map<vector<int>,ll> mp;
	vector<int> def(K);
	mp[def] = 0;
	for(i = 0; i < N; i++)
	{
		map<vector<int>,ll> mp_new;
		for(auto& [key, value] : mp)
		{
			if(mp_new.count(key) == 0) mp_new[key] = value;
			else chmin(mp_new[key], mp[key]);

			vector<int> aft = key;
			for(j = 0; j < K; j++)
			{
				aft[j] = min(key[j]+a[i][j], P);
			}
			if(mp_new.count(aft) == 0) mp_new[aft] = value + c[i];
			else chmin(mp_new[aft], value + c[i]);
		}

		swap(mp, mp_new);
	}

	vector<int> last(K, P);
	if(mp.count(last) == 0) cout << -1 << endl;
	else cout << mp[last] << endl;

	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC123 https://atcoder.jp/contests/abc123

/*
 * priority_queueを用いて解く。
 * 公式解説の#3に近い。
 *   https://img.atcoder.jp/abc123/editorial.pdf
 * 
 * Aだけ降順ソートしておく。
 * B*Cの全組み合わせについて、「Aの大きい方から何番目まで組み合わせたか」をidx[y][z]で管理する。
 * これと優先度付きキューを用いれば、A*B*Cの全組み合わせにて大きい方から取得できる。
 * 
 * [ACまでの思考の流れ]
 * ・X*Y*Z通りを全探索はもちろんTLE.
 * ・「組み合わせて大きい方からK個」はpriority_queueで解ける。(ex:ABC331-E)
 * ・B*Cの組み合わせについて全て管理し、それぞれで「Aの大きい方から何番目まで組み合わせたか」を持たせれば
 *   A*B*Cの組み合わせで大きい方から取得できそう。
 */

int main(void)
{
	int i, j;
	ll x, y, z, k; cin >> x >> y >> z >> k;
	vector<ll> a(x); for(i = 0; i < x; i++) {cin >> a[i];}
	vector<ll> b(y); for(i = 0; i < y; i++) {cin >> b[i];}
	vector<ll> c(z); for(i = 0; i < z; i++) {cin >> c[i];}
	sort(a.begin(), a.end(), greater<ll>());

	vector<vector<int>> idx(y, vector<int>(z, 0));  // idx[y][z] (B[y],C[z])の組み合わせにて、現在キューに積まれているAの番号
	using dt = array<ll,3>;  // {y+zの美味しさ, yのidx, zのidx}
	priority_queue<dt> que;
	for(i = 0; i < y; i++)
	{
		for(j = 0; j < z; j++)
		{
			ll tmp = a[0] + b[i] + c[j];
			que.push({tmp, i, j});
		}
	}

	while(k > 0)
	{
		k--;
		auto [val, yy, zz] = que.top();
		que.pop();
		cout << val << endl;

		int &xx = idx[yy][zz];
		xx++;
		if(xx < x)  // まだAが残っていれば
		{
			ll tmp = a[xx] + b[yy] + c[zz];
			que.push({tmp, yy, zz});
		}
	}

	return 0;
}

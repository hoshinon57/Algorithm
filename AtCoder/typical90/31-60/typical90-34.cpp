#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:34 https://atcoder.jp/contests/typical90/tasks/typical90_ah

/*
 * 尺取り法で解く。
 * 実装方針はけんちょん氏の以下を参考にした。
 *   https://qiita.com/drken/items/ecd1a472d3a0e7db8dce
 * 
 * 区間を[l,r)の半開区間で管理し、
 * rは1つ進めても大丈夫か(K種以下か)を判定してから、1つ進める。
 */

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	int l = 0, r = 0;  // [l,r)
	int ans = 0;
	map<int,int> cnt;  // cnt[i]:A=iの登場回数
	for(l = 0; l < N; l++)
	{
		while(r < N)
		{
			// rを1つ進めても大丈夫か判定し、大丈夫なら1つ進める
			if(cnt.count(a[r]) == 0 && (int)cnt.size() == K) break;  // rを加えたらK種を超える
			cnt[a[r]]++;
			r++;
		}
		// [l,r)は条件を満たす
		chmax(ans, r-l);

		if(--cnt[a[l]] == 0)
		{
			cnt.erase(a[l]);
		}
	}
	cout << ans << endl;

	return 0;
}

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

// ABC404 https://atcoder.jp/contests/abc404

/*
 * 地味に混乱した。O(N^2)で解く。
 *   https://x.com/kyopro_friends/status/1918662593964544265
 * 
 * 1操作にて取り出す豆に制限はないことから、
 * ・ある茶碗の豆をまとめて移動させる、としてよい（損しない）
 * ・行き先に「豆のある茶碗」があれば、そこに便乗するのが良い（損しない）
 * となる。
 * ここから、dist[i]を「そこに豆があるとき、何回の操作が必要か」と定義すると、
 * 茶碗iについて
 * ・区間[i-Ci,i-1]に豆のある茶碗があれば、dist[i]=1.
 * ・なければ、dist[i]=min(dist[i-Ci]～dist[i-1])+1
 * となる。
 * 
 * 愚直にやってO(N^2)で、制約より間に合う。
 * 
 * 最後に、豆があるところのdist総和が答。
**/

int main(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> c(N); for(i = 1; i < N; i++) {cin >> c[i];}
	vector<int> a(N); for(i = 1; i < N; i++) {cin >> a[i];}

	vector<int> dist(N, INF32);
	dist[0] = 0;
	for(i = 1; i < N; i++)  // 茶碗i
	{
		for(j = i-c[i]; j < i; j++)
		{
			if(a[j] > 0)  // そこに豆があれば便乗する（損しない）
			{
				dist[i] = 1;
				break;
			}
			chmin(dist[i], dist[j]+1);
		}
	}
	// 豆があるところのdist総和
	int ans = 0;
	for(i = 1; i < N; i++)
	{
		if(a[i] > 0) ans += dist[i];
	}
	cout << ans << endl;

	return 0;
}

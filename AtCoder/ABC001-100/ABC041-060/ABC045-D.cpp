#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC045 https://atcoder.jp/contests/abc045

/*
 * 各種解説と同じ方針。
 *   https://img.atcoder.jp/data/arc/061/editorial.pdf
 *   https://blog.hamayanhamayan.com/entry/2016/09/12/020114
 *   https://drken1215.hatenablog.com/entry/2024/09/14/122306
 * 
 * 盤面が10^9と大きいので、黒く塗りつぶすNマスを軸にして考える。
 * あるマス(a,b)を塗りつぶしたときに、3x3の正方形で影響するのは、正方形の左上マスを(y,x)としたときに
 *   y:a-2～a
 *   x:b-2～b
 * の計9通りに抑えられる。（盤面の外になるような正方形は適切に除外する）
 * blk[{y,x}]を、(y,x)を3x3の左上としたときの黒マスの個数と定義すると、
 * 各(a,b)に対して上記9パターンについて+1していけばよい。
 * 
 * 黒いマスが0個の数は、正方形の置き方全体から、黒いマスが1個以上のものを引いて求められる。
**/

int main(void)
{
	// 0-indexed
	ll i;
	ll H, W, N; cin >> H >> W >> N;
	map<pair<ll,ll>,int> blk;  // blk[{y,x}]: (y,x)を3x3の左上としたときの、黒マスの個数
	for(i = 0; i < N; i++)
	{
		ll a, b; cin >> a >> b;
		a--; b--;  // (a,b)
		for(ll y = max(a-2, 0LL); y <= min(a, H-3); y++)
		{
			for(ll x = max(b-2, 0LL); x <= min(b, W-3); x++)
			{
				blk[{y,x}]++;
			}
		}
	}

	vector<ll> ans(10);
	ans[0] = (H-2)*(W-2);  // 3x3の正方形を置けるのは (H-2)*(W-2) 通り
	for(auto&[k,v] : blk)
	{
		ans[v]++;
		ans[0]--;
	}
	for(auto &e : ans) cout << e << endl;

	return 0;
}

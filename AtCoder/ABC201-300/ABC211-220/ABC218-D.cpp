#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC218 https://atcoder.jp/contests/abc218

/*
 * 長方形の対角線にある2頂点について全探索する。
 * 
 * 対角線にある2頂点を選んだとき、長方形は一意に定まる。
 * 残りの2頂点が存在するかを判定すればよい。
 * 選んだ2頂点のxまたはyが一致する場合、長方形は作れないので除外。
 * 
 * ある長方形について「左上と右下」「左下と右上」とで2回カウントされるので、出力時は/2する。
 */

int main(void)
{
	int i, j;
	int N; cin >> N;
	set<pair<int,int>> p;  // {x,y}
	vector<int> x(N), y(N);
	for(i = 0; i < N; i++)
	{
		cin >> x[i] >> y[i];
		p.insert({x[i], y[i]});
	}

	int ans = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i+1; j < N; j++)  // i,jを長方形の対角線の2頂点とする
		{
			if(x[i] == x[j] || y[i] == y[j]) continue;
			if(p.count({x[i], y[j]}) > 0 && p.count({x[j], y[i]}) > 0) ans++;
		}
	}
	cout << ans/2 << endl;

	return 0;
}

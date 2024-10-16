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

// ABC375 https://atcoder.jp/contests/abc375

/*
 * 基本は公式解説の通り。
 *   https://atcoder.jp/contests/abc375/editorial/11141
 *   https://www.youtube.com/watch?v=W4grGfqWOr8
 * 
 * 1-indexedにて以下が成り立つ。
 * iとN+1-iは中央を境に対象の位置となる。
 * (y,x)を90度時計回りすると(x,N+1-y)となる。
 * ※0-indexedの場合、+1が-1になる。
 * 
 * 問題文を読み解くと、
 * ・外周の点は、時計回りを1回行った点に移動
 * ・外周から2周り目は、時計回りを2回行った点に移動
 * ・外周からx周り目は、時計回りをx回行った点に移動
 * となる。
 * 
 * 各点について操作回数を求め(4回で元に戻るのでMOD4で良い)、実際に移動させる。
 * これならO(N^2)で間に合う。
 * 
 * [ACまでの思考の流れ]
 * ・まず、問題文が何を言っているのかを理解する。
 *   ⇒外周から何周り目にいるかで、操作回数が決まる。
 *     操作は時計回りになっている。
 * ・1-indexedにて、
 *   iとN+1-iは中央を境に対象の位置となる。
 *   (y,x)を90度時計回りすると(x,N+1-y)となる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<string> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	auto b = a;
	for(int y = 0; y < N; y++)
	{
		for(int x = 0; x < N; x++)  // (y,x)
		{
			int k = min({y, x, N-1-y, N-1-x}) + 1;  // マス(y,x)の操作回数
			k %= 4;
			int ny = y, nx = x;
			for(i = 0; i < k; i++)  // (ny,nx)を(nx,N-1-ny)へ
			{
				int ty = nx;
				int tx = N-1-ny;
				nx = tx;
				ny = ty;
			}
			b[ny][nx] = a[y][x];
		}
	}

	for(auto &e : b) cout << e << endl;

	return 0;
}

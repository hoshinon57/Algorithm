#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC249 https://atcoder.jp/contests/abc249

/*
 * 後の操作で前の操作が上書きされる場合、逆から考えてみる。 [典型]
 * 
 * 入力例3を参考に考えてみる。以下はyの値であり、t=1の代入操作を "y" として区別する。
 *   3 -1 "4" -1 5 -9 2 "-6" 5 -3   (K=3)
 * "-6"の代入操作を無視しない場合、それより手前の操作は答に影響しない。
 * よって"-6"以降の値について考えればよい。これは
 *   正の値なら無視せず使う
 *   負の値なら最大K個無視する。K個を超えた場合、大きいものから使っていく
 * という手順にて求められる。
 * 
 * 次に"4"を無視しないケースを考えると、先ほどの"-6"は無視する必要がある。
 * よって上記の手順にて、最大(K-1)個まで無視した場合の値となる。
 * 
 * 無視する値を優先度付きキューにて管理するとうまくいく。
 * t=2操作のときはとりあえずキューに入れ、t=1のときにまとめて処理すると実装が楽だった。
 * (ABC281-E Least Elements, ABC306-EBest Performances などで習得した)
 * 
 * また、先頭に番兵として {t=1, y=0} を入れると実装が楽。
 * 
 * 末尾から見ていって、t=2の代入操作はK個で探索ストップする必要がある。
 * そこより右側にて必ず代入操作が発生するため。(1WA)
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<ll> t(N+1), y(N+1);
	t[0] = 1; y[0] = 0;  // 先頭は番兵として {t=1,y=0}
	for(i = 1; i <= N; i++) cin >> t[i] >> y[i];

	priority_queue<ll> que;  // t=2にて、無視するyの値のリスト 大きい方が優先
	ll now = 0;
	ll ans = -INF64;
	for(i = N; i >= 0; i--)
	{
		if(t[i] == 1)
		{
			while(!que.empty())
			{
				auto q = que.top();
				// 無視リスト先頭が正の値なら、無視せず使う
				// 無視リストのサイズがKより大きければ、無視しきれないので最大のを使う
				if(q > 0 || (ll)que.size() > K)
				{
					now += q;
					que.pop();
				}
				else break;
			}
			chmax(ans, y[i]+now);
			K--;
			if(K < 0) break;  // 現在(=i)のt=1操作は無視できない、つまりこれ以上の探索は打ち切り
		}
		else
		{
			que.push(y[i]);  // とりあえず無視リストへ
		}
	}
	cout << ans << endl;

	return 0;
}

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

// ABC421 https://atcoder.jp/contests/abc421

/*
 * 公式解説を元に、連結リストっぽく解いた版。
 *   https://atcoder.jp/contests/abc421/editorial/13787
 * 
 * 基本は上記解説を参照。
 * nxt[i]をiの次の値として構築する。
 * 
 * タイプ2のクエリについて、x<yと仮定すると
 * x->nxt[x]->nxt[nxt[x]]->... とyになるまでたどっていき、区間の和が答。
 * その区間の削除は nxt[x]=y だけで良い。
 * 
 * 問題はy<xになるケースもあること。
 * これはy->nxt[y]->nxt[nxt[y]]->... とたどるのも並行して実施すれば、計算量はO(Q)で抑えられる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・xから進めていくのとyから進めていくのとでどちらが正解か不明なとき、
 *   両方を並行して進めることで、全体の計算量を変えずO(N)で解く手法。
**/

int main(void)
{
	ll i;
	ll Q; cin >> Q;
	vector<ll> nxt(Q+5);  // nxt[i]: iの次の要素
	nxt[0] = -1;  // 末尾
	for(i = 1; i <= Q; i++)  // 1始まり
	{
		int ki; cin >> ki;
		if(ki == 1)
		{
			ll x; cin >> x;
			nxt[i] = nxt[x];
			nxt[x] = i;
		}
		if(ki == 2)
		{
			ll x, y; cin >> x >> y;
			ll ix = x, iy = y;
			ll sumx = 0, sumy = 0;
			while(true)
			{
				if(nxt[ix] != -1)  // x<y
				{
					ix = nxt[ix];
					if(ix == y)
					{
						cout << sumx << endl;
						nxt[x] = y;
						break;
					}
					sumx += ix;
				}
				if(nxt[iy] != -1)  // y<x
				{
					iy = nxt[iy];
					if(iy == x)
					{
						cout << sumy << endl;
						nxt[y] = x;
						break;
					}
					sumy += iy;
				}
			}
		}
	}

	return 0;
}

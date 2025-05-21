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

// ABC077 https://atcoder.jp/contests/abc077

/*
 * 自力で解ききれず、回答をチラ見した。
 * 桁DPやダイクストラ法っぽく解く。kzrnm氏の解説が近かった。
 *   https://atcoder.jp/contests/abc077/editorial/3823
 *   https://drken1215.hatenablog.com/entry/2021/07/30/160000
 *   https://blog.hamayanhamayan.com/entry/2017/11/06/021353
 * 
 * 答はKの倍数なので、mod Kが0である。
 * 全ての正整数を"Kで割った余り"で分類することを考えてみる。
 * ここでdist[mo]を、
 *   ke=X%K であるような正整数Xのうち、桁和の最小値
 * として定義すると、dist[0]が答。
 * 
 * ここで、全ての正整数は
 *   0から始めて、
 *   末尾に0～9の1文字を追加していく
 * ことで構築することができる。
 * ※実際には0から始めるとdist[0]=0となって不都合なので、1～9を初期値とする実装としている。
 * 末尾に値iを追加するとき、
 *   mod値 -> mo*10+i
 *   桁和 -> ke+i
 * となる。
 * この要領で桁和をキーとしてダイクストラ法を実装すると、解ける。
 * 
 * ※最初、dist[i][j]=x: i=x%K, j=keta(x) であるような最小値x
 *   として実装したがWA.
 *   これだと桁和ではなく値そのものをdistに持たせることになるが、
 *   値そのものはlong long型に収まるとは限らないので、上手くいかなかったと思われる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・倍数はmodでグループ分けしてみる。
 *   また2倍3倍4倍…とチェックしていくのではなく、桁DPの要領で桁を増やしていき、mod Kが0である要素について答える。
**/

int main(void)
{
	ll i;
	ll K; cin >> K;
	// dist[i]=*: i=X%modであるXのうち、桁和の最小値
	vector<ll> dist(K, INF64);

	using dist_v = pair<ll, ll>;  // {keta, mod}
	priority_queue<dist_v, vector<dist_v>, greater<dist_v>> que;

	for(i = 9; i >= 1; i--)  // distを上書きするので大きい方から
	{
		dist[i%K] = i;
		que.push({i, i%K});
	}

	while(!que.empty())
	{
		auto[ke, mo] = que.top();
		que.pop();

		if(ke > dist[mo]) continue;

		for(i = 0; i < 10; i++)  // xの末尾に新規桁としてiを追加
		{
			ll nxmo = (mo*10+i) % K;
			ll nxke = ke + i;
			if(nxke > 9*5) continue;  // Kの桁和は45(=99999)を超えない
			if(chmin(dist[nxmo], nxke))
			{
				que.push({nxke, nxmo});
			}
		}
	}
	cout << dist[0] << endl;

	return 0;
}

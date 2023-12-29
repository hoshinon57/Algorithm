#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC173 https://atcoder.jp/contests/abc173

/*
 * 優先度付きキューで解いた。
 * 参考：
 *   https://scrapbox.io/yu2ta7ka/AtCoder_ABC_173_D_-_Chat_in_a_Circle_(400%E7%82%B9)
 *   https://blog.hamayanhamayan.com/entry/2020/07/05/235409
 * 
 * 手順は以下。
 * ・Aを降順ソートする。
 * ・降順の優先度付きキューを用意し、a[0]だけ先だって入れておく。
 * ・a[1]以降の各a[i]について、
 *   キューから1つ取り出して解に加算。
 *   キューにa[i]を"2つ"入れる
 * 大きい方から入れた方がよりスコアを稼げて、またそのスコアを得られるのは2回まで、という観点。
 * (証明はできていない)
 * 
 * 公式解説を見たところ、スコア加算の流れをふまえて
 *   a[0]+a[1]+a[1]+a[2]+a[2]+...
 * でN-1回加算することと同じ。
 * 
 * [ACまでの思考の流れ]
 * ・難しい。どうやったら最適なスコアになるかを手を動かしながら考える。
 * ・大きい方から置いていくのが良さそう。
 *   これは未証明だがN=2や3のときを考えて割り切って進めた。
 * ・あるAについて、どのようにスコアが増えていくかを、手を動かしながら調べた。
 *   「現在得られるスコアのリスト」みたいなのを用意しておくと、
 *   常にリストから最大スコアを取得し、
 *   自身Aiの左右それぞれにスコアAiをリストに追加する感じか。
 *   優先度付きキューが使えそう。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end(), greater<ll>());

	priority_queue<ll> que;  // 大きい方が優先
	que.push(a[0]);
	ll ans = 0;
	for(i = 1; i < N; i++)  // キューが空になることは無い
	{
		auto n = que.top();
		que.pop();
		ans += n;
		que.push(a[i]);
		que.push(a[i]);
	}
	cout << ans << endl;

	return 0;
}

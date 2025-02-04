#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <array>
#include <set>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC391 https://atcoder.jp/contests/abc391

/*
 * 公式解説の1と同じ方針だった。
 *   https://atcoder.jp/contests/abc391/editorial/12085
 * ABC331-E(Set Meal)が類題かなと。
 * 
 * 数列A,B,Cを降順ソートしておくと、 f()=AB+BC+CAに対して
 *   f(ia,ib,ic) >= f(ia+1,ib  ,ic  )
 *   f(ia,ib,ic) >= f(ia  ,ib+1,ic  )
 *   f(ia,ib,ic) >= f(ia  ,ib  ,ic+1)
 * が成り立つ。
 * よってf(0,0,0)から初めて、現在の最大値に対して上記3通りを次の候補として登録していく。
 * これは優先度付きキューを用いる。
 * K回最大値を取得したら、それが答。
 * 
 * ただし、f(ia,ib,ic)はf(ia-1,ib,ic)とf(ia,ib-1,ic)など3パターンから登録されうるので、
 * 同じ組み合わせを2回以上登録しないよう注意が必要。
 * ⇒setで別途管理した。
 * 
 * ※コンテスト時、優先度付きキューではなくsetで次の最大値を求める実装としており、
 *   「同じ組み合わせは2回以上登録しないようにする」という処理を入れていなかった。
 *   https://atcoder.jp/contests/abc391/submissions/62299093
 *   振り返るとこれ正当性があるか…？ が自身なく（まあ大丈夫な気がするが）、あぶなかった…
 * 
 * [ACまでの思考の流れ]
 * ・「大きい方からK番目」で二分探索を思い浮かんだが、うまい実装が見つからず。
 * ・制約よりKにminが付いており、あやしい。
 *   O(K)とかO(Klog*)が通りそう。
 *   大きい方から順にK個列挙するアルゴリズムがあれば、それで通せるっぽい。
 *   ⇒これで考察を進めてAC.
 */

int main(void)
{
	ll i;
	ll N, K; cin >> N >> K;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	vector<ll> b(N); for(i = 0; i < N; i++) {cin >> b[i];}
	vector<ll> c(N); for(i = 0; i < N; i++) {cin >> c[i];}
	sort(a.begin(), a.end(), greater<ll>());
	sort(b.begin(), b.end(), greater<ll>());
	sort(c.begin(), c.end(), greater<ll>());

	priority_queue<array<ll, 4>> que;  // {num, ia, ib, ic}
	set<array<ll,3>> s;  // {ia, ib, ic}を登録

	// a[ia],b[ib],c[ic]の組み合わせが未登録であれば、問題文の式の値をキューに登録する
	auto f = [&](ll ia, ll ib, ll ic) -> void
	{
		if(ia >= N || ib >= N || ic >= N) return;  // 範囲外
		if(s.count({ia, ib, ic})) return;  // キューに積み済み
		s.insert({ia, ib, ic});
		ll num = a[ia]*b[ib] + b[ib]*c[ic] + c[ic]*a[ia];
		que.push({num, ia, ib, ic});
	};

	f(0, 0, 0);
	while(!que.empty())
	{
		auto [num, ia, ib, ic] = que.top();
		que.pop();
		if(--K == 0)
		{
			cout << num << endl;
			break;
		}
		f(ia+1, ib, ic);
		f(ia, ib+1, ic);
		f(ia, ib, ic+1);
	}

	return 0;
}

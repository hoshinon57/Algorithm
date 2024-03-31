#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC347 https://atcoder.jp/contests/abc347

/*
 * いやー苦戦した。
 * 例えばA+B=7のとき、D=3とD=10は同じことになる。
 * よってDを(A+B)で割った余りで置き換えても問題なく、置き換えた上でソートしておく。
 * 
 * ここでD[0]を休日の1日目に割り当ててみる。
 * Yes判定になるには、D[N-1]が休日であればよい。
 * これは D[N-1]-D[0]<A で判定できる。
 * 
 * D[1],D[2],... を休日の1日目に割り当てた場合の判定も必要。
 * データを2周させることで実装を簡易にした。[典型]
 * 
 * 答は、D[0]～D[N-1]それぞれについて上記判定を行い、1つでもOKならYes.
 * 全てNGならNoとなる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・入力DをA+Bで割った余りに置き換えても同じ、というのに気づけるかどうか。
 * ・D[0]と休日の1日目を合わせてみる。
 *   これはD[1]以降も同様の判定が必要。(※)
 *   円環になるので、データを2周させて持つ。[典型]
 *   (※)これをやらないと、A=2,B=5,D1=1,D2=7 のようなケースでWAとなる。D2を休日の1日目に割り当てる必要があるので。
 */

bool solve(void)
{
	int i;
	ll N, A, B; cin >> N >> A >> B;
	ll ab = A+B;
	vector<ll> d;
	for(i = 0; i < N; i++)
	{
		ll dd; cin >> dd;
		dd %= ab;
		d.push_back(dd);
		d.push_back(dd+ab);  // ddはA+BのMODにしているので、A+Bで足したものは全て2周目側となる
	}
	sort(d.begin(), d.end());  // N*2個

	for(i = 0; i < N; i++)
	{
		if(d[i+N-1] - d[i] < A) return true;
	}

	return false;
}

int main(void)
{
	YesNo(solve());
	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <set>
#include <queue>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC260 https://atcoder.jp/contests/abc260

/*
 * いもす法で解く。
 * 
 * i:1～Mについて、iを数列Sの左端とする。
 * Sが「良い数列」になるようなものを考える。
 * AorBを含み、かつA<Bであることから、multiset(以下ms)にてAを全て挿入すると
 * msの最大値(以下r)をSの末尾にすれば、Sは良い数列になる。
 * r以降、Mまで伸ばしても良い数列であり続けるので、
 *   最小値がiであるようなSについて、
 *   {i,i+1,...,r}  から
 *   {i,i+1,...,r,r+1,...,M} までの数列
 * が良い数列になる。
 * これはいもす法で管理するのが良い。
 * 
 * また、iを増やしていくとAの範囲から外れる。
 * 外れる際、msからAを消してBを追加することで、引き続き処理できる。
 * これは優先度付きキューで{A,B}を登録して実装した。
 * 
 * ※解説を見ると、multisetは不要でAの最大値を管理していれば良い模様。
 * 
 * [ACまでの思考の流れ]
 * ・どんな数列なら条件を満たすかを手で色々書いた。
 * ・[l,r]が条件を満たすなら、rを増やしていって[l,M]まで条件を満たしそう。
 *   ⇒いもす法っぽい。
 * ・[l,r]の最小範囲はどうなるだろうか。
 *   まずlを固定してみると、Aを全部含む、つまりAの最大値がそれっぽい？
 *   lがAより右にあれば、それはBを使う感じか。
 *   まずは全てのAを集合に入れておき、lを増やしていく過程でAが範囲から外れたらBに置き換える、でいけそう。
 */

int main(void)
{
	int i;
	int N, M; cin >> N >> M;
	using pii = pair<int,int>;
	priority_queue<pii, vector<pii>, greater<pii>> que;
	multiset<int> ms;
	int bmin = INF32;  // bの最小値
	for(i = 0; i < N; i++)
	{
		int a, b; cin >> a >> b;
		que.push({a, b});
		chmin(bmin, b);
		ms.insert(a);
	}
	vector<int> imos(M+5);
	for(i = 1; i <= bmin; i++)  // iを数列の左端とする, bminを超えると条件を満たさなくなるので、bminまでで良い
	{
		int r = *ms.rbegin();
		imos[r-i+1]++;  // 長さr-i+1から
		imos[M-i+1+1]--;  // 長さM-i+1まで  (最後の+1はいもす法のアルゴリズムによるもの)
		while(!que.empty())
		{
			// iの範囲から外れるようなAがあれば、代わりにBを使う
			auto p = que.top();
			if(p.first != i) break;
			que.pop();
			ms.erase(ms.find(p.first));
			ms.insert(p.second);
		}
	}
	for(i = 1; i <= M; i++)
	{
		imos[i] += imos[i-1];
		cout << imos[i] << " ";
	}
	cout << endl;

	return 0;
}

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

// ABC331 https://atcoder.jp/contests/abc331

/*
 * 主菜と副菜を{価格,idx}のペアであらかじめ降順ソートしておく。
 * 主菜x副菜のマトリクスを考えると、
 *   (0,0)マスが価格最大
 *   (0,1) or (1,0)が2番目の候補
 *   (2番目じゃない方) と (2番目から下および右のマス) が3番目の候補
 * となっていく。
 * これは価格最大のものから右と下へ探索範囲を広げることで、
 * 優先度付きキューを使えば価格が最大のものから順に取得することができる。
 * 
 * 優先度付きキューで取り出した組み合わせが「食べ合わせが悪いリスト」に含まれていなければ、そこが答。
 * L+1回取り出せば確実に答は得られ、Lは最大10^5のため十分に間に合う。
 * 
 * [ACまでの思考の流れ]
 * ・価格が高い順に組み合わせを求めていきたい ⇒ 優先度付きキューならできそう
 * ・NGリストがある ⇒ 最大10^5のため、価格が高い方からL+1個求めれば確実に答は出る
 */

int main(void)
{
	int i;
	int N, M, L; cin >> N >> M >> L;
	using pli = pair<ll, int>;  // {val, idx}
	vector<pli> a(N), b(M);
	for(i = 0; i < N; i++)
	{
		cin >> a[i].first;
		a[i].second = i;
	}
	for(i = 0; i < M; i++)
	{
		cin >> b[i].first;
		b[i].second = i;
	}
	sort(a.begin(), a.end(), greater<pli>());
	sort(b.begin(), b.end(), greater<pli>());
	set<pair<int,int>> ng;  // 組み合わせNG
	for(i = 0; i < L; i++)
	{
		int c, d; cin >> c >> d;
		c--; d--;
		ng.insert({c, d});
	}

	using elm = pair<ll, pair<int,int>>;  // {sum, {a_idx, b_idx}}
	priority_queue<elm> que;
	que.push({a[0].first+b[0].first, {0, 0}});
	set<pair<int,int>> chk;  // {a_idx, b_idx} チェック済
	while(true)  // 必ず探索完了する
	{
		auto [sum, idx] = que.top();
		auto [a_idx, b_idx] = idx;
		que.pop();

		if(chk.count({a_idx, b_idx}) > 0) continue;  // 既にチェック済
		chk.insert({a_idx, b_idx});

		// 組み合わせOKなら出力して終了
		if(ng.count({a[a_idx].second, b[b_idx].second}) == 0)
		{
			cout << a[a_idx].first + b[b_idx].first << endl;
			break;
		}

		if(a_idx+1 < N)  // 次の主菜を登録
		{
			que.push({a[a_idx+1].first+b[b_idx].first, {a_idx+1, b_idx}});
		}
		if(b_idx+1 < M)  // 次の副菜を登録
		{
			que.push({a[a_idx].first+b[b_idx+1].first, {a_idx, b_idx+1}});
		}
	}

	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <deque>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_3_D

/*
 * スライド最小値(最大値)をdequeを用いてO(N)で解く。
 *   https://qiita.com/kuuso1/items/318d42cd089a49eeb332
 *   https://perogram.hateblo.jp/entry/2020/09/24/075511
 * 
 * セグ木やsetでも解ける(はず)が、O(NlogN)になる(はず)。
 * 
 * 「ヒストグラム中の最大正方形問題」と似た考え方。
 * dequeを用いて、中身が単調増加になるように積んでいく。
 * (実際にはa[i]ではなくidxであるiを保持している)
 * dequeの先頭が現在見ているウィンドウの最小値となっている。
 * 
 * dequeに単調増加で値を持たせることで、
 *   現在のウィンドウの最小値
 *   (ウィンドウはスライドしていくので)将来の最小値の候補
 * が昇順に並ぶことになる。
 * ここでdeque内の要素で、後から入ってきたa[i]より大きいものが存在する場合、それは最小値にはなりえない。
 * つまりdequeから取り除くことができる。
 * 
 * [ポイント]
 * ・実装では、dequeにはa[i]ではなくidxであるiを持たせている。
 * ・dequeに出入りするのは各要素ごと高々1回なので、全体でO(N)になる。
 */

int main(void)
{
	int i;
	int N, L; cin >> N >> L;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	deque<int> de;  // de={a[]のidx}
	vector<int> ans;

	for(i = 0; i < N; i++)  // a[i]を加え、a[i-L]を取り除く
	{
		// a[i-L]を取り除く
		if(i-L >= 0 && de.front() == i-L) de.pop_front();

		// デックが単調増加になるよう積んでいく
		while(de.size() > 0 && a[de.back()] > a[i]) de.pop_back();
		de.push_back(i);

		if(i >= L-1) ans.push_back(a[de.front()]);
	}

	int sz_ = (int)ans.size();
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}

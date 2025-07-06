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

// ABC395 https://atcoder.jp/contests/abc395

/*
 * 結構難しい。
 * クエリ2のハト交換が厄介。愚直にやるともちろんTLEなので、何らか簡単なやり方を考える必要がある。
 * ハトの移動をさせたくない気持ちになるので、巣について
 *   巣にかかっているフダの番号
 *   巣の(内部的な)番号
 * の2つを管理すると、クエリ2についてはフダを交換する方針にできるので、上手くいく。
 * 
 * ということで、以下3つの情報を管理する。
 *   h_idx[i]: ハトiがいる巣の番号
 *   fuda[i]: 巣iにかかっているフダ番号
 *   f_idx[i]: フダiがかかっている巣の番号(逆引き)
 * クエリでは上記で決めた「フダの番号」が指定されるので、そこから逆引き用のf_idxが必須。
 * 
 * クエリ1はh_idx[a]を、フダbがかかっている巣に移動させたいので、f_idx[b]に更新する。
 * クエリ2はフダ番号と逆引き用をそれぞれ更新する必要があり、
 *   fuda[f_idx[a]] と fuda[f_idx[b]]
 *   f_idx[a] と f_idx[b]
 * をそれぞれswapする。
 * 
 * [ACまでの思考の流れ]
 * ・クエリ処理で愚直にやるとTLEするものは、データ構造を検討する。
 *   例：要素の"別名"を用意し、また別名から本来の要素の逆引きを用意する。
**/

int main(void)
{
	// 1-indexed
	int i;
	int N, Q; cin >> N >> Q;
	vector<int> h_idx(N+1);  // h_idx[i]: ハトiがいる巣の番号
	vector<int> fuda(N+1);  // fuda[i]: 巣iにかかっているフダ番号
	vector<int> f_idx(N+1);  // f_idx[i]: フダiがかかっている巣の番号(逆引き)
	for(i = 1; i < N+1; i++)
	{
		h_idx[i] = i;
		fuda[i] = i;
		f_idx[i] = i;
	}

	while(Q > 0)
	{
		Q--;
		int ki; cin >> ki;
		if(ki == 1)
		{
			int a, b; cin >> a >> b;
			h_idx[a] = f_idx[b];
		}
		if(ki == 2)
		{
			int a, b; cin >> a >> b;
			int sa = f_idx[a];  // フダaがかかっている巣の番号
			int sb = f_idx[b];
			swap(fuda[sa], fuda[sb]);
			swap(f_idx[a], f_idx[b]);
		}
		if(ki == 3)
		{
			int a; cin >> a;
			cout << fuda[h_idx[a]] << endl;
		}
	}

	return 0;
}

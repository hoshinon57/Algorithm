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

// ABC282 https://atcoder.jp/contests/abc282

/*
 * 公式解説では長さ1,2,4,8,... としているが、自分が最初に考えたのは1,3,7,15,... だった。
 *   https://atcoder.jp/contests/abc282/editorial/5403
 *   https://ikatakos.com/pot/programming_algorithm/data_structure/sparse_table
 *   https://drken1215.hatenablog.com/entry/2022/12/29/112939
 * 
 * フェイズ1で作る区間の長さlenを {1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047} とし、
 * それぞれの長さについて左端を1ずつずらしながら区間を作っていく。
 * するとクエリでどんな[L,R]が来ても回答できる。
 * 例：[L,R]=[1,4]なら[1,3]と[2,4]
 *     [L,R]=[1,6]なら[1,3]と[4,6]
 * 
 * lenの種類は11, 左端を1ずつ全探索したときの個数はN(max 4000)なので、
 * 11*4000<50000で整数Mの制約を満たす。
 * 
 * クエリ[L,R]に対し、長さ(R-L+1)以下で最大のlenを使えば良い。
 * 実装ではupper_bound()を用いた。
 * 
 * [備忘録]
 * ・何らか値をたくさん作り(本問では区間[l,r])、後で区間を使うときに「何番目の区間か？」の情報が必要なとき、
 *   値の作成時に逆引きを用意しておくと処理が楽。(mapなり、単なる配列なり)
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・区間を1,3,7,...とすれば上手く行けそう、までは思いついたが、
 *   左端を1つずつ全て選ぶという考えが思いつけなかった。
 */

int idx[4001][4001];

int main(void)
{
	vector<int> len = {1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047};
	int N; cin >> N;
	vector<pair<int,int>> lr;
	for(auto &e : len)
	{
		for(int l = 1; l+e-1 <= N; l++)  // 全ての左端について(l,r)を作成
		{
			int r = l+e-1;
			lr.push_back({l, r});
			idx[l][r] = (int)lr.size();
		}
	}
	cout << lr.size() << endl;  // M
	for(auto [l,r] : lr) cout << l << " " << r << endl;

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		int L, R; cin >> L >> R;
		auto itr = upper_bound(len.begin(), len.end(), R-L+1) - 1;  // [L,R]の長さ以下で最大のlen
		int a = idx[L][L+(*itr)-1];
		int b = idx[R-(*itr)+1][R];
		cout << a << " " << b << endl;
	}

	return 0;
}

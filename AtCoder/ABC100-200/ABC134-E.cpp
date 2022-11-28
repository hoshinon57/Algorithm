#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 100000;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題55より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc134

/*
 * 最長増加部分列（以下、LIS）の考え方で解く。
 * 
 * {2,1,4,5,3} という入力があったときに、
 *  2,4,5
 *  1,3
 * の2つに分類するのが最適っぽい。よって
 * ・まずLISを1つ求める
 * ・使わなかった要素のみを残して、またLISを求める
 * ・全要素を使ったら終了、LISの数が答
 * を考えたが、LIS1つ辺りの計算量がO(NlogN)で、ワーストケースでこれをN回繰り返すことになるため間に合わない。
 * 
 * よって1ループで完了させる方法を考える。
 * 上記で4までを処理したときに、
 *  2,4
 *  1
 * 次の5はどちらに入れることも可能だが、より「つぶしが効く」のは4、つまり値が大きい方。
 * よって各LISの末尾の数値の一覧に対して、次の数値をAとすると
 *   A未満で最大のもの
 * と考えることができる。
 * これはlower_bound(A)の1つ前の要素となる。
 * （A未満が無い場合、新規にLISを構築する）
 * 
 * 各LISの末尾の要素以外は不要なので、
 *  2,4
 *  1
 * なら{1,4}といった配列を持てば十分。
 * 入力は0以上なので、配列の初期値を-1にすれば番兵の代わりとなり処理が簡潔になる。
 * 
 * 答は、上記配列のうち値が-1以外である要素の数。
 * 配列は昇順ソートが保たれているので、これもlower_bound()で計算できる。
 * 
 * 以下記事と考え方は同じ。
 * https://sarashina-nikki65.hatenablog.com/entry/2019/07/26/130917
 * その他参考：https://drken1215.hatenablog.com/entry/2020/12/25/184700
 * 
 * 計算量はO(NlogN).
 */

int main(void)
{
	int i;
	int N;
	cin >> N;

	// LISの一覧を構築したときに、各LISの末尾（＝最大）の数値を持つイメージ
	// -1はLIS未構築の部分
	vector<int> col(N, -1);

	for(i = 0; i < N; i++)
	{
		int a;
		cin >> a;
		// col[]の中で、a未満で最大の要素を探したい
		// これはlower_bound(a)をして1つ手前の要素となる
		auto itr = lower_bound(col.begin(), col.end(), a);
		itr--;  // col[]は要素数N, かつ-1で初期化しているため無条件でデクリメントしても安全
		*itr = a;
		// col[]は昇順ソートが保たれる
	}

#if 0
	for(auto &e : col)
	{
		cout << e << " ";
	}
	cout << endl;
#endif

	// -1以外の要素の数が答となる
	int answer = col.end() - lower_bound(col.begin(), col.end(), 0);
	cout << answer << endl;

	return 0;
}

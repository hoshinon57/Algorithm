#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC057 https://atcoder.jp/contests/abc057

/*
 * 二項係数を用いて解く。各種解説と同じ方針だった。
 *   https://drken1215.hatenablog.com/entry/2019/02/16/000900
 *   https://img.atcoder.jp/abc057/editorial.pdf
 * 
 * ナップザック問題？ いや価値vが10^15とかだから無理だ。
 * 半分全列挙？ うーん…
 * となり、そもそも平均値が最大なのはどういうケースだろう？を考えた。
 * 
 * とりあえずは大きい順にソートしておこう。[典型]
 * v=2,2,1,1,1,1,1 A=4,B=6
 * A個以上なので、"2,2,1,1"までは確定。
 * しかしここから1を追加で選ぶと平均値は下がってしまう。ということは、選ぶ個数はA個しかありえない。
 * 一方で最後に選んだ値1は、他の1でも良い。
 * ⇒これは「A個選んだときの1の個数=2」と「全体での1の個数=5」より、C(5,2)となる。
 * 
 * んー、Bには意味が無いのか？と考えると、
 * v=2,2,2,2,2,1,1  A=3,B=6
 * のとき、2を3個 or 4個 or 5個選ぶケースで平均値が一致する。
 * つまりv[0]=v[A-1]のケース。
 * ⇒これは「何個選ぶか=i」と「全体での2の個数=5」より、ΣC(5,i)となる。
 *   何個選ぶか、とBとの大小関係に注意。
 * 
 * 以上の流れで考えてAC.
**/

// MODではなく正確な値を求めるような問題において、
// パスカルの法則を用いてnCkを計算するライブラリ
const int NCK_PASCAL_SIZE = 55;  // nCkのnの最大値 問題文の制約に合わせる
ll nCk_pascal_memo[NCK_PASCAL_SIZE][NCK_PASCAL_SIZE];
void nCk_Pascal_Init(void)
{
	int i, j;
	for(i = 0; i < NCK_PASCAL_SIZE; i++)
	{
		for(j = 0; j < NCK_PASCAL_SIZE; j++)
		{
			nCk_pascal_memo[i][j] = -1;
		}
	}
}
ll nCk_Pascal(int n, int k)
{
	// nCr = (n-1)Cr + (n-1)C(r-1)    ※パスカルの法則
	if(k == 0) return 1;
	if(n == k) return 1;
	if(nCk_pascal_memo[n][k] != -1) return nCk_pascal_memo[n][k];
	return nCk_pascal_memo[n][k] = nCk_Pascal(n-1, k) + nCk_Pascal(n-1, k-1);
}

int main(void)
{
	nCk_Pascal_Init();
	ll i;
	ll N, A, B; cin >> N >> A >> B;
	vector<ll> v(N); for(i = 0; i < N; i++) {cin >> v[i];}
	sort(v.begin(), v.end(), greater<ll>());

	// 上からA個を貪欲、の方針
	ll sum = 0, cnt = 0;
	// 平均値は先だって計算できる
	sum = reduce(v.begin(), v.begin()+A);
	cout << std::fixed << std::setprecision(20);
	cout << (double)sum / A << endl;

	if(v[0] != v[A-1])
	{
		ll tmp1 = count(v.begin(), v.begin()+A, v[A-1]);  // 貪欲でv[A-1]を何個選んでいるか
		ll tmp2 = count(v.begin(), v.end(), v[A-1]);  // v[A-1]が全体で何個あるか
		cnt = nCk_Pascal(tmp2, tmp1);
		cout << cnt << endl;
	}
	else  // A個が全一致
	{
		ll tmp2 = count(v.begin(), v.end(), v[A-1]);  // v[A-1]が全体で何個あるか
		for(i = A; i <= B; i++)  // 選ぶ個数i
		{
			if(i > tmp2) break;  // 上限チェック
			cnt += nCk_Pascal(tmp2, i);
		}
		cout << cnt << endl;
	}

	return 0;
}

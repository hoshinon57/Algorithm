#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <bitset>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC348 https://atcoder.jp/contests/abc348

/*
 * 自力で解けず、解説を見た。
 * bitsetによる高速化と、自分は考え方として主客転倒的な要素も必要だと感じた。
 *   https://atcoder.jp/contests/abc348/editorial/9731
 *   https://qiita.com/sinzyousan/items/43e5531a644b0db3bd5e
 * 
 * まず最初に、「AiとAjの要素M個を全てチェックし、一致数を数える。これをi,jで全探索する」
 * という愚直方針を思いつくが、これとは別の方針(以下)を思いつく必要がある。
 * 
 * 各Aについてj個目の要素を見て、
 *   lst[x]={*,*,*}: x=a[i][j]となるiを登録
 * として定義するlstを更新していく。
 * 次に
 *   sim[i][j]: AiとAjの一致数が奇数なら1
 * として定義するsimを更新する。
 * M個目の要素まで見た後、sim[]が1である要素数をカウントすればよい(※)
 * 
 * 上記を実装したのが以下で、TLEとなる。
 * https://atcoder.jp/contests/abc348/submissions/56142591
 * 
 * ここからbitsetを用いた高速化を行う。
 * lst[x]はNの数だけbitを用意してあげれば表現でき、またsim[i]も同様。
 * 詳細は実装を参照。
 * 
 * (※)
 * Mが奇数のとき、AiとAiが似ているとカウントされているため減算必要。
 * またi<jという条件があるので/2が必要。対角線で見てi>jの要素を削除するイメージ。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・bitset高速化、という手法。
 *   これを使うタイミングを思いつくには、
 *     条件を満たす個数が偶数/奇数である (偶奇なら0/1で表せる)
 *     要素間で条件を満たす数を求める
 *   といった問題のときにbitsetが使えるかも、と覚えておく。
 */

int main(void)
{
	// 0-indexed
	int i, j;
	int N, M; cin >> N >> M;
	vector<vector<int>> a(N, vector<int>(M));
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < M; j++) cin >> a[i][j];
	}

	const int MAXA = 999;
	const int MAXN = 2000;
	vector<bitset<MAXN>> sim(N);  // sim[i]=(長さNのbit): jビット目をAjに割り当て、AiとAjの一致数が奇数なら1
	vector<bitset<MAXN>> lst(MAXA+1);  // lst[x]=(長さNのbit): iビット目をAiに割り当て、a[i][*]=xであるiの一覧
	for(j = 0; j < M; j++)  // a[*][j]について調べる
	{
		for(i = 0; i < N; i++) lst[a[i][j]].set(i);
		for(i = 0; i < N; i++) sim[i] ^= lst[a[i][j]];

		// [memo]
		// 毎ループごとlstを定義するオーバーヘッドを避けるため、ループ外で定義して毎ループbitを戻す
		// ただし毎ループごと定義する実装でも、実行時間はほとんど変化しなかった
		for(i = 0; i < N; i++) lst[a[i][j]].reset(i);
	}

	int ans = 0;
	for(i = 0; i < N; i++) ans += sim[i].count();
	if(M%2 == 1) ans -= N;  // Mが奇数のとき、AiとAiが似ているとカウントされているため減算
	ans /= 2;  // i<jという条件があるので、対角線で見てi>jの要素を削除
	cout << ans << endl;

	return 0;
}

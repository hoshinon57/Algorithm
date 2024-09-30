#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>
#include <list>
#include <map>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <cassert>
#include <numeric>
using namespace std;

void _lambda_(void);
void _vector_(void);
void _string_(void);
void _set_multiset_(void);
void _priority_queue_(void);
void _list_(void);
void _map_(void);
void _pair_(void);
void _deque_(void);
void _mod_(void);
void _pow_(void);
void _unordered_set_(void);
void _unordered_map_(void);
void _stl_(void);
void _structured_bindings_(void);
void _aggregate_(void);
void _optimize_(void);
void _tie_(void);
void _bitset_(void);
void _mex_(void);
void _math_(void);
void _value_with_index_(void);
void _zobrist_hash_(void);
void _largest_rectangle_histogram_(void);

int main(void)
{
	_lambda_();
	_vector_();
	_priority_queue_();
	_set_multiset_();
	_pow_();
	_list_();
	_map_();
	_pair_();
	_deque_();
	_unordered_set_();
	_unordered_map_();
	_mod_();
	_string_();
	_stl_();
	_structured_bindings_();
	_aggregate_();
	_optimize_();
	_tie_();
	_bitset_();
	_mex_();
	_math_();
	_value_with_index_();

	/*
	AtCoderにてWAが出た場合のチェックポイント
	特に、テストケースのうち半分がAC, もう半分がWAといった場合、コーナーケースの確認と合わせて以下をチェック。
	(1)変数はint型で良いか。long longにする必要は無いか。
	   long longでも途中でオーバーフローしてしまう実装になっていないか。
	   入力はint型で収まっても、出力はlong longにする必要があるかもしれない。何通りあるか、など。
	   ★制約に10^9が見えたらlong longにしてしまった方が良い。 (ABC320-Dでの反省点)
	   ★制約が10^5でも、2次元ならlong longにしてしまった方が良い。 (ABC346-Eでの反省点)
	(2)「998244353 で割った余りを求めてください」のとき、計算の各所でMOD計算をしているか。
	   入力値の時点でMODを超えていた場合、先だってMOD計算が必要かもしれない。 (ex:ABC332-F)
	(3)WAとなった場合、WAになるような入力例を何としても作る。
	(4)二分探索のとき、l,rの初期値は適切か、判定関数は適切かを再考する。初期値は特にlを見直す。
	(5)1が立っているbitをシフト演算を用いて計算する場合、
	   1にlong long等のキャストを付ける必要は無いか。
	(6)lower_bound, upper_boundを使うときは要素がソートされているか。
	(7)INF32,INF64を使って複数要素のminを求めるとき、INFの値は適切か。要素の方が大きいケースは無いか。

	コーナーケースは、制約をぐっとにらんで考える。
	1 <= M <= N <= 2*10^5なら、M=Nのケースを試したか。M=1のケースを試したか。
	*/

	/*
	ABCのC,D辺りの問題で、実装が重めの問題リスト。実装の練習に。
	ABC359-D
	ABC345-D
	ABC326-D
	ABC322-E
	ABC322-D
	ABC320-C
	ABC319-C
	ABC315-D
	ABC307-C
	ABC268-D
	ABC260-D
	ABC193-D
	ABC175-D
	ABC155-D
	*/

	/*
	ABC265-E問題(https://atcoder.jp/contests/abc265/tasks/abc265_e)にて
	配るDPでの自分の実装(ABC265-E.cpp)では
	---------
	300 0
	0 0 1 0 0 1
	---------
	の入力例がローカル環境では10秒弱かかったのが、AtCoderでは1.6秒程度と大きく差があった。
	自分の立てた方針や実装に自信があれば、いちど提出しても良いかも。
	(連想配列で大量の要素を追加するケースとか？)
	*/

	/*
	数値型の範囲
	int	-2147483648～2147483647  およそ-2*10^9～+2*10^9   2^32(1<<32)
	long long  -9223372036854775808～9223372036854775807  およそ-9*10^18～+9*10^18  2^64(1<<64)

	一時的にlong long型を超える範囲で計算したい場合、gccでは以下が使える。cin/coutは不可能。
	  __int128_t  符号つき128bit
	  __uint128_t 符号なし128bit
	https://kenkoooo.hatenablog.com/entry/2016/11/30/163533
	https://qiita.com/trineutron/items/3ad05e0aec602a56bc30
	*/

	/*
	小数の出力
	#include <iomanip> が必要
	setprecisionにて桁数指定
	fixedにより小数点以下の指定となる
	cout << std::fixed << std::setprecision(10);
	*/

	/*
	0埋めの出力
	#include <iomanip> が必要
	cout << std::setfill('0') << std::setw(4);  // 4桁の0埋め
	cout << 12 << endl;  // "0012"
	*/

	/*
	N個の中から好きな数だけ選ぶ → bit全探索
	for( i = 0; i < (1<<N); i++ )  // iについて、bitが1になっているものを選ぶ
	{
		for( j = 0; j < N; j++ )
		{
			if( ((i>>j)&1) == 0 ) continue;  // 演算子の優先順位注意 &1を()で囲む必要あり
			// 以降、j番目について選んだ場合の処理
		}
	}
	[関連する問題]
	ABC002-D
	ABC159-E
	ABC197-C
	ABC200-D
	ABC219-E
	ABC264-C
	ABC269-C
	ABC289-C
	ABC345-D
	ABC356-C
	典型90-63
	典型90-86
	アルゴリズムと数学 068 https://atcoder.jp/contests/math-and-algorithm/
	*/

	/*
	__builtin_popcount(x) : xを2進数で表したときに、bitが1になる個数を返す
	追加のincludeは不要とのこと。
	----
	cout << "7:" << __builtin_popcount(7) << endl;  // 3
	cout << "16:" << __builtin_popcount(16) << endl;  // 1
	cout << "255:" << __builtin_popcount(255) << endl;  // 8
	----
	__builtin_popcountll(x) : 引数xがlong long型の場合

	＞この関数は gcc で利用可能ですが、Visual Studio 2019 などでは使えません。
	とのこと。
	https://qiita.com/e869120/items/518297c6816adb67f9a5
	*/

	/*
	dp(動的計画法)の自分なりのコツ
	// dp[i][j]
	// i日目まで終わったときに、   i:1～N
	// かつi日目に 0:A, 1:B, 2:C の活動を行ったときの、 j:0～2
	// 幸福度の最大値
	のように、添字の意味と取りうる範囲をメモしておく。

	もらうDP/配るDPのどちらを選んでも、
	・dp[]の定義は同じ
	・答(出力すべき値)も同じ
	としてまず考える。
	それを踏まえ、もらう/配るのどちらを選んだかによって、遷移を考えていく。

	期待値DPについて：
	後ろから求めていくのが良さそう。
	  dp[i]:iの状態からNになるまでの回数の期待値
	  dp[N]=0
	  dp[1]を最終的に求めたい
	のような感じで。
	また、もらうDPで解くのが良さそう。
	ABC263-Eを「前から、配るDP」で解こうとしたら、どうしても上手くいかなかった。
	参考問題：
	  https://atcoder.jp/contests/dp/tasks/dp_j
	  https://atcoder.jp/contests/abc263/tasks/abc263_e
	
	ABC265-E問題(https://atcoder.jp/contests/abc265/tasks/abc265_e)のように
	各フェーズの結果を配列ではなく連想配列で持つ場合にて、
	  vector<map<pair<ll, ll>, ll>> dp;
	のようにvectorで全フェーズの結果を持たせると、TLEになるテストケースがあった。
	  map<pair<ll, ll>, ll> dp, new_dp;
	2フェーズだけ持ち、毎回swap(dp, new_dp)させると、TLEは解消した。
	原因は不明だが、前者だとメモリを300MBほど使っており、
	vector<map<...>>で大量のメモリを消費する解法は、AtCoderでは避けた方が良いのかもしれない。

	[ゲームのDP]
	自分と相手が交互に行動する場合、dp配列も[2]として定義した方が良い。
	「次が{0:自ターン, 1:相手ターン}のときの最大スコア」といったイメージ。
	参考問題：
	  DPまとめコンテスト K
	  ABC195-E (DP配列は自他で区別はしていない)
	  ABC201-D
	  ABC270-D
	  ABC298-E

	[確率DP、参考問題]
	ABC184-D
	ABC275-E
	ABC280-E
	ABC298-E
	ABC300-E
	ABC310-F 確率DP&bitDP的な
	ABC314-E
	ABC323-E
	ABC326-E
	ABC350-E  自己ループあり
	ABC360-E  場合の数を用いて遷移式を考える

	[LIS/最長増加部分列]
	考え方として、dp[i][j]を "i個目まで見て、長さjであるLISの末尾の値の最小値" というのを覚えておく。
	ABC237-FのYouTube公式解説にて。

	[in-place DP(inline DP), 参考問題]
	ABC203-E
	ABC372-F

	[DPの復元、参考問題]
	ABC200-D (ただし想定解法は異なる)
	典型90-56

	[過去いくつかの状態を持ち、スライドさせていくようなDP]
	ABC359-D
	JOI Pasta https://atcoder.jp/contests/joi2012yo/tasks/joi2012yo_d
	*/

	/*
	等差数列
	初項a, 末項l, 公差d, 項数nの等差数列の和は、
	  n*(a+l)/2
	  n*(a + (a+(n-1)*d) )/2      ※末項l=a+(n-1)*d
	https://www.kwansei.ac.jp/hs/z90010/sugakua/suuretu/tousasum/tousasum.htm
	*/

	/*
	累積和(1次元)
	参考：https://qiita.com/drken/items/56a6b68edef8fc605821
	a[]に対する累積和a_sum[]は、以下のように計算すると良い。
	a_sum[0] = 0;
	for(i = 0; i < N; i++)
		a_sum[i+1] = a_sum[i] + a[i];
	
	s0=0
	s1=a0
	s2=a0+a1
	s3=a0+a1+a2
	sN=a0+a1+a2+...+a(N-1)
	★0-indexedとして、a[]の先頭i個の総和がs[i]になる。
	
	するとa[]の[l,r)の半開区間の和は a_sum[r] - a_sum[l] で計算できる。
	[3,5)であれば a_sum[5] - a_sum[3] で、a[3]+a[4]の意味になる。

	[l,r]であれば a_sum[r+1] - a_sum[l] とすれば良い。
	[3,5]であれば a_sum[5+1] - a_sum[3] で、a[3]+a[4]+a[5]の意味になる。

	[参考/両端から累積和]
	ABC181-E
	ABC334-C
	ABC346-D

	[その他]
	ABC182-D 累積和のMAX
	*/

	/*
	累積和(2次元)
	参考：https://qiita.com/drken/items/56a6b68edef8fc605821

	a_sum[y][x]を、[0,y) x [0,x) の半開区間における総和と考える。
	★1次元の場合と同様、a_sum[y][x]を "先頭y個 × 先頭x個" の総和でイメージする。

	a_sumの構築は以下のように計算できる。
	for(i = 0; i < M; i++)  // y
	{
		for(j = 0; j < N; j++)  // x
		{
			a_sum[y+1][x+1] = a_sum[y+1][x] + a_sum[y][x+1] - a_sum[y][x] + a[y][x]
		}
	}

	するとa[]の[y1,y2) x [x1,x2)の半開区間の和は以下となる。
	a_sum[y2][x2] - sum[y2][x1] - sum[y1][x2] + sum[y1][x1]

	[y1,y2] x [x1,x2]の閉区間のときは、y2,x2を+1すれば良い。
	
	[関連する問題]
	ABC130-E (累積和そのものではないが、考え方が累積和)
	ABC269-F (累積和そのものではないが、考え方が累積和)
	ABC278-E
	ABC313-E (別解)
	*/

	/*
	いもす法
	記録のフェーズとシミュレートのフェーズ、そして解の算出のフェーズに分ける。
	ABC256-D-2.cppを参照。
	https://imoz.jp/algorithms/imos_method.html
	[関連する問題]
	ABC153-F
	ABC183-D
	ABC260-E
	ABC268-E
	ABC338-D
	*/

	/*
	インタラクティブ問題
	ABC269-E
	ABC299-D
	ABC313-D
	ABC337-E
	*/

	/*
	主客転倒
	「和の形の数え上げ」は主客転倒に置き換えると解けるケースがありそう。
	  https://physics0523.hatenablog.com/entry/2020/01/12/052513
	[関連する問題]
	ABC127-E
	ABC140-E 難しかったけど良問
	ABC151-E
	ABC162-E 難しかったけど良問
	ABC169-F
	ABC173-F
	ABC206-E 難しかった
	ABC214-D
	ABC224-F
	ABC297-F 難しかった
	ABC306-F 難しかったが何とか自力AC
	ABC371-E 余事象も考察に必要 良問
	典型90-39
	*/

	/*
	マージテク
	データ構造をマージする一般的なテクニック。
	2つのグループをマージするとき、小さい方から大きい方へマージすることで計算量をO(NlogN)に抑えられる。
	ライブラリは other_library.cpp - set_merge() を参照
	[関連する問題]
	ABC183-F
	ABC329-F
	*/

	/*
	円環での距離について (内容がピンポイントすぎる気もするが…、ABC338-Dの反省)
	N頂点 0,1,...N-1 について、0,1,2,... を時計回り、逆方向を反時計回りと表現する。
	頂点a->b間の距離は、a<bとしても一般性を失わず、
	  時計回り：b-a
	  反時計回り：a+N-b
	となる。(2つを加算するとNになる)
	*/

	/*
	Grundy数
	Grundy数は、現状態からの遷移先のGrundy数の集合について、そのmexとなる。
    山が複数ある場合は、山ごとのGrundy数のxorを計算し、それが0以外なら勝ち、0なら負け。
	  https://yang33-kassa.hatenablog.com/entry/2017/12/21/202812
	  https://augusuto04.hatenablog.com/entry/2015/05/03/233606  その2,3もある
	  https://kyopro-friends.hatenablog.com/entry/2023/04/12/195012  xorとなる理由
	[関連する問題]
	典型90-31
	*/

	/*
	包除原理
	4つ以上の包除原理は、集合の数の偶奇によって加算/減算が切り替わる。奇数なら答に加算、偶数なら答に減算など。
	※問題によって偶奇が逆になったりする。
	[参考記事]
	https://drken1215.hatenablog.com/entry/2022/04/03/212800
	https://satanic0258.hatenablog.com/entry/2016/04/10/104524
	[関連する問題]
	ABC152-F
	ABC246-F
	ABC361-F
	典型90-80
	アルゴリズムと数学 068 https://atcoder.jp/contests/math-and-algorithm/
	EDPC-Y
	*/

	/*
	半分全列挙
	[関連する問題]
	ABC271-F
	ABC326-F
	典型90-51
	*/

	/*
	二重シグマ(Σ)
	ΣΣで1つ目のΣがi=1～, 2つ目がj=i+1～ の場合(ex:https://atcoder.jp/contests/abc353/tasks/abc353_c)
	i<jである(i,j)の全ペアを表す。(j,i)および(i,i)は含まれない。
	N個から2つを選ぶので、ペア数は NC2=N*(N-1)/2 となる。
	N*Nの表において、対角線より右上側にあるセルを表すイメージ。
	もしくは、i<jにて頂点i->jへの辺を張るイメージもアリかも。
	(ABC356-Eの反省)N*Nの表を作ってイメージすべし

	ΣΣで1つ目がi=1～, 2つ目がj=1～の場合、(j,i)および(i,i)も含む。
	N*Nの表において全セルを表すイメージ。
	ここから(i,i)ぶんを除外して半分にすると、最初に書いたΣΣと同値になる。
	[関連する問題]
	ABC353-C
	ABC353-D
	ABC356-E 累積和や調和級数など、難しかったが良問
	ABC371-E 主客転倒や余事象が絡む良問
	*/

	/*
	ac-library(AtCoder Library) 検索用:ACL
	https://atcoder.github.io/ac-library/master/document_ja/
	https://github.com/atcoder/ac-library/tree/master?tab=readme-ov-file
	#include <atcoder/all>
	using namespace atcoder;

	modintのみを使う場合
	https://atcoder.github.io/ac-library/master/document_ja/modint.html
	#include <atcoder/modint>
	using namespace atcoder;
	using mint = modint998244353;
	-> or modint1000000007, modint(modがfixedでない場合)

	[中国剰余定理]
	crt()
	https://atcoder.github.io/ac-library/production/document_ja/math.html
	三元以上の場合、m1,m2,...,miについて「どの2つも互いに素」であること。

	[使用した問題]
	ABC286-F crt(中国剰余定理)
	ABC362-G suffix array
	modint:ABC372-F, (多いので横方向に列挙していく)
	*/
	return 0;
}

void _lambda_(void)
{
	cout << "-----lambda-----" << endl;
	/*
	ラムダ式の書き方メモ
	参考：
	https://rinatz.github.io/cpp-book/ch02-09-lambda-expressions/
	*/

	// -> int は戻り値の型
	// 型推論に任せるならば省略可能
	auto func = [](int a, int b) -> int
	{
		return a + b;
	};
	// 最後に ; が必要
	assert(func(10,20) == 30);
	
	// ラムダ式の外にある変数を使いたい場合、コピーキャプチャか参照キャプチャを使う
	// 前者は[=], 後者は[&]
	// また特定の変数のみを使いたい場合は[&num1, num2]など。
	// 基本的には[&]を使うことになりそう

	// C++14以降、以下の形で自己再帰を書ける
	// https://yohhoy.hatenadiary.jp/entry/20211025/p1
	auto func2 = [&](auto &self, int n) -> int
	{
		if(n == 1) return 1;
		return n + self(self, n-1);  // n+(n-1)+(n-2)+...
	};
	assert(func2(func2, 1) == 1);
	assert(func2(func2, 4) == 10);	
}

// a,bの最大公約数を返す
// C++17から標準ライブラリに搭載されているとのこと
// 参考：https://cpprefjp.github.io/reference/numeric/gcd.html
long long gcd(long long a, long long b)
{
	if(b == 0) return a;
	else return gcd(b, a%b);
}

void _vector_(void)
{
	cout << "-----vector-----" << endl;
	vector<int> a = {3,1,4,1,5,9};  // 定義時に要素を指定
	/*
	vectorの二次元配列
	a[X][Y]
	vector<vector<int>> a(X, vector<int>(Y));

	a[X][Y]  0で初期化
	vector<vector<int>> a(X, vector<int>(Y, 0));

	定義後に、要素数をa[X][Y]に設定
	vector<vector<int>> a;
	a.assign(X, vector<int>(Y, 0));  // 0で初期化
	a.resize(X, vector<int>(Y));

	a[X]
	a[0], a[1], ... それぞれについてpush_back()していく
	vector<vector<int>> a(X, vector<int>());
	vector<vector<int>> a(X);
	*/
	vector<int> b(a.begin()+2, a.begin()+5);  // [2,5) で初期化  他の要素を使って初期化できる 
	assert(b == vector<int>({4, 1, 5}));
	/*
	vectorの三次元配列
	a[X][Y][Z]  -1で初期化
	vector<vector<vector<int>>> a(X, vector<vector<int>>(Y, vector<int>(Z, -1)));
	以下の形式でいけるらしい
	vector dp(X, vector(Y, vector<ll>(Z, -1)));
	四次元の場合は以下 dp[A][B][C][D]
	vector dp(A, vector(B, vector(C, vector<ll>(D, -1))));

	【注意】
	多次元にしすぎると生配列に比べ速度がだいぶ落ちそう。
	三次元ぐらいまでに抑えておいて、それ以上はグローバル変数にて生配列で定義するのが良いかなあ。
	参考：ABC264-F.cppの解説
	*/
	/*
	vectorの要素数変更
	要素数をN個に変更
	a.resize(N);

	a[][]に対して、要素数をN*N個に変更
	a.resize(N);
	for(int i = 0; i < N; i++)
	{
		a[i].resize(N);
	}

	要素数変更しつつ、中身の指定
	a.resize(N, 10);
	または
	a.assign(N, 10);
	違いは、元がa={1, 2, 3} のとき、
	resize:a={1, 2, 3, 10, 10, ..., 10}
	assign:a={10, 10, 10, ..., 10}
	のように、元から入っていた値を保持するかしないか、の違い
	参考：
	  https://minus9d.hatenablog.com/entry/2021/02/07/175159
	  https://qiita.com/matumoto2/items/4da55312786db838c0b3
	*/
	/*
	vectorのソート
	#include <algorithm> が必要
	sort(a.begin(), a.end());  // 昇順
	sort(a.begin(), a.end(), greater<int>());  // 降順

	自作関数でソートしたい場合、
	bool myCompare(int left, int right) { return left < right; }
	のように用意しておいて、
	sort(a.begin(), a.end(), myCompare);

	自作のクラスをソートしたい場合、
	bool myCompare(const Edge &a, const Edge &b){ return a.w < b.w; }
	のように用意しておいて(引数は値渡しでも良い模様)、
	sort(graph.begin(), graph.end(), myCompare);
	参考：https://arcslab.hatenablog.jp/entry/2020/03/26/011321
	(第1引数)<(第2引数) だと昇順ソートとなる。先頭から < の順に並ぶ、と覚えると良いかな。
	よって降順ソートなら > で比較する。
	注意：<= での比較はNG. myCompare(a, a) == false を満たす必要がある。
	参考：https://qiita.com/threecups/items/aa1923a9922dc0a7abfe
	*/
	/*
	lower_bound(), upper_bound()はstd::を使う
	auto itr = lower_bound(a.begin(), a.end(), 3);
	cout << itr - a.begin() << endl;  // ランダムアクセスが可能のため、加減算が可能
	*/
	/*
	find()はstd::を使う
	auto itr = std::find(a.begin(), a.end(), 4);
	cout << itr - a.begin() << endl;
	*/
}

void _string_(void)
{
	cout << "-----string-----" << endl;
	string str(5, '*');  // 5文字ぶん、'*'で初期化
	assert(str == string(5, '*'));

	// 文字列の切り出し:substr()
	str = "012345";
	assert(str.substr(2, 3) == "234");  // str[2]から3文字
	assert(str.substr(3) == "345");  // str[3]以降の文字列
	assert(str.substr(str.size() - 4) == "2345");  // 末尾から4文字取り出したい場合
	assert(str.substr(2, 100) == "2345");  // 切り出し数が大きくてもOK 末尾まで切り出してくれる

	// 文字列の検索:find()
	str = "0123450";
	assert(str.find("3") == 3);  // 3文字目(0-indexed)
	assert(str.find("6") == string::npos);  // 見つからない場合は string::npos
	assert(str.find("0", 1) == 6);  // 1文字目から検索

	// 指定位置から先頭に向かって検索:rfind()
	str = "01110";
	assert(str.rfind("1") == 3);  // 基本はこれかな 末尾から先頭に向かって検索
	assert(str.rfind("1", 2) == 2);  // str[2]から先頭に向かって検索

	// 数値を文字列に変換:std::to_string()
	assert(to_string(1234) == "1234");

	// 文字列を数値に変換:stoi(), stol(), stoll(), stoull()など
	assert(stoi("1234") == 1234);
	assert(stol("12345") == 12345);  // long
	assert(stoll("12345678901") == 12345678901LL);  // long long
	assert(stoull("12345678901") == 12345678901ULL);  // unsigned long long
	// assert(stoi("12345678901") == 12345678901LL);  // 範囲外のときはout_of_range例外が投げられる
	// その他、stof(), stod(), stold()もあるらしい
	// 第3引数にて何進法(基数)を指定できる
	assert(stoi("0011", nullptr, 2) == 3);  // 先頭に0があっても良い / 第2引数はnullptrで
	assert(stoi("044", nullptr, 5) == 24);
	assert(stoi("ABC", nullptr, 13) == 1845);  // A=10,B=11,C=12として 13^2*10+13*11+12=1845

	// 文字列を全て小文字に変換
	str = "ABCYZ";
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	assert(str == "abcyz");
	// 大文字に変換する場合は ::toupper を指定する
}

void settest(set<int> &s, string &&prev)
{
	cout << prev << endl;
	for(auto &e : s)
	{
		cout << e << ", ";
	}
	cout << endl;
}

void multisettest(multiset<int> &s, string &&prev)
{
	cout << prev << endl;
	for(auto &e : s)
	{
		cout << e << ", ";
	}
	cout << endl;
}

// set:集合を管理できるデータ構造
// 要素は昇順に並んでいる
// 各操作の計算量は、要素数NのときにおおよそO(logN)
// setは重複した要素を持てないが、multisetなら重複した要素も独立して持てる
// 
// find()などで得られるイテレータは、s.find()-s.begin()のような加減算は不可能。
// インクリメント、デクリメントなら可能。
// 
// #include <set>  が必要
void _set_multiset_(void)
{
	set<int> s;

	// insert:要素を追加する。既に要素が存在する場合は追加しない(multisetは追加する)。
	s.insert(2);
	s.insert(5);
	s.insert(1);
	s.insert(5);  // 既に存在する要素は追加しない
	s.insert(6);
	// s={1,2,5,6};
	settest(s, "---1---");

	// erase:要素を削除する。値またはイテレータを指定することが可能。
	s.erase(6);
	auto itr = s.begin();
	itr++;
	s.erase(itr);
	// s={1,5};
	settest(s, "---2---");

	// イテレータの移動
	// --,++に加え、prev,nextが使える。_stl_()を参照。

	s.insert(10);
	s.insert(15);
	s.insert(20);
	// s={1,5,10,15,20};
	// lower_bound(x):x以上である最小の要素を指すイテレータを返す
	// upper_bound(x):xより大きい最小の要素を指すイテレータを返す
	s.erase(s.lower_bound(10));  // 10以上の最小として、10を削除
	s.erase(s.upper_bound(5));  // 5より大きいものの最小として、15を削除
	// s={1,5,20};
	settest(s, "---3---");

	auto a = s.find(20);
	if(a != s.end())  // 見つからなければend()
	{
		a--;
		// s={1,5};
		s.erase(a);
	}
	settest(s, "---4---");
	// [注意]
	// set/multisetはランダムアクセス不可のため、値20が何番目の要素か、を計算で出すことはできない。
	// s.find(20)-s.begin() といったコードはエラーとなる。

	// [注意]
	// set/multisetの場合、lower_boundはstd::lower_bound()ではなく、自身が持つ関数を使うこと。
	// ->s.lower_bound(10);
	// 参考：https://rsk0315.hatenablog.com/entry/2019/09/10/173708
	// std::lower_bound()だとO(N)で、s.lower_bound()だとO(logN)になる。

	// clear:要素を全削除する
	s.clear();
	settest(s, "---5---");

	// 自作の構造体を扱うには、"<"の実装が必要。
	/*
	struct Person
	{
		bool operator<(const Person& a) const {
        return y < a.y;
	    }
	};
	*/

	// リバースイテレータ(逆イテレータ)
	// 参考：
	//   https://rsk0315.hatenablog.com/entry/2020/07/06/004531
	//   https://cpprefjp.github.io/reference/iterator/rbegin.html
	// rbegin():要素の末尾を指すリバースイテレータ
	// itr = s.end(); itr--; とかせず、末尾の要素にアクセスできて便利。
	s = {1, 2, 3};
	assert(*s.rbegin() == 3);

	multiset<int> ms;
	ms.insert(2);
	ms.insert(5);
	ms.insert(2);  // 重複した要素も持てる
	ms.insert(10);
	ms.insert(10);
	// s={2,2,5,10,10};
	multisettest(ms, "---6---");

	ms.erase(2);  // この場合、2が全て削除される
	ms.erase(ms.find(10));  // 1つだけ消したい場合、イテレータを渡す
	multisettest(ms, "---7---");

	// lower_bound(), upper_bound()についてはsetでの前述注意と同様。

	ms.insert(10);
	ms.insert(10);
	// s={5,10,10,10};
	multisettest(ms, "---8---");
	// count(x):要素がxである数を返す
	// ただし計算量に注意。全体の要素数がN, xの個数をkとするとO(k+logN)となる。
	// ->findならO(logN)
	// 参考：https://atcoder.jp/contests/abc253/editorial/4019
	cout << "10 count:" << ms.count(10) << endl;

}

namespace for_priority_queue
{
struct stque_ {
	int a, b;
};
bool operator< (const stque_ &l, const stque_ &r) {
	if(l.a != r.a) return (l.a < r.a);
	else if(l.b != r.b) return (l.b < r.b);
	else return false;  // 同値はfalseにする必要がある
};
bool operator> (const stque_ &l, const stque_ &r) {
	if(l.a != r.a) return (l.a > r.a);
	else if(l.b != r.b) return (l.b > r.b);
	else return false;
};
}

// 優先度付きキュー
// キューからの取り出しが、優先度が高い要素からになる
// push, pop, topの計算量はO(logN)
// 
// #include <queue> が必要
// 
// 参考：https://qiita.com/e869120/items/518297c6816adb67f9a5
void _priority_queue_(void)
{
	// int型の要素を持ち、小さい方が優先度が高いキュー
	priority_queue<int, vector<int>, greater<int>> queue;
	// 大きい方が優先度を高くするには以下。
	// priority_queue<int, vector<int>, less<int>> queue;
	// priority_queue<int> queue;  // もしくはこっち

	// push:要素を追加する
	queue.push(5);
	queue.push(3);
	queue.push(10);
	// queue={3,5,10}

	// top:キューの先頭の要素を返す(削除はしない)
	auto a = queue.top();
	cout << "1:a=" << a << endl;
	// pop:キューの先頭の要素を削除する
	queue.pop();
	// queue={5,10}
	a = queue.top();
	cout << "2:a=" << a << endl;

	// size():要素数
	cout << "3:size=" << queue.size() << endl;

	// empty():要素数が0ならtrue, それ以外はfalse
	queue.pop();
	queue.pop();
	if(queue.empty())
	{
		cout << "4:empty:true" << endl;
	}
	else
	{
		cout << "4:empty:false" << endl;
	}

	// pairと組み合わせる場合
	// この場合、firstの値が小さい方が優先度が高く、
	// firstが同じ値ならsecondの値が小さい方が優先度が高い。
	priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> queue_pair;
	pair<int,int> tmpPair;
	tmpPair.first = 10;
	tmpPair.second = 20;
	queue_pair.push(tmpPair);
	queue_pair.push(pair<int,int>(20,30));  // これでも良い模様 emplace()との違いは理解できていない
	queue_pair.push(pair<int,int>(10,5));
	queue_pair.push(pair<int,int>(20,35));
	// queue_pair={(10,5), (10,20), (20,30), (20,35)}
	cout << "5:queue/pair" << endl;
	while(!queue_pair.empty())
	{
		tmpPair = queue_pair.top();
		cout << tmpPair.first << " " << tmpPair.second << endl;
		queue_pair.pop();
	}

	// vector<int>と組み合わせる場合
	// この場合、[0]が小さい方が優先度が高く、
	// [0]が同じなら[1]が小さい方が優先度が高い。[2]以降も同様。
	// キューの各要素のサイズが異なる場合の挙動は未確認。
	priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> que_vec;
	que_vec.push({5, 1, 100});
	que_vec.push({5, 0, 50});
	que_vec.push({3, 100, 0});
	// que_vec = {{3,100,0}, {5,0,50}, {5,1,100}}
	string s;
	while(!que_vec.empty())
	{
		auto tmp = que_vec.top();
		que_vec.pop();
		s += to_string(tmp[2]) + " ";
	}
	assert(s == "0 50 100 ");
	// ★arrayとも同様に組み合わせ可能。
	//   競プロでは無理に自作クラスやカスタム比較を使うよりも、arrayで済ます方が手っ取り早いかな

	// 自作クラスを使う場合
	// < or > の演算子オーバーロードが必要。前者はlessでの比較、後者はgreaterの比較で必要。
	//   https://rkd3.dev/post/pqmycls/
	using namespace for_priority_queue;
	priority_queue<stque_, vector<stque_>, greater<stque_>> stque;
	stque.push({10, 20});
	stque.push({20, 40});
	stque.push({20, 30});
	stque.push({10, 20});
	s.clear();
	while(!stque.empty())
	{
		auto tmp = stque.top();
		stque.pop();
		s += "{" + to_string(tmp.a) + "," + to_string(tmp.b) + "},";
	}
	assert(s == "{10,20},{10,20},{20,30},{20,40},");
}

// 双方向リスト
// ランダムアクセスがO(N), 任意位置への挿入と削除がO(1)で可能
// #include <list> が必要
// 
// 参考：http://vivi.dyndns.org/tech/cpp/list0.html
void _list_(void)
{
	list<int> lst;

	// push_back();末尾に追加
	// push_front():先頭に追加
	lst.push_back(100);
	lst.push_front(50);
	// lst={50,100}

	// insert(itr, value):itrの"手前"に値を挿入
	// 戻り値は挿入した場所へのイテレータ
	auto itr = lst.begin();
	itr++;  // 100
	itr = lst.insert(itr, 75);
	itr = lst.insert(itr, 60);
	cout << "1:*itr=" << *itr << endl;
	// lst={50,60,75,100}

	cout << "2:lst  ";
	for(itr = lst.begin(); itr != lst.end(); itr++)
	{
		cout << *itr << " ";
	}
	cout << endl;

	// pop_front:最初の要素を削除
	// pop_back:最後の要素を削除
	lst.pop_front();
	lst.pop_back();
	// lst={60,75}
	cout << "3:pop  ";
	for(auto &e : lst)
	{
		cout << e << " ";
	}
	cout << endl;

	cout << "4:size  ";
	if(lst.empty())
	{
		cout << "list enmty." << endl;
	}
	else
	{
		cout << "size:" << lst.size() << endl;
	}

	// erase(itr):itrの要素を削除
	// 戻り値は削除した要素の次の要素
	lst.push_back(80);
	lst.push_back(90);
	lst.push_back(99);
	// lst={60,75,80,90,99}
	itr = lst.begin();
	itr++;
	itr++;  // 80
	itr = lst.erase(itr);  // 90
	cout << "5:erase  *itr=" << *itr << endl;

	cout << "6:lst  ";
	for(auto &e : lst)
	{
		cout << e << " ";
	}
	cout << endl;
}

// 連想配列
// 検索可能なキーと、キーに対応する値のペアを要素とする。
// キーを指定して値を取り出すことができる。
// (2分木にて実装されており、値の取り出しは O(logN) で可能)
// 
// 内部で要素がソートされている。
// ソートの必要が無い場合、ハッシュを用いた unordered_map もある。
// (値の取り出しを平均的に O(1) で可能)
//
// #include <map> が必要
// 参考：
// http://vivi.dyndns.org/tech/cpp/map.html
// https://qiita.com/_EnumHack/items/f462042ec99a31881a81
void _map_(void)
{
	cout << "*** map ***" << endl;

	/*
	 * [実行時間についてのメモ]
	 * ABC344-Eにて不可解な現象が発生した。
	 * キーに負の値を使うと、C++20よりC++23が圧倒的に遅くなった。(920ms vs 1590ms)
	 * 負の値を使わない場合、C++23の方がまだ遅いが、おおよそ同等にはなった。 (920ms vs 1060ms)
	 * 理由はサッパリ不明だが、とりあえずは以下だろうか… 「C++23を使わない」というのも何か変な感じがするし。
	 * ・要素数が多いとか、先頭から辿っていくとかの場合、unordered_mapを使うことを意識する
	 * ・mapを使う場合、キーに負の値は使わないようにする
	 */

	map<string, int> mp;
	mp["aaa"] = 100;
	mp["bbb"] = 200;
	mp["ccc"] = 150;
	// mp={aaa,bbb,ccc}
	cout << "1:" << endl;
	cout << "size:" << mp.size() << " bbb:" << mp["bbb"] << " ccc:" << mp["ccc"] << endl;

	// begin(),end()で各要素にアクセスできる
	// first, second
	cout << "2:" << endl;
	for(auto itr = mp.begin(); itr != mp.end(); itr++)
	{
		cout << "first:" << itr->first << " second:" << itr->second << endl;
	}

	// 範囲for
	cout << "3:" << endl;
	for(auto &e : mp)
	{
		cout << e.second << " ";
	}
	cout << endl;

	// []にて存在しない要素にアクセスすると、自動的に追加されてしまう
	cout << "4:" << endl;
	cout << "size:" << mp.size() << endl;
	cout << mp["ddd"] << endl;  // 存在しない要素にアクセス
	cout << "size:" << mp.size() << endl;  // sizeが1増えている
	// mp={aaa,bbb,ccc,ddd}
	// 自動追加をさせたくない場合、find()やcount()を使う
	auto itr = mp.find("eee");
	if(itr != mp.end())
	{
		cout << "eee is found" << endl;
	}
	else
	{
		cout << "eee is not found" << endl;
	}

	// erase:要素の削除
	cout << "4:" << endl;
	mp.erase("aaa");  // erase(キー)での削除
	itr = mp.find("bbb");
	if(itr != mp.end())
	{
		mp.erase(itr);  // erase(イテレータ)での削除
	}
	// mp={ccc,ddd}
	for(itr = mp.begin(); itr != mp.end(); itr++)
	{
		cout << "first:" << itr->first << " second:" << itr->second << endl;
	}
	// イテレータ指定でerase(first, last)とすると、[first, last) の範囲の要素をまとめて削除できる。
	// "[)"の範囲指定になるため、key_1からkey_2まで削除したい場合は
	// erase(lower_bound(key_1), upper_bound(key_2))  にするのが良い。

	// size():要素数
	// empty():要素数が0ならtrue

	// 自作の構造体を扱うには、"<"の実装が必要。
	/*
	struct Person
	{
		bool operator<(const Person& a) const {
        return y < a.y;
	    }
	};
	*/
}

// pair
// 2つの要素を持てる型
// 
// 追加のincludeは不要らしい。
// 参考：
// https://qiita.com/e869120/items/518297c6816adb67f9a5#3-18-pair
// https://atcoder.jp/contests/apg4b/tasks/APG4b_z
void _pair_(void)
{
	cout << "*** pair ***" << endl;

	pair<int,int> p;
	// first, secondでそれぞれ1つ目、2つ目の要素にアクセス
	p.first = 10;
	p.second = 20;
	cout << "1:" << endl;
	cout << "p.first:" << p.first << " p.second" << p.second << endl;

	pair<string, int> p2;
	pair<string, string> p3;
	pair<int, int> p4;
	// make_pairでpairの生成が可能
	p2 = make_pair("test", 100);
	// = {first, second} でも良いらしい
	p3 = {"hoge", "hage"};
	// pair<型1, 型2>(値1, 値2) という方法もある
	p4 = pair<int, int>(10, 30);
	cout << "2:" << endl;
	cout << p2.first << " " << p2.second << endl;
	cout << p3.first << " " << p3.second << endl;
	cout << p4.first << " " << p4.second << endl;
	
	// vectorとの組み合わせ
	vector<pair<int, int>> LR;
	// 値の指定方法は、前述の3通りいずれも使える
	LR.push_back(make_pair(10,20));
	LR.push_back({1,2});
	LR.push_back(pair<int,int>(10,5));
	// LR={10,20},{1,2},{10,5}
	cout << "3:" << endl;
	for(auto &e : LR)
	{
		cout << e.first << " " << e.second << endl;
	}

	// 大小比較は以下。
	// ＞2つの pair 型要素の大小比較は、以下のように決められます。
	// ＞1つ目の要素が小さい方を小さいとみなす。
	// ＞1つ目の要素が同じである場合、2つ目の要素が小さい方を小さいとみなす。
	// よってvector<pair<>>のソートは、まず1番目の要素でソートし、同じ値なら2番目の要素でソートされる。
	cout << "4:" << endl;
	sort(LR.begin(), LR.end());
	// LR={1,2},{10,5},{10,20}
	for(auto &e : LR)
	{
		cout << e.first << " " << e.second << endl;
	}

	// pairの要素でupper_bound, lower_boundをする場合
	// {val, (何か補助的な値)} のように1つ目の要素で実施したい場合は以下。
	// {*,*}ではなくmake_pair()にしないとエラーになる。
	const int INF32 = 0x3FFFFFFF;
	// upper_boundは +INF を指定
	auto idx = upper_bound(LR.begin(), LR.end(), make_pair(1, INF32));
	assert(*idx == make_pair(10,5));
	// lower_boundは -INF を指定
	idx = lower_bound(LR.begin(), LR.end(), make_pair(10, -INF32));
	assert(*idx == make_pair(10,5));
}

// deque
// vectorと違い、先頭/末尾それぞれ追加/削除が可能
// かつdeq[3]のように添字アクセスができる
// 
// #include <deque> が必要
// #include <queue> でも良いっぽい
void _deque_(void)
{
	cout << "*** deque ***" << endl;
	deque<int> de;
	// 先頭/末尾への追加
	de.push_back(3);
	de.push_front(1);  // {1,3}
	assert(de[0] == 1 && de[1] == 3);
	de.push_back(4);  // {1,3,4}

	// 先頭/末尾の削除
	de.pop_front();
	de.pop_back();  // {3}
	assert(de.size() == 1 && de[0] == 3);
}

// mod mの世界におけるaの逆元を返す
// 以下URLのコードをそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modinv(long long a, long long m)
{
	long long b = m, u = 1, v = 0;
	while (b) {
        long long t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= m; 
    if (u < 0) u += m;
    return u;
}

void _mod_(void)
{
	// *, /, % は同じ優先順位
	/*
	参考：https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a

	const ll MOD = 998244353;
	足し算、引き算、掛け算については計算途中でMODを計算して良い
	また、このときのMODは素数でなくても良い
	(a+b) * c % MOD
	->((a+b)%MOD) * c % MOD

	引き算については、引く前にMODを足しておいた方が良さそう
	(a-b)%MOD
	->(a+MOD-b)%MOD

	割り算は注意。
	「MODしてから除算」は正しく計算できない
	a/b % MOD
	-> (a%MOD)/b % MOD  は間違い
	「除算してからMOD」か、逆元をかける

	逆元を求める方法：
	※(1)はpが素数である必要がある。(2)はpが素数でなくとも、後述の逆元が存在する条件を満たせば使える。
	(1)フェルマーの小定理を用いる方法
	  pを素数、aをpの倍数でない整数としたときに、以下が成り立つ。
	    a^(p-1) ≡ 1 (mod p)
	  よって
	    a*a^(p-2) ≡ 1 (mod p)
	  となり、aにa^(p-2)をかけると1になる。つまりaの逆元は a^(p-2) となる。
	  これを繰り返し二乗法などで求めればよい。
	(2)拡張ユークリッドの互除法を用いる方法
	  詳細略。本ソースコードのmodinv()がこれを用いた実装。

	逆元が存在する条件：
	mod pでのaの逆元が存在する条件は、pとaが互いに素であること
	・pが素数でなくとも、aに逆元が存在することはある
	  例：p=10, a=9
	・pが素数であっても、aに逆元が存在しないことがある
	  例：p=素数, a=0, 2p, 3p, ...
	★mod pでpが素数という条件が出てくるのは、フェルマーの小定理を用いるとき。
	  それ以外はpが素数という条件は無い。
	*/

	long long a, b;
	long long mod = 998244353;
	a = 10; b = 2;
	cout << "a=" << a << ", b=" << b << ", a/b=" << a * modinv(b, mod) % mod << endl;
	// a/b=5

	a = 10; b = 3;
	cout << "a=" << a << ", b=" << b << ", a/b=" << a * modinv(b, mod) % mod << endl;
	// a/b=332748121
	// 332748121*3 = 998244363 を998244353で割った余りは10(=a)
	
	a = 12345678900000; b = 100000;
	cout << "a=" << a << ", b=" << b << ", a/b=" << (a%mod) * modinv(b, mod) % mod << endl;
	// a/b=123456789
	// aに事前に剰余計算忘れずに

	// 負の数に対してMOD計算した結果は、負になる(※)
	a = -10; b = 3;
	cout << "a=" << a << ", b=" << b << ", a%b=" << a%b << endl;  // -10%3 = -1
	// -10%3=-1
	// 
	// 0～2(=3-1)の範囲にしたい場合は、MOD計算結果が負になった場合に3を足すようにする
	if(a%b < 0)  // 判定はa<0ではダメで、a%b<0でないと、a%b=0のときに+3が出力されてしまう(a=-3のようなケース)
	{
		cout << "a=" << a << ", b=" << b << ", a%b=" << (a%b)+b << endl;  // (-10%3)+3 = 2
	}
	else
	{
		cout << "a=" << a << ", b=" << b << ", a%b=" << a%b << endl;  // -3%3 = 0
	}
	// (※)言語によって仕様が異なる。Pythonでは同様の計算結果は正になる
	//    参考：https://atcoder.jp/contests/abc266/editorial/4665
}

// a^n (mod) を計算する
// 例えば3^5=3^1*3^4であり、5をビットごとに見ていって最下位ビットが1のときに積算する
// ※modは素数といった制約は無い
// 以下URLのコードをほぼそのまま持ってきている
//   https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
long long modpow(long long a, long long n, long long mod)
{
	// a %= mod;  // a*aにてオーバーフローしそうな場合に有効化
	long long ans = 1;
	while(n > 0)
	{
		if((n&1) != 0)  // nの最下位ビットが立っているなら、答に掛ける
			ans = (ans * a) % mod;
		a = (a * a) % mod;
		n >>= 1;
	}
	return ans;
}

void _pow_(void)
{
	// #include <cmath>  が必要
	cout << pow(10, 3) << endl;  //10^3

	// long longの最大値(10^18)付近になる場合、powl()の方が良さそう   ★AtCoderの場合
	// pow() :戻り値はdouble型        64bit
	// powl():戻り値はlong double型   環境依存だが、128bitとか何とか
	// 参考：https://qiita.com/mod_poppo/items/8860505f38e2997cd021

	cout << "2^3 (mod 1000000007)=8 : " << modpow(2, 3, 1000000007) << endl;
	cout << "5^8 (mod 1000000007)=390625 : " << modpow(5, 8, 1000000007) << endl;
	cout << "93^200023 (mod 1000000007)=415192366 : " << modpow(93, 200023, 1000000007) << endl;
	cout << "83^999999570 (mod 1000000007)=642657703 : " << modpow(83, 999999570, 1000000007) << endl;
}

void _sqrt_(void)
{
	// sqrt() :引数と戻り値はdouble型        64bit
	// sqrtl():引数と戻り値はlong double型   環境依存だが、128bitとか何とか
	// 参考：https://qiita.com/mod_poppo/items/8860505f38e2997cd021
	// よってlong long型の大きな値をsqrt()に入れると、引数にて long long->double への変換で誤差が生じる可能性がある
	// (64bitだが、仮数部は53bitのため)
	// 参考：https://rsk0315.hatenablog.com/entry/2022/06/04/190255
}

// limitを上限として、a*bを返す
// 乗算でオーバーフローを防ぐ用途で用いる
// -> min(a*b, limit)  a,b,u>=0
// hamamuさんの以下コードをほぼそのまま持ってきている
//   https://twitter.com/hamamu_kyopro/status/1612015305110454274/
long long limitmul(long long a, long long b, long long limit)
{
	if(b == 0) return 0;  // a*b=0

	if(a <= limit / b)
		return a*b;
	else
		return limit;
}

// ハッシュテーブルを用いた集合
// 同一要素を複数格納できず、格納順が規定されていない
// 挿入、削除、検索などの操作が、平均的にO(1)で可能
// 
// #include <unordered_set>  が必要
// 
// pairとの組み合わせ方法は、本関数の後半にあるコメントを参照のこと。
// 
// 参考：
// https://cpprefjp.github.io/reference/unordered_set/unordered_set.html
// https://zenn.dev/reputeless/books/standard-cpp-for-competitive-programming/viewer/library-unordered_set
void _unordered_set_(void)
{
	unordered_set<int> us;
	cout << "-----unordered_set-----" << endl;

	// insert:要素を挿入
	us.insert(10);
	us.insert(50);
	us.insert(200);
	us.insert(110);
	// us={10,50,110,200}  ただし格納順は規定されていない

	// erase:要素を削除
	us.erase(50);
	auto it = us.find(110);
	if(it != us.end())  us.erase(it);  // イテレータによる削除も可能
	// us={10,200}

	// count/find:要素の検索
	if(us.count(10) == 1)
	{
		cout << "10 exists." << endl;
	}
	else
	{
		cout << "10 does not exist." << endl;
	}
	it = us.find(150);
	if(it != us.end())
	{
		cout << "150 exists." << endl;
	}
	else
	{
		cout << "150 does not exist." << endl;
	}

	// size:要素数
	cout << "size:" << us.size() << endl;
	
	// 要素一覧の出力
	for(auto &e : us)
	{
		cout << e << ",";
	}
	cout << endl;

	/*
	pairにはハッシュ関数が定義されていないため、そのままでは
	  unordered_set<pair<int,int>> us;
	のように定義できない。
	使うにはpairのハッシュ関数を定義する必要がある。

	https://qiita.com/tqk/items/7c4df1eeb5ebaedcf714  を参考に、以下のphashを追加する。
struct phash{
	inline size_t operator()(const pair<int,int> & p) const{
		const auto h1 = hash<int>()(p.first);
		const auto h2 = hash<int>()(p.second);
		return h1 ^ (h2 << 1);
	}
};
	そして
	  unordered_set<pair<int,int>, phash> us;
	と定義する。

	h1 ^ (h2 << 1) の部分はサイトによって色んな手法があるようだが、
	単に h1^h2 だと {a,b}と{b,a}で同じハッシュ値になってしまう、という記載が上記サイトにてされていた。
	*/
}

// ハッシュテーブルを用いた連想配列
// 要素へのアクセスを平均的にO(1)で可能な代わりに、内部で要素はソートされていない。
// その他、_map_()側の説明を参照のこと。
// 
// #include <unordered_map> が必要
// 参考：
// http://vivi.dyndns.org/tech/cpp/unordered_map.html
void _unordered_map_(void)
{
	unordered_map<string, int> um;
	cout << "-----unordered_set-----" << endl;

	um["aaa"] = 100;
	um["bbb"] = 200;
	um["ccc"] = 150;

	// 要素はソートされていないため、先頭からたどっても順序はバラバラ
	for(auto &e : um)
	{
		cout << e.first << ":" << e.second << endl;
	}

	// 要素の検索
	// countの他、find()やeraseもmapと同様。
	if(um.count("aaa") == 1)
	{
		cout << "aaa exists." << endl;
	}
	else
	{
		cout << "aaa does not exist." << endl;
	}
}

// STL(コンテナ含む)に関するメモ
void _stl_(void)
{
	cout << "-----stl-----" << endl;

	// 以下に記載している標準ライブラリ
	// reverse, find, count, max(min), max_element(min), reduce, merge

	/*
	コンテナ使用時、要素の追加や削除操作には要注意。「イテレータ破壊」が発生する可能性がある。
	例：
	  vectorに対する範囲for内で、push_back()操作など。
	  範囲forでなくとも、itr = a.begin() などのイテレータ操作時も同様。
	  map/setではイテレータ破壊は起きないかも？(後述 ※)
	参考：
	  https://qiita.com/izmktr/items/0c95aff5ba7554afcaa7
	  https://dixq.net/forum/viewtopic.php?t=11823
	(※)map/setについて：
	  map/setでは以下のようなコードにてイテレータ破壊は起きなさそうだった。
	  出力は"1 2 3 5"であった。
	  途中で追加/削除されても、要素を昇順にイテレートされていく感じか。
	  	------------------------
		set<int> sa = {1,2,3,4};
		for(auto &e : sa)
		{
			if(e == 1) sa.insert(5);
			if(e == 2) sa.insert(0);
			if(e == 3) sa.erase(4);
			cout << e << endl;
		}
		------------------------
	  ただし不安なときは、昇順に操作していくという前提で、以下のようなコードも良さそう。
		------------------------
		while(sa.size() > 0)
		{
			int n = *sa.begin();
			sa.erase(sa.begin());
			(nを使って計算)
		}
		------------------------
	*/

	// 要素の並びを反転する:std::reverse
	// stringやvector, 通常の配列などにも使える
	// #include <algorithm> が必要
	// 計算量は要素数をNとしてO(N).
	int a[5] = {0, 1, 2, 3, 4};
	reverse(a+1, a+4);  // [1,4) を反転する -> {0,3,2,1,4}
	for(int i = 0; i < 5; i++) {cout << a[i] << " ";} cout << endl;

	string str = "abcde";
	reverse(str.begin(), str.end());
	assert(str == "edcba");
	
	vector<int> a_v = {10, 11, 12, 13, 14};
	// reverse(a_v.begin(), a_v.end());  // 使うのはこのケースが大半だと思う
	reverse(a_v.begin()+1, a_v.begin()+3);  // [1,3) を反転する -> {10,12,11,13,14}
	for(auto &e : a_v) {cout << e << " ";} cout << endl;

	// 要素の中にある値xが含まれるか検索する:std::find
	// 配列やvectorで使える
	// 計算量は要素数をNとしてO(N).
	// (stringはこれではなく、メンバ関数のfindを使う感じか)
	a_v = {1, 2, 3, 4, 5, 4, 3};
	assert(find(a_v.begin(), a_v.end(), 4) - a_v.begin() == 3);  // 4はa_v[3]  [begin, end)の半開区間にて探す
	assert(find(a_v.begin(), a_v.end(), 100) - a_v.begin() == (int)a_v.size());  // 見つからない場合
	// 末尾から検索したい場合、string::rfind()のような関数は無いので、reverse_iteratorを使う
	// a_v[*]の要素番号idxを求める式は以下の感じ (把握しきってないので、使う際は要注意かな…)
	int idx = find(a_v.rbegin(), a_v.rend(), 4) - a_v.rbegin();
	idx = a_v.size() - 1 - idx; // -1が必要
	assert(idx == 5);  // 4が最後に登場するのはa_v[5]

	// 要素に値xがいくつ含まれるかを返す:std::count
	// stringやvector, 通常の配列などにも使える
	// 計算量はO(r-l).
	// (setやmap系はメンバ関数のcount()を使う)
	str = "abcdeeef";
	assert(count(str.begin(), str.end(), 'e') == 3);  // "e"だとエラー
	a_v = {1, 2, 3, 4, 5, 4, 4, 3};
	assert(count(a_v.begin(), a_v.end(), 4) == 3);

	// 前後のイテレータを返す:std::prev, std::next
	// prev(itr):1つ前のイテレータを返す
	// prev(itr, k):k個前のイテレータを返す
	// 引数に渡したイテレータ自体は変化しない
	// 手前の要素を参照したいとき、itr-- が不要になり実装が少し簡易になるかも。
	set<int> s;
	s.insert(1); s.insert(3); s.insert(5); s.insert(7);
	auto itr = s.lower_bound(3);
	if(itr != s.begin()) s.erase(*std::prev(itr));  // 3より1つ手前の要素を取得する
	assert(*s.begin() == 3);

	// max,minは、以下の記述により3つ以上の要素を取ることができる(C++11より)
	// initializer_list と呼ぶらしい
	assert(max({1, 2, 3}) == 3);
	assert(min({3, 2, 1}) == 1);

	// 要素の最大/最小値のイテレータを返す:max_element(min_element)
	// 計算量はO(r-l).
	assert(*max_element(a_v.begin(), a_v.end()) == 5);
	assert(*min_element(a_v.begin(), a_v.end()) == 1);
	
	// 要素の合計を求める:std::reduce(C++17より)
	// #include <numeric> が必要
	// C++17より前の環境ではstd::accumulate()を使う
	// また、reduce()は並列計算を行うため、先頭から順に計算しないとNGなケースではaccumulate()を使う方が良さそう。
	//   https://qiita.com/yumetodo/items/3712dbb26b11f282e0e7
	a_v = {10, 100, 1000};
	assert(reduce(a_v.begin(), a_v.end()) == 1110);
	// ★注意★戻り値は渡した要素の型となるため、オーバーフローに注意
	// 合計値を特定の型にしたい場合、第3引数にて指定する
	reduce(a_v.begin(), a_v.end(), 0LL);  // long long型で計算
	reduce(a_v.begin(), a_v.end(), 0.0);  // double型で計算

	// 2つのソート済みイテレータ範囲をマージする:std::merge
	// 入力がソート済みであることで、マージソートの要領で全体としてソートされた要素を得られる
	// 結果のイテレータ範囲と入力のイテレータ範囲は重なってはならない
	// back_inserterは引数にpush_back()を繰り返し行うイメージ
	// 計算量はO(N).
	//   https://cpprefjp.github.io/reference/algorithm/merge.html
	//   https://cpprefjp.github.io/reference/iterator/back_insert_iterator.html
	{
		vector<int> v1 = {1, 3, 10};
		vector<int> v2 = {2, 4, 5};
		vector<int> result;
		std::merge(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(result));
		assert(result == vector<int>({1, 2, 3, 4, 5, 10}));
	}
}

// 構造化束縛に関するメモ
// 参考：https://cpprefjp.github.io/lang/cpp17/structured_bindings.html
void _structured_bindings_(void)
{
	// 構造化束縛にてpairや配列、mapなどを分解して要素を取り出すことができる
	pair<int,int> data = {10, 20};
	auto [v1, v2] = data;  // これが構造化束縛
	assert(v1 == 10);
	assert(v2 == 20);
	
	auto& [v3, v4] = data;  // auto& で参照で受け取る
	v3 = 33;
	assert(data.first == 33);  // dataが書き換わっている

	// mapに使えるのは便利
	map<string, int> mp;
	mp["hoge"] = 10;
	mp["tmp"]  = 20;
	mp["foo"]  = 30;
	// auto&で受け取る場合、keyはconstが付く。以下参照。
	// https://cpprefjp.github.io/lang/cpp17/structured_bindings.html
	for(auto& [key, value] : mp)
	{
		cout << "key:" << key << ", value:" << value << endl;
	}

	// 構造化束縛は変数宣言であり、以下はコンパイルエラーとなる
	/*
	int a, b;
	auto [a, b] = data;  // 定義済みの変数には使えない
	*/

	// 要素数が確定しているものが対象であるため、vectorには使えない
	/*
	vector<int> a = {10,20,30};
	auto [c1, c2, c3] = a;  // コンパイルエラー
	*/
}

// 集成体およびその初期化に関するメモ (仕様の理解は浅い)
// 参考：
//   https://onihusube.hatenablog.com/entry/2019/04/06/123202
//   https://onihusube.hatenablog.com/entry/2019/02/22/201044
void _aggregate_(void)
{
	struct aggregate {
		int a, b;
		string s;
	};
	// 集成体となる構造体の場合、以下のような初期化が可能
	aggregate ag = {10, 20, "test"};  // {}にてメンバ変数を順に指定
	aggregate ag2 = {30};  // 残りはデフォルト初期化されるらしい
	assert(ag.s == "test");
	assert(ag2.a == 30);
	assert(ag2.s == "");

	vector<aggregate> va;
	va.push_back({20, 30, "vec"});  // これは便利
	assert(va.back().s == "vec");
}

// #pragmaを用いた最適化テクニックについて
// 参考：https://qiita.com/ageprocpp/items/7bda728d109c953ece3c
void _optimize_(void)
{
#if 0
	// 以下3行を追加で、高速なバイナリが生成されるとのこと。
	// 順に自動ベクトル化(SIMD), 最適化オプション、ループアンロール
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")

	// ＞浮動小数点の計算が絡むときにも、次のpragmaでSIMDを有効化できます。
	// とのこと。 [未施行]
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,popcnt,abm,mmx,avx,tune=native")
#endif
}

// pairやtupleの分解にはstd::tieを使える
// 特にtupleで便利そう
void _tie_(void)
{
	cout << "-----tie-----" << endl;

	pair<int,string> data = {10, "Hello"};
	int a;
	string b;
	tie(a, b) = data;
	assert(a == 10);
	assert(b == "Hello");

	// 値コピーのため、tieで受けた先を書き換えても元データには影響しない(逆も同様)
	a = 11;
	b = "Hoge";
	assert(data.first == 10);
	assert(data.second == "Hello");

	// 代入不要の要素がある場合、std::ignoreを指定する
	tie(a, std::ignore) = data;
	assert(b == "Hoge");
}

// ビット集合を表す型。N桁の2進数ともみなせる。
// 競プロでは定数倍を1/64にして高速化を図る問題が存在する。
// #include <bitset> が必要。
// 参考：
//   https://cpprefjp.github.io/reference/bitset/bitset.html
//   https://qiita.com/SSK_Ayagawa/items/544a277101948f8307bb
//   https://qiita.com/e869120/items/518297c6816adb67f9a5
//   https://w.atwiki.jp/vivid_turtle/pages/40.html  bitsetでの定数倍改善
void _bitset_(void)
{
	cout << "-----bitset-----" << endl;
	bitset<8> bs("10000001");  // 長さ8 長さは固定サイズであること
	// set, reset:指定したビットを1,0にする
	bs.set(1);
	bs.reset(0);
	assert(bs.to_string() == "10000010");  // 文字列化が可能 bitsetのメンバ関数を使う

	// 論理演算は &,|,^,~ が可能
	bs |= bitset<8>("01110000");  // "11110010"
	bs = ~bs;  // "00001101";
	assert(bs == bitset<8>("00001101"));

	// シフト演算
	bs <<= 3;  // 3ビット左シフト
	bs >>= 2;
	assert(bs == bitset<8>("00011010"));
}

// a[]の要素に含まれない最小の非負整数を返す
// 呼び出し元で mex({0,2,3}); のようにinitializer_listを用いることを想定しているため、aは参照渡しにしていない
// a[]の要素数をNとして、計算量はO(N^2).
int mex(vector<int> a)
{
	int i, j;
	int a_size = (int)a.size();
	// iがMEXになるか判定
	// 要素数を超える値はMEXになりえないため、上限は要素数未満まで
	// (入力が {0,1,2} のような場合、i=3でループを抜けるため正しく判定される)
	for(i = 0; i < a_size; i++)
	{
		bool check = false;
		for(j = 0; j < a_size; j++)  // a[]にiが含まれているか
		{
			if(a[j] == i) check = true;
		}
		if(!check) break;
	}
	return i;
}

// a[]の要素に含まれない最小の非負整数を返す
// 呼び出し元で mex({0,2,3}); のようにinitializer_listを用いることを想定しているため、aは参照渡しにしていない
// 計算量はmex()と違いO(N)だが、Nが小さければmex()の方が早い (定数倍によるものと推測)
int mex2(vector<int> a)
{
	int i;
	int a_size = (int)a.size();
	vector<bool> exist(a_size, false);  // [i]:a[]にiがあればtrue
	for(i = 0; i < a_size; i++)
	{
		if(a[i] >= a_size) continue;
		exist[a[i]] = true;
	}
	// exist[]がfalseになる最小の値を求める
	// (入力が {0,1,2} のような場合、i=3でループを抜けるため正しく判定される)
	for(i = 0; i < a_size; i++)
	{
		if(!exist[i]) break;
	}
	return i;
}

void _mex_(void)
{
	cout << "-----mex-----" << endl;

	vector<int> a = {0, 1, 2, 4, 5};
	assert(mex(a) == 3);
	assert(mex2(a) == 3);

	a = {2, 1, 0};
	assert(mex(a) == 3);
	assert(mex2(a) == 3);

	a = {1};
	assert(mex(a) == 0);
	assert(mex2(a) == 0);

	a = {};
	assert(mex(a) == 0);
	assert(mex2(a) == 0);
}

// 数学関連のメモ
// 現状はsqrt()などのライブラリというよりも、浮動小数点数の誤差に関する内容が多い
void _math_(void)
{
	cout << "-----math-----" << endl;

	/*
	浮動小数点数の誤差について：
	誤差を避けたい場合、できるだけ整数の範囲で計算する。
	例えば入力が "1.23" のように小数第2位まで与えられる場合、以下の手法がある。
	  (1)文字列として受け取り、 s[0]*100 + s[2]*10 + s[3] とする。(正確には、s[0]-'0' )
	  (2)double型で受け取り、 (int)(X*100+0.5) とする。
	     ※double型で受け取って100倍する場合も誤差に注意。 X=2.51のとき、(int)(X*100)=250 になってしまう。
		   ただし(2)はXが正のケースでのみ有効。intに変換時は0方向に切り捨てられるので、Xが負のときは正しく計算できない。
	  (3)double型で受け取り、llround(X*100) とする。
	     正負どちらでも使えるので、これが良いかも。
	例：ABC169-C
	    https://twitter.com/kyopro_friends/status/1267093871957164032
		ABC191-D の解説
		https://atcoder.jp/contests/abc191/editorial/611
	 */
	double x = 2.51;
	// assert((int)(x*100) == 251);  // 251にならない
	assert((int)(x*100+0.5) == 251);  // これならOK

	/*
	double型の精度は53bitのため、正しく表現できる整数は2^53まで。
	2^53≒9.01*10^15
	参考：
	  https://zenn.dev/mod_poppo/articles/atcoder-beginner-contest-284-d
	  https://qiita.com/mod_poppo/items/910b5fb9303baf864bf7
	 */
	x = 36028797018963969;  // 2^55+1
	// assert(36028797018963969 == (ll)x);  一致しない

	/*
	原点から見た座標(x,y)の偏角を求める -> atan2(y,x)
	y,xの順に座標を指定することで、[-π, π]の範囲で偏角を返す。(単位はラジアン)

	d=y/xとして atan(y/x) としても偏角が求まるが、こちらの範囲は[-π/2, π/2]のため注意。
	引数に傾きを与えるという考え方。
	 */
}

// 値とインデックスを同時に持たせたいときのテクニック
// 参考：
//   https://kuretchi.hateblo.jp/entry/2017/07/28/131151
//   https://hackmd.io/@tatyam-prime/value_with_index
void _value_with_index_(void)
{
	cout << "-----_value_with_index_-----" << endl;
	int i;
	/*
	配列の要素をソートしつつ、元のインデックスが必要になるケースがある。
	例：
	  N要素の数列 a[0],a[1],...,a[N-1] について、
	  a[b[0]]<=a[b[1]]<=...<=a[b[N-1]] となるような数列b[]を求めよ。
	以下2つの方法がある。いずれも実装は後述。
	方法1：値とインデックスのペアで管理
	方法2：インデックス用配列を別に作り、値をキーとしてソートする
	 */
	// 方法1：値とインデックスのペアで管理
	int N = 5;
	vector<int> inp(N); inp = {4, 0, 2, 1, 3};
	vector<int> ans(N); ans = {1, 3, 2, 4, 0};
	{
		vector<pair<int,int>> a(N);  // {value, index}
		for(i = 0; i < N; i++)
		{
			a[i] = {inp[i], i};
		}
		sort(a.begin(), a.end());  // valueでソート
		for(i = 0; i < N; i++) assert(a[i].second == ans[i]);  // {1, 3, 2, 4, 0}

		// [memo]元の入力について、ソート後のインデックスを求める方法は以下
		vector<int> after_idx(N);
		for(i = 0; i < N; i++) after_idx[a[i].second] = i;  // {4, 0, 2, 1, 3}
	}
	// 方法2：インデックス用配列を別に作り、値をキーとしてソートする
	{
		vector<int> a = inp;
		vector<int> a_idx(N);
		iota(a_idx.begin(), a_idx.end(), 0);  // {0, 1, 2, 3, 4}
		sort(a_idx.begin(), a_idx.end(), [&](int x, int y){ return a[x] < a[y]; });  // a[]の値でソート
		// a[a_idx[0]], a[a_idx[1]], a[a_idx[2]], ... でソートされている
		for(i = 0; i < N; i++) assert(a_idx[i] == ans[i]);  // {1, 3, 2, 4, 0}
		
		// [memo]元の入力について、ソート後のインデックスを求める方法は以下
		vector<int> after_idx(N);
		for(i = 0; i < N; i++) after_idx[a_idx[i]] = i;  // {4, 0, 2, 1, 3}
	}
}

// Zobrist Hashのメモ
void _zobrist_hash_(void)
{
	/*
	[概要]
	集合をハッシュすることで、2つの集合の一致判定を高確率で判定できる。判定の計算量はO(1).
	多重集合でも利用可能。後述。

	1.集合の要素にランダムな整数を割り当てる。
	2.集合のハッシュは割り当てられた整数をxorしたもの。

	多重集合の場合、割り当てられた整数を偶数回xorすると0になってしまい、区別がつかない。
	xorではなく加算することでハッシュにできる。かんたん。
	https://x.com/chokudai/status/875112616523218944/quotes

	[参考]
	https://trap.jp/post/1594/
	https://cympfh.cc/procon/hash.zobrist.html
	https://blog.hamayanhamayan.com/entry/2017/05/24/154618
	https://qiita.com/a_uchida/items/bec46c20fd2965c6e1a0

	[関連する問題]	
	ABC367-F
	*/
}

// "ヒストグラム中の最大正方形問題"のメモ
// 合わせてスライド最小値(最大値)についても記載する
void _largest_rectangle_histogram_(void)
{
	// ヒストグラム中の最大正方形問題
	// スライド最小値(最大値)
	// の順に記載する

	/*
	[ヒストグラム中の最大正方形問題]
	[概要]
	各iについて考えたときに、見るべき要素が単調増加(減少)になっていて、iをずらしていく場合、stackを用いて処理する。
	もしくは、「その時点までの単調」を管理するのにstackを用いる、という考え方。
	stackへの出し入れは各要素ごと高々1回なので、全体でO(N)で抑えられる。

	[実装方針]
	・スタックに入っている要素を見たとき、単調増加か単調減少か
	・要素を前から見ていくか、後ろから見ていくか
	・横方向の位置は保持必要か、不要か
	  (最大正方形問題なら横幅の情報が必要のため、必要)
	・ans[]はどのタイミングで決定するか forループの先頭か末尾か、またはスタック操作中か

	[参考]
	http://algorithms.blog55.fc2.com/blog-entry-132.html
	https://zenn.dev/k41531/articles/22df24117c1a15
	最大長方形問題は、上方向に何マスOKかを事前計算しておき、行ごとにヒストグラム問題を解く
	  https://lets-csharp.com/max-rectangle/
	  https://algorithmbeginner.blogspot.com/2020/06/blog-post.html

	[関連する問題]
	ABC359-E
	ABC372-D
	AOJ DPL_3_C https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_3_C
	AOJ DPL_3_B https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DPL_3_B 最大長方形

	[疑似コード]
	// 単調増加、前から見ていく、横方向の情報を保持必要 のケース
	h.push_back(0);  // 末尾番兵
	stack<pair<ll,ll>> st;
	st.push({0, -1});  // 先頭番兵
	ll ans = 0;
	for(i = 0; i < (int)h.size(); i++)
	{
		int x = i;
		// スタックにh[i]を入れたときに単調増加になるよう、h[i]より大きいものを削除していく
		// 合わせてansと横方向の情報を更新
		while(st.top().first > h[i])
		{
			auto [hh,xx] = st.top();
			st.pop();
			chmax(ans, (i-xx)*hh);
			x = xx;
		}
		// スタックに積む
		st.push({h[i], x});
	}

	// 単調増加、後ろから見ていく、横方向の情報は保持不要 のケース
	// ans[]への設定はループ内の先頭か末尾か、問題に応じて
	for(i = N-1; i >= 0; i--)  // 逆方向から見ていく
	{
		// スタックにh[i]を入れたときに単調増加になるよう、h[i]より小さいものを削除していく
		while(h[i] > st.top()) st.pop();
		// スタックに積む
		st.push(h[i]);

	}
	*/

	/*
	[スライド最小値(最大値)]
	[概要]
	配列内の長さKの各区間について、最小値を求める問題。
	最小値に(今後含め)なりうる要素を、deque内に単調増加になるよう積んでいく。これにより
	  現在のウィンドウの最小値
	  (ウィンドウはスライドしていくので)将来の最小値の候補
	が昇順に並ぶことになる。
	ここでdeque内の要素で、後から入ってきたa[i]より大きいものが存在する場合、それは最小値にはなりえない。
	つまりdequeから取り除くことができる。

	dequeへの出し入れは各要素ごと高々1回なので、全体でO(N)で抑えられる。

	[実装方針]
	・"ヒストグラム中の最大正方形問題"と同様と思われる。
	・ウィンドウをスライドしていく形なので、入れた要素は必ず一定期間で取り除かれる。
	  取り除く判定のため、要素番号の情報が必要。
	  ⇒a[idx]ではなくidxをdequeに持たせるというテクニック。

	[参考]
	https://perogram.hateblo.jp/entry/2020/09/24/075511
	https://qiita.com/kuuso1/items/318d42cd089a49eeb332

	[関連する問題]
	AOJ DSL_3_D https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_3_D

	[疑似コード]
	// dequeにa[i]ではなくidxを積む実装
	for(i = 0; i < N; i++)  // a[i]を加え、a[i-L]を取り除く
	{
		// a[i-L]を取り除く
		if(i-L >= 0 && de.front() == i-L) de.pop_front();

		// デックが単調増加になるよう積んでいく
		while(de.size() > 0 && a[de.back()] > a[i]) de.pop_back();
		de.push_back(i);

		if(i >= L-1) ans.push_back(a[de.front()]);
	}
	*/
}

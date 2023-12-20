#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>
#include <list>
#include <map>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <numeric>
using namespace std;

void _string_(void);
void _set_multiset_(void);
void _priority_queue_(void);
void _list_(void);
void _map_(void);
void _pair_(void);
void _mod_(void);
void _pow_(void);
void _unordered_set_(void);
void _unordered_map_(void);
void _stl_(void);
void _structured_bindings_(void);
void _tie_(void);
void _mex_(void);
void _math_(void);
void _value_with_index_(void);

int main(void)
{
	_priority_queue_();
	_set_multiset_();
	_pow_();
	_list_();
	_map_();
	_pair_();
	_unordered_set_();
	_unordered_map_();
	_mod_();
	_string_();
	_stl_();
	_structured_bindings_();
	_tie_();
	_mex_();
	_math_();
	_value_with_index_();

	/*
	AtCoderにてWAが出た場合のチェックポイント(ABC262, ABC319での反省点)
	特に、テストケースのうち半分がAC, もう半分がWAといった場合、コーナーケースの確認と合わせて以下をチェック。
	(1)変数はint型で良いか。long longにする必要は無いか。
	   入力はint型で収まっても、出力はlong longにする必要があるかもしれない。何通りあるか、など。
	   ★制約に10^9が見えたらlong longにしてしまった方が良い。 (ABC320-Dでの反省点)
	(2)「998244353 で割った余りを求めてください」のとき、計算の各所でMOD計算をしているか。
	(3)WAとなった場合、WAになるような入力例を何としても作る。
	(4)二分探索のとき、l,rの初期値は適切か、判定関数は適切かを再考する。初期値は特にlを見直す。
	(5)1が立っているbitをシフト演算を用いて計算する場合、
	   1にlong long等のキャストを付ける必要は無いか。
	*/

	/*
	ABCのC,D辺りの問題で、実装が重めの問題リスト。実装の練習に。
	ABC326-D
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
	ABC200-D
	ABC219-E
	ABC264-C
	ABC269-C
	ABC289-C
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
	  ABC201-D
	  ABC270-D
	  ABC298-E

	[確率DP、参考問題]
	ABC184-D
	ABC275-E
	ABC280-E
	ABC298-E
	ABC300-E
	ABC314-E
	ABC326-E

	[in-place DP, 参考問題]
	ABC203-E

	[DPの復元、参考問題]
	ABC200-D (ただし想定解法は異なる)
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
	★0-indexedとして、s[i]はaの先頭i個の総和をイメージする。
	
	するとa[]の[l,r)の半開区間の和は a_sum[r] - a_sum[l] で計算できる。
	[3,5)であれば a_sum[5] - a_sum[3] で、a[3]+a[4]の意味になる。

	[l,r]であれば a_sum[r+1] - a_sum[l] とすれば良い。
	[3,5]であれば a_sum[5+1] - a_sum[3] で、a[3]+a[4]+a[5]の意味になる。
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
	ABC278-E
	ABC313-E (別解)
	*/

	/*
	いもす法
	記録のフェーズとシミュレートのフェーズ、そして解の算出のフェーズに分ける。
	ABC256-D-2.cppを参照。
	https://imoz.jp/algorithms/imos_method.html
	[関連する問題]
	ABC268-E
	ABC153-F
	*/

	/*
	インタラクティブ問題
	ABC269-E
	ABC299-D
	ABC313-D
	*/

	/*
	主客転倒
	「和の形の数え上げ」は主客転倒に置き換えると解けるケースがありそう。
	  https://physics0523.hatenablog.com/entry/2020/01/12/052513
	[関連する問題]
	ABC151-E
	ABC214-D
	*/
	return 0;
}

void _lambda_(void)
{
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

	cout << func(10,20) << endl;
	
	// ラムダ式の外にある変数を使いたい場合、コピーキャプチャか参照キャプチャを使う
	// 前者は[=], 後者は[&]
	// また特定の変数のみを使いたい場合は[&num1, num2]など。
	// 基本的には[&]を使うことになりそう
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
	// vector<int> a = {3, 1, 4, 1, 5};  // 定義時に要素を指定
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
	/*
	vectorの三次元配列
	a[X][Y][Z]  -1で初期化
	vector<vector<vector<int>>> a(X, vector<vector<int>>(Y, vector<int>(Z, -1)));
	以下の形式でいけるらしい
	vector dp(X, vector(Y, vector<ll>(Z, -1)));
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
	// reverse, find, count, max(min), max_element(min), reduce

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
	// 合計値を特定の型にしたい場合、第3引数にて指定する
	reduce(a_v.begin(), a_v.end(), 0LL);  // long long型で計算
	reduce(a_v.begin(), a_v.end(), 0.0);  // double型で計算
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
// 現状はsqrt()などのライブラリというよりも、浮動小数点数の誤差に関する内容
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

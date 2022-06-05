#include <iostream>
#include <set>
#include <cmath>
#include <queue>
using namespace std;

void _set_multiset_(void);
void _priority_queue_(void);
void _pow_(void);

int main(void)
{
	_priority_queue_();
	_set_multiset_();
	_pow_();

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
	N個の中から好きな数だけ選ぶ → bit全探索
	for( i = 0; i < (1<<N); i++ )  // iについて、bitが1になっているものを選ぶ
	{
		for( j = 0; j < N; j++ )
		{
			if( ((i>>j)&1) == 0 ) continue;  // 演算子の優先順位注意 &1を()で囲む必要あり
			// 以降、j番目について選んだ場合の処理
		}
	
	}
	*/
	return 0;
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

	a[X]
	a[0], a[1], ... それぞれについてpush_back()していく
	vector<vector<int>> a(X, vector<int>());
	vector<vector<int>> a(X);
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
	#include <vector> が必要
	sort(a.begin(), a.end());  // 昇順
	sort(a.begin(), a.end(), greater<int>());  // 降順

	自作関数でソートしたい場合、
	bool myCompare(int left, int right) { return left < right; }
	のように用意しておいて、
	sort(a.begin(), a.end(), myCompare);
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
// 各操作の計算量は、要素数NのときにおおよそO(√N)
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

void _mod_(void)
{
	// *, /, % は同じ優先順位
	/*
	const ll MOD = 998244353;
	足し算、引き算、掛け算については計算途中でMODを計算して良い
	(a+b) * c % MOD
	->((a+b)%MOD) * c % MOD

	引き算については、引く前にMODを足しておいた方が良さそう
	(a-b)%MOD
	->(a+MOD-b)%MOD
	*/
	/*
	割り算は注意。
	「MODしてから除算」は正しく計算できない
	a/b % MOD
	-> (a%MOD)/b % MOD  は間違い
	「除算してからMOD」か、逆元をかける
	参考：https://qiita.com/drken/items/3b4fdf0a78e7a138cd9a
	*/
}

void _pow_(void)
{
	// #include <cmath>  が必要
	cout << pow(10, 3) << endl;  //10^3

	// long longの最大値(10^18)付近になる場合、powl()の方が良さそう   ★AtCoderの場合
	// pow() :戻り値はdouble型        64bit
	// powl():戻り値はlong double型   環境依存だが、128bitとか何とか
	// 参考：https://qiita.com/mod_poppo/items/8860505f38e2997cd021
}

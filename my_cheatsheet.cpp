#include <iostream>
#include <algorithm>
#include <set>
#include <cmath>
#include <list>
#include <map>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;

void _set_multiset_(void);
void _priority_queue_(void);
void _list_(void);
void _map_(void);
void _pair_(void);
void _pow_(void);
void _unordered_set_(void);
void _unordered_map_(void);

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
	*/

	/*
	等差数列
	初項a, 末項l, 公差d, 項数nの等差数列の和は、
	  n*(a+l)/2
	  n*(a + (a+(n-1)*d) )/2      ※末項l=a+(n-1)*d
	https://www.kwansei.ac.jp/hs/z90010/sugakua/suuretu/tousasum/tousasum.htm
	*/

	/*
	いもす法
	記録のフェーズとシミュレートのフェーズ、そして解の算出のフェーズに分ける。
	ABC256-D-2.cppを参照。
	https://imoz.jp/algorithms/imos_method.html
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
	#include <algorithm> が必要
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
	for(auto itr = mp.begin(); itr != mp.end(); itr++)
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

// ハッシュテーブルを用いた集合
// 同一要素を複数格納できず、格納順が規定されていない
// 挿入、削除、検索などの操作が、平均的にO(1)で可能
// 
// #include <unordered_set>  が必要
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

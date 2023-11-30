#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <set>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// MEXのメモ、ライブラリ

/*
 * MEXとは、ある集合Sに含まれていない要素のうち、最小の非負整数を指す。
 * 
 * ポイント：要素数がN個のとき、MEXは最大でもNまで。N+1以上は無視してよい。
 * 例：N=5のとき、要素={0,1,2,3,4}のときのMEX=5が最大。
 *     MEX=6になることはあり得ない。
 * 
 * [関連する問題]
 * ABC194-E
 * ABC330-E
 */

// MEXライブラリ
// ある数列Aがあったときに、各値の登場回数をカウントしておくことで、
// MEXをO(logN)で求めることができる。
// 使い方
//   (1)コンストラクタ(m)にて、MEXとしてあり得る最大値をmに指定する。
//      要素数がNであればNなど。
//   (2)要素の追加削除をAdd/Removeにて行う。
//   (3)GetにてMEXを取得する。
class Mex
{
private:
	int m;  // MEXとしてあり得る最大値
	set<int> non;  // 要素に存在しない値の集合＝MEX候補
	vector<int> cnt;  // cnt[i]:要素における数値iの個数

	void Init(void)
	{
		for(int i = 0; i <= m; i++) { non.insert(i); }
		cnt.assign(m+1, 0);  // [0]～[m]まで
	}

public:
	// m_:MEXとしてあり得る最大値
	Mex(int m_) : m(m_)
	{
		Init();
	}

	// 数値nを要素に追加する
	void Add(int n)
	{
		if(n > m) return;  // MEXにはなりえない値
		if(++cnt[n] == 1) non.erase(n);
	}

	// 数値nを要素から1つ削除する
	void Remove(int n)
	{
		if(n > m) return;
		if(cnt[n] == 0) return;  // nは要素に無い(念のためガード)
		if(--cnt[n] == 0) non.insert(n);
	}

	// MEXを返す
	// nonが空集合の場合のケアはしていない
	int Get(void)
	{
		return *non.begin();
	}
};

// ABC330-E
// https://atcoder.jp/contests/abc330/tasks/abc330_e
void solve_ABC330_E(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	Mex mex(N);
	vector<int> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		mex.Add(a[i]);
	}
	while(Q > 0)
	{
		int idx, x; cin >> idx >> x;
		idx--;
		mex.Remove(a[idx]);
		a[idx] = x;
		mex.Add(a[idx]);
		cout << mex.Get() << endl;
		Q--;
	}
}

int main(void)
{
	Mex mex(5);
	assert(mex.Get() == 0);
	mex.Add(0); mex.Add(0);
	mex.Add(1);
	assert(mex.Get() == 2);
	mex.Remove(0);
	assert(mex.Get() == 2);
	mex.Remove(0);
	assert(mex.Get() == 0);

	mex.Remove(0);
	mex.Add(6);
	mex.Remove(6);
	assert(mex.Get() == 0);

	solve_ABC330_E();

	return 0;
}

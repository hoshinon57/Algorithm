#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;

// ABC150 https://atcoder.jp/contests/abc150/
// 参考：再帰と深さ優先探索を用いた順列の生成
//   https://drken1215.hatenablog.com/entry/2020/05/04/190252

/*
 * 再帰と深さ優先探索を用いて、{1, 2, ..., N}までを使った順列を作成していく。
 * (順列の値自体はdicArray[]を使い、その要素番号に使うa[]の順列を作成する)
 * 同じ数値がa[]に積まれないようにしておく。
 * 辞書順に順列が作成されていくので、作成回数をカウントしておき、
 * P,Qと一致した際のカウンタが求めるa,bとなる。
 */
int N;
vector<int> p, q;
vector<int> dicArray;  // {1, 2, ..., N}を保持する配列
int pNo = 0, qNo = 0;  //P,Qの辞書順
int count_ = 0;  // 順列の作成回数
void func(vector<int> &a)
{
	int i;
	if(a.size() == N)  // 順列を生成したのでP,Qと比較
	{
		bool bEqual = true;  // P/Qと等しければtrue
		for(i = 0; i < N; i++)
		{
			if(dicArray[a[i]] != p[i])
			{
				bEqual = false;
				break;
			}
		}
		if(bEqual) pNo = count_;
		// 次はQ
		bEqual = true;
		for(i = 0; i < N; i++)
		{
			if(dicArray[a[i]] != q[i])
			{
				bEqual = false;
				break;
			}
		}
		if(bEqual) qNo = count_;
		count_++;
		return;
	}

	// 順列のため、a[]の値が重複しないようにする
	vector<bool> b(N, false);
	for(i = 0; i < a.size(); i++)
	{
		b[a[i]] = true;
	}
	for(i = 0; i < N; i++)
	{
		if(b[i]) continue;  // iはa[]に積み済
		a.push_back(i);
		func(a);
		a.pop_back();
	}

	return;
}

int main(void)
{
	cin >> N;
	p.resize(N);
	q.resize(N);
	dicArray.resize(N);
	for(int i = 0; i < N; i++) cin >> p[i];
	for(int i = 0; i < N; i++) cin >> q[i];
	for(int i = 0; i < N; i++) dicArray[i] = i+1;  // P,Qは1始まりのため、比較用

	vector<int> a;  // 生成した順列を入れていく
	func(a);

//	cout << pNo << "," << qNo << endl;
	cout << abs(pNo-qNo) << endl;
	return 0;
}

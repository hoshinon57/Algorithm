#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC258 https://atcoder.jp/contests/abc258

/*
 * クエリ1ごとに愚直に文字列操作を行っていると、TLEになる(はず)。
 *
 * 例えば、"abcdef"という文字列に対して"1 2"というクエリが来た場合、"efabcd"となる。
 * これは文字列のインデックスが0から0-2+6=4になることに等しい。(+6は文字列長)
 * よってインデックスを用意しておき、クエリ1が来たらインデックスを減算すればよい。
 * クエリ2が来た場合、インデックス値を加味してx番目の文字を出力する。
 */
int main(void)
{
	// 0-indexed
	int N, Q;
	cin >> N >> Q;
	string S;
	cin >> S;

	int index = 0;  // 元のSにおける何文字目が先頭か
	for(int i = 0; i < Q; i++)
	{
		int query, x;
		cin >> query >> x;
		if(query == 1)
		{
			index -= x;
			if(index < 0) index += N;
		}
		else  //query:2
		{
			int tmp;
			tmp = index + (x-1);  // 0-indexedにしたので-1して計算
			tmp %= N;
			cout << S[tmp] << endl;
		}
	}

	return 0;
}

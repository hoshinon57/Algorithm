#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// 「分野別 初中級者が解くべき過去問精選 100問」の問題18
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_4_B&lang=ja

/*
 * 二分探索で解く。
 * 
 * 数列Tの各要素について、それが数列Sに含まれるかを二分探索で求める。
 * 含まれる数をカウントしていけば、最終的にそれが答となる。
 * 
 * 計算量はO(qlogn).
 * 以下の実装では、二分探索は自前実装とした。
 * 標準ライブラリを使う場合はbinary_search()を用いる。
 */

int main(void)
{
	int i;
	int n, q;
	cin >> n;
	vector<int> s(n);
	for(i = 0; i < n; i++) cin >> s[i];
	cin >> q;

	// s[]にnumがあればtrueを返す
	// 二分探索で判定する
	auto check = [&](int num) -> bool
	{
		int l, r;
		// l, rの初期値は範囲外の値とする
		// while(r-l>1)という条件よりl, rの値はmidにならないため、この初期値もmidに選ばれることは無い
		l = -1;
		r = (int)s.size();
		while(r - l > 1)
		{
			int mid = (l+r)/2;
			if(s[mid] == num) return true;  // 見つかればtrueを返す

			if(s[mid] > num)
				r = mid;
			else
				l = mid;
		}
		return false;
	};

	int answer = 0;
	for(i = 0; i < q; i++)
	{
		int t;
		cin >> t;

		if(check(t)) answer++;

		// 自前実装ではなく、binary_search()を使う場合はこちら
		// if(binary_search(s.begin(), s.end(), t)) answer++;
	}
	cout << answer << endl;

	return 0;
}

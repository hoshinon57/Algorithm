#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC229 https://atcoder.jp/contests/abc229

/*
 * 二分探索で解く。
 * 
 * 「XをZ個以降連続させることができるか」の判定問題を考える。
 * これは先頭からn文字ぶんを見ていって、XがZ-K個以上あればOKとなる。計算量はO(|S|).
 * 
 * Zを変化させたときに、どこかで できる/できない が切り替わるので、
 * それを二分探索で求める。
 */

int main(void)
{
	string s; cin >> s;
	int K; cin >> K;
	
	int l = -1;
	int r = INF32;

	// Xをn個以上連絡させることができるならtrue
	auto check = [&](int n) -> bool
	{
		int i;
		if(n > (int)s.size()) return false;  // そもそも文字列Sがn文字未満
		int x_count = 0;

		for(i = 0; i < (int)s.size(); i++)
		{
			if(i-n >= 0 && s[i-n] == 'X') x_count--;  // (n+1)文字目以降で、n文字前が'X'なら範囲外になるので-1する
			if(s[i] == 'X') x_count++;
			if(x_count >= n-K) return true;
		}
		return false;
	};

	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(mid))
		{
			l = mid;			
		}
		else
		{
			r = mid;
		}
	}
	cout << l << endl;

	return 0;
}

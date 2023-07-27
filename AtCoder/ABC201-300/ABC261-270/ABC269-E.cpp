#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC269 https://atcoder.jp/contests/abc269

/*
 * 二分探索で解く。
 * 
 * 答となるマスは、「ルークが存在しない行」かつ「ルークが存在しない列」となる。
 * よって行と列をそれぞれ二分探索で求める。
 * 
 * ※公式解説の実装では右半開区間としているが、以下は閉区間で考える。
 * 
 * ルークが存在しない行について、"? l r 1 N"の形で調べる。
 * ルークが存在しない行が[l,r]のどこかにあるときに、mid=(l+r)/2 として[l,mid]の範囲で調べる。
 * ・応答が調べた行数より1小さい場合：
 *   [l,mid]の範囲であるので、r=midに更新する。
 * ・応答が調べた行数と一致する場合：
 *   [l,mid]の範囲外であるので、l=mid+1に更新する。
 * これを r>l (またはl!=r) の条件にて繰り返す。
 * 
 * 列についても同様に求めていく。
 */
int main(void)
{
	// 1-indexed
	int N;
	cin >> N;

	int UD, LR;  // それぞれ、ルークが存在しない行と列を指す
	int l, r;

	// "? l r 1 N" の形で二分探索し、ルークが存在しない行を探す
	l = 1;
	r = N;
	while(r > l)
	{
		// ルークが存在しない行は、[l,r]のどこかにある
		// [l,mid]の範囲で調べて、範囲を狭めていく
		int mid = (l+r)/2;
		cout << "? " << l << " " << mid << " " << 1 << " " << N << endl;
		int T;
		cin >> T;
		if(T == -1) return 0;  // 不正解

		// 調べた行の数は mid-(l-1) となる
		// 応答がこれより1小さいならば、調べた範囲に「ルークが存在しない行」が存在する

		if(T == (mid-(l-1)) - 1)  // ルークが存在しない行は、[l,mid]の範囲である場合
		{
			r = mid;
		}
		else  // ルークが存在しない行は、[l,mid]の範囲ではない場合
		{
			l = mid+1;  // +1に注意
		}
	}
	// l=rとなった場合にwhileから抜ける
	UD = l;
	
	// "? 1 N l r" の形で二分探索し、ルークが存在しない列を探す
	l = 1;
	r = N;
	while(r > l)
	{
		// ルークが存在しない列は、[l,r]のどこかにある
		// 二分探索で範囲を狭める
		int mid = (l+r)/2;
		cout << "? " << 1 << " " << N << " " << l << " " << mid << endl;
		int T;
		cin >> T;
		if(T == -1) return 0;  // 不正解

		// 調べた行の数は mid-(l-1) となる
		// 応答がこれより1小さいならば、調べた範囲に「ルークが存在しない列」が存在する

		if(T == (mid-(l-1)) - 1)  // ルークが存在しない列は、[l,mid]の範囲である場合
		{
			r = mid;
		}
		else  // ルークが存在しない列は、[l,mid]の範囲ではない場合
		{
			l = mid+1;  // +1に注意
		}
	}
	// l=rとなった場合にwhileから抜ける
	LR = l;

	cout << "! " << UD << " " << LR << endl;

	return 0;
}

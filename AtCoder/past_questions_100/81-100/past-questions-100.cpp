#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題100
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/tenka1-2018-beginner/tasks/tenka1_2018_d

/*
 * まず、どのようなNなら「存在する」かを調べてみた。
 * N=1のとき、
 *   S1:1
 *   S2:1
 * N=3のとき、
 *   S1:1 2
 *   S2:1   3
 *   S3:  2 3
 * N=6のとき、
 *   S1:1 2 3
 *   S2:1     4 5
 *   S3:  2   4   6
 *   S4:    3   5 6
 * N=10のとき、
 *   S1:1 2 3 4
 *   S2:1       5 6 7
 *   S3:  2     5     8 9 
 *   S4:    3     6   8   10
 *   S5:      4     7   9 10
 * のように、N=1から+2, +3, +4, +5, ... となっていると推測。
 * よって入力Nに対して、k*(k-1)/2=Nとなるような整数kが存在すれば、
 * 条件を満たすようなS1～Skが存在する、と言える。
 * 
 * 処理は以下の流れで進める。
 * (1)
 * 入力Nに対し、k*(k-1)/2=Nとなるような整数kを求める。
 * 実装では二分探索を用いたが、kを1ずつ増やしながら愚直に探索する方法でも十分に間に合うはず。
 * 整数kが存在しない場合、Noを出力して終了する。
 * (2)
 * 求めたkに対し、S1～Skを構築する。
 * S1～Skまで2つ選び(Si, Sj)、それらに同じ値を付与すればよい。
 * (3)
 * S1～Skを、問題文の指定に合わせて出力する。
 * 
 * 計算量は、(2)のS1～Sk構築の部分がいちばん影響する形か。
 * kの最大値をMAX_Kとすると、O(MAX_K^2)となる。
 * MAX_Kは450弱なので、O(MAX_K^2)=2*10^5ほど。
 */

int main(void)
{
	int N;
	cin >> N;

	// Skについて、k(k-1)/2=Nとなるようなkを二分探索で求める
	auto check = [&](int k) -> bool
	{
		if(k*k - k - 2*N < 0) return true;
		else return false;
	};

	// 入力Nに対し、k*(k-1)/2=Nとなるような整数kを求める。
	// ※練習のため二分探索としたが、kを1ずつ増やしながら愚直に探索する方法でも十分に間に合う
	int l, r;
	// l:k*(k-1)/2 < 0 を満たす
	// r:k*(k-1)/2 >= 0 を満たす
	l = 0;
	r = 1000;  // 1000*1000 > 10^5(Nの最大) なので、これで十分
	while(r - l > 1)
	{
		int mid = (l+r)/2;
		if(check(mid)) l = mid;
		else r = mid;
	}
	// 二分探索の終了後、r*(r-1)/2=Nになるか確認
	int k;
	if(r*(r-1)/2 == N)
	{
		k = r;
	}
	else
	{
		// 条件を満たすSが存在しないため、Noを出力して終了
		cout << "No" << endl;
		return 0;
	}

	// S1～Skの構築
	// S1～Skまで2つ選び(Si, Sj)、それらに同じ値を付与していく
	int count = 1;
	int i, j;
	vector<vector<int>> S(k+1);
	for(i = 1; i <= k-1; i++)
	{
		for(j = i+1; j <= k; j++)
		{
			S[i].push_back(count);
			S[j].push_back(count);
			count++;
		}
	}

	// 結果出力
	cout << "Yes" << endl;
	cout << k << endl;
	for(i = 1; i <= k; i++)
	{
		cout << S[i].size();
		for(auto &e : S[i])
		{
			cout << " " << e;
		}
		cout << endl;
	}

	return 0;
}

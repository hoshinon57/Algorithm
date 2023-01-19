#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数

// 「分野別 初中級者が解くべき過去問精選 100問」の問題5より
// https://qiita.com/e869120/items/eb50fdaece12be418faa
// 
// https://atcoder.jp/contests/abc095/tasks/arc096_a

/*
 * Aピザ/Bピザの購入枚数について全探索するとO(N^2)となりTLE.
 * ここではABピザの枚数について全探索する。
 * ABピザの最小枚数は0枚、最大は max(X, Y)*2 枚となる。
 * 
 * ABピザを買う枚数を決めたら、そこからAピザBピザを買う枚数が決まる。
 * これで購入代金が計算できるので、その最小値を求めればよい。
 * 
 * 計算量は、N=max(X,Y)としてO(N).
 */

int main(void)
{
	int A, B, C, X, Y;
	cin >> A >> B >> C >> X >> Y;
	
	int ab_max = max(X, Y)*2;
	int answer = INF32;
	for(int num_ab = 0; num_ab <= ab_max; num_ab += 2)  // ABピザの枚数について全探索
	{
		int num_a, num_b;  // AピザおよびBピザを買う枚数
		num_a = max(X-num_ab/2, 0);  // 最小0枚でクリップ
		num_b = max(Y-num_ab/2, 0);
		int price = num_a * A + num_b * B + num_ab * C;
		answer = min(answer, price);
	}

	cout << answer << endl;
	
	return 0;
}

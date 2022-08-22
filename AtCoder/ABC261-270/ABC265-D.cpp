#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 1 << 30;

// ABC265 https://atcoder.jp/contests/abc265

/*
 * コンテスト中に考えた方法。
 * 二分探索と累積和を使って解く。
 * 
 * Aについて愚直に1つずつ判定していくとTLE.
 * 累積和を用いて、sum[i]=a[0]+a[1]+...+a[i] と定義する。
 * すると、
 *   (1)sum[y-1]-sum[x-1]=P
 *   (2)sum[z-1]-sum[y-1]=Q
 *   (3)sum[w-1]-sum[z-1]=R
 * と表すことができる。
 * 
 * yについて1つずつ全探索したときに、(2)を満たすようなzが存在するか、二分探索で求める。
 * 同様に(3)のwと(1)のxも、存在するか二分探索で求める。
 * 
 * ※方針が悪く、可読性が悪かったりバグが出やすかったりしてイマイチの実装。
 *   設計が悪いとどうやっても実装が汚くなるという典型になってしまった。
 */

int main(void)
{
	// 0-indexed
	ll i;
	ll N, P, Q, R;
	cin >> N >> P >> Q >> R;
	vector<ll> a(N);
	vector<ll> a_sum(N);  // a_sum[i]:a[0]+a[1]+...+a[i]
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		if(i == 0)
		{
			a_sum[i] = a[i];
		}
		else
		{
			a_sum[i] = a_sum[i-1]+a[i];
		}
	}
	// ここまで入力

	// yについて全探索し、z(Q),w(R),x(P)の順に探していく
	// ll x, y, z, w;
	ll y, z;
	bool bOK = false;  // {x,y,z,w}の組が見つかればtrue
	for(y = 1; y <= N-2; y++)  // yの取りうる範囲は1～N-2
	{
		// まずはz(ないしQ)について二分探索
		ll z_l, z_r;
		// z_lは必ず <Q となるz,
		// z_rは必ず >=Q となる(または範囲外となる)z と決める
		// zの範囲はy<z<Nなので、範囲外となるy,Nを初期値とする
		z_l = y;
		z_r = N;
		while(z_r - z_l > 1)
		{
			ll z_mid = (z_l+z_r)/2;
			ll tmp = a_sum[z_mid-1] - a_sum[y-1];
			if(tmp < Q)
			{
				z_l = z_mid;
			}
			if(tmp >= Q)
			{
				z_r = z_mid;
			}
		}
		// z_rでの計算結果がQに等しいか
		if(a_sum[z_r-1] - a_sum[y-1] == Q)
		{
			z = z_r;  // zが見つかった
		}
		else continue;

		// 次にw(ないしR)について二分探索
		ll w_l, w_r;
		// w_lは必ず <R となるようなw,
		// w_rは必ず >=R となるようなw と決める
		// wの範囲はz<w<=Nなので、範囲外となるz,N+1を初期値とする
		w_l = z;
		w_r = N+1;
		while(w_r - w_l > 1)
		{
			ll w_mid = (w_l+w_r)/2;
			ll tmp = a_sum[w_mid-1] - a_sum[z-1];
			if(tmp < R)
			{
				w_l = w_mid;
			}
			else if(tmp >= R)
			{
				w_r = w_mid;
			}
		}
		// w_rでの計算結果がQに等しいか
		if(a_sum[w_r-1] - a_sum[z-1] == R)
		{
			// w = w_r;  // wが見つかった
		}
		else continue;

		// 次にx(ないしP)について二分探索
		ll x_l, x_r;
		// xの範囲は0<=x<yなので、範囲外となる-1,yを初期値とする
		x_l = -1;
		x_r = y;
		while(x_r - x_l > 1)
		{
			ll x_mid = (x_l+x_r)/2;
			ll tmp;
			if(x_mid > 0)
			{
				tmp = a_sum[y-1] - a_sum[x_mid-1]; 
			}
			else
			{
				tmp = a_sum[y-1];
			}
			// xだけ処理が逆で、xを左方向に動かすとtmpが大きくなる
			if(tmp <= P)
			{
				x_r = x_mid;  // x_lではなくx_rに設定
			}
			else if(tmp > P)
			{
				x_l = x_mid;  // x_rではなくx_lに設定
			}
		}
		// x_rでの計算結果がPに等しいか
		ll tmp;
		if(x_r > 0)
		{
			tmp = a_sum[y-1] - a_sum[x_r-1];
		}
		else
		{
			tmp = a_sum[y-1];
		}
		if(tmp == P)
		{
			// x = x_r;  // xが見つかった
			bOK = true;
			break;
		}
		else continue;
	}

	if(bOK)
	{
		cout << "Yes" << endl;
	}
	else
	{
		cout << "No" << endl;
	}

	return 0;
}

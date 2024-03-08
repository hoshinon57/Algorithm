#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
const ll MOD = 1000000007;
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:86 https://atcoder.jp/contests/typical90/tasks/typical90_ch

/*
 * bitごとに独立して考え、bit全探索で解く。
 * 
 * まず、bit演算はbitごとに独立となる。
 * よって「0ビット目について条件を満たす組数」「1ビット目について同様」…「59ビット目について同様」を求めると、
 * その積が答となる。
 * 
 * 「xビット目について条件を満たす組数」の求め方は、bit全探索を用いる。
 * 数列Aの長さはNのため、0～(2^N)-1までループし、
 * kビット目の0/1を、Akの(xビット目の)0/1に割り当てる。
 * そして各x,y,z,wの条件について、Ax|Ay|Az==(wのxビット目) を見たせばOK.
 * 
 * [ACまでの思考の流れ]
 * ・基本は全探索、という考え方。
 * ・bit演算はbitごとに独立に考えられる。
 */

int N, Q;
const int MAX_Q = 50;
ll x[MAX_Q+1], y[MAX_Q+1], z[MAX_Q+1], w[MAX_Q+1];

ll calc(int b)
{
	int i, j;
	ll cnt = 0;
	for(i = 0; i < (1<<N); i++)  // kビット目がAkに相当する
	{
		bool ok = true;
		for(j = 0; j < Q; j++)
		{
			int xx = ((i>>x[j])&1);
			int yy = ((i>>y[j])&1);
			int zz = ((i>>z[j])&1);
			int ww = ((w[j]>>b)&1);
			if((xx|yy|zz) != ww) ok = false;
		}
		if(ok) cnt++;
	}
	return cnt;
}

int main(void)
{
	int i;
	cin >> N >> Q;
	for(i = 0; i < Q; i++)
	{
		cin >> x[i] >> y[i] >> z[i] >> w[i];
		x[i]--;
		y[i]--;
		z[i]--;
	}

	ll ans = 1;
	for(i = 0; i < 60; i++)
	{
		ans *= calc(i);  // i番目のbit
		ans %= MOD;
	}
	cout << ans << endl;

	return 0;
}

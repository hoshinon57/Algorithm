#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC413 https://atcoder.jp/contests/abc413

/*
 * コンテスト時にハマった…
 * 
 * コーナーケース的な処理として、r=1,r=-1の2パターンがある。
 * これらを事前にチェックしておく。
 * 前者は全要素が同じならr=1として構築可能(Yes).
 * 後者は各要素の絶対値が同じで、正と負の要素数の差が1以下なら、構築可能(Yes).
 * 
 * 以下、等比数列は昇順にも降順にも並べられることから、
 * 昇順にして |r|>1 で考える。
 * a[i+1]/a[i]が各要素で一致するかの判定は、浮動小数点数での誤差が怖いので避ける。
 * a[i]*a[i] と a[i+1]*a[i-1] との比較により、整数での比較に持ち込む。
 * 
 * ※N=2は常にYesとなるが、以上の実装では特に意識する必要は無し。
**/

bool mycomp(ll l, ll r)
{
	return (abs(l) < abs(r));
}

bool solve(void)
{
	ll i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	// r=1を検出
	{
		sort(a.begin(), a.end());
		if(a[0] == a.back()) return true;
	}

	// r=-1を検出
	{
		auto ab = abs(a[0]);
		ll poc=0, nec=0;
		for(auto &e : a)
		{
			if(abs(e) != ab) break;
			if(e > 0) poc++;
			else nec++;
		}
		if(poc+nec == N && abs(poc-nec) <= 1) return true;
	}

	// 絶対値でソート
	sort(a.begin(), a.end(), mycomp);
	for(i = 1; i < N-1; i++)
	{
		if(a[i]*a[i] != a[i-1]*a[i+1]) return false;
	}
	return true;
}

int main(void)
{
	int T; cin >> T;
	while(T > 0)
	{
		T--;
		YesNo(solve());
	}

	return 0;
}

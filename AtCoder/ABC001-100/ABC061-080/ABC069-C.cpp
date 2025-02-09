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

// ABC069 https://atcoder.jp/contests/abc069

/*
 * 構築系問題。
 * 
 * 入力のaを
 *   4の倍数か
 *   そうではなく、2の倍数か
 *   それ以外
 * の3パターンに分ける。
 * 以下、上記の個数を順にp4,p2,p1とする。
 * 
 * p1<=p4のとき
 *   1 4 1 4 1 4 ... の1,4連続に、残った4と2を付与すれば条件を満たす、ので常にYes.
 * p1==p4+1のとき
 *   1 4 1 4 1 の時点では条件を満たす。
 *   2が1つでもあると、どこに入れても条件未達になるのでNo.
 *   2が無ければYes.
 * それ以外 (p1 >= p4+2)
 *   1 4 1 4 1 1 のように、どうやっても未達になる。
 */ 

bool solve(void)
{
	int i;
	int N; cin >> N;
	int p1=0, p2=0, p4=0;
	for(i = 0; i < N; i++)
	{
		int a; cin >> a;
		if(a%4 == 0) p4++;
		else if(a%2 == 0) p2++;
		else p1++;
	}
	if(p1 <= p4) return true;
	else if(p1 == p4+1)
	{
		return p2==0;
	}
	else return false;
}

int main(void)
{
	YesNo(solve());
	return 0;
}

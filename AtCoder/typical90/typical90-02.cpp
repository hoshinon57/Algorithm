#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// 競プロ典型90問:02 https://atcoder.jp/contests/typical90/tasks/typical90_b

/*
 * bit全探索で解く。以下の解説とほぼ同じ方針だった。
 *   https://twitter.com/e869120/status/1377391097836544001
 *   https://drken1215.hatenablog.com/entry/2021/06/12/151200
 * 
 * 長さNに対し、カッコ列の種類は2^N個。
 * 整数iについてbitごとに0:'(', 1:')'に割り当てると、0～2^N-1 のループで全探索できる。
 * 
 * カッコ列が正しいかどうかは、上の桁から見ていって
 *   '('ならカウンタを+1, ')'なら-1
 *   途中でカウンタが負になったらNG
 *   最後まで行って、カウンタ=0ならOK
 * で判定できる。
 * 
 * 計算量はbit全探索部分がO(2^N), 判定問題がO(N), 全体でO(N*2^N).
 * 
 * [ACまでの思考の流れ]
 * ・N=20なので2^N, さらにはbit全探索っぽい。[典型]
 * ・判定問題は、()の数が一致していることは大前提として、
 *   左から見ていったときに')'の方が一時でも多くなったらダメっぽい。[典型]
 */

int main(void)
{
	int i;
	int N; cin >> N;
	auto check = [&](int x) -> void
	{
		int j;
		int cnt = 0;
		string s;
		for(j = N-1; j >= 0; j--)  // j桁目 上の桁から見ていく
		{
			if(((x>>j)&1) == 0)
			{
				cnt++;
				s += '(';
			}
			else
			{
				cnt--;
				s += ')';
			}
			if(cnt < 0) return;
		}
		if(cnt == 0) cout << s << endl;
	};

	for(i = 0; i < 1<<N; i++)
	{
		check(i);
	}
	return 0;
}

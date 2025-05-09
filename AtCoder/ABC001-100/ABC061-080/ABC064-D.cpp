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

// ABC064 https://atcoder.jp/contests/abc064

/*
 * カッコ列が正しいかの判定は、「'('と')'の数が同じ」「左から見ていって、常に'('の数>=')'の数となっている」
 * という典型。
 * 
 * ここから、以下の手順でいける。
 * ・'('なら+1, ')'なら-1して、途中経過も含めた最小値をmiとして保存。
 *   (-mi)の数だけ先頭に'(を追加する。
 * ・sumの最終値と、(-mi)の数だけ、')'を末尾に追加する。
 */

int main(void)
{
	int N; cin >> N;
	string s; cin >> s;

	int i;
	int sum = 0, mi = INF32;
	for(i = 0; i < N; i++)
	{
		if(s[i] == '(') sum++;
		else sum--;
		chmin(mi, sum);
	}
	// -mi の数だけ先頭に'('を付ける

	string ans;
	for(i = 0; i < (-mi); i++) ans += '(';
	ans += s;
	for(i = 0; i < sum+(-mi); i++) ans += ')';
	cout << ans << endl;

	return 0;
}

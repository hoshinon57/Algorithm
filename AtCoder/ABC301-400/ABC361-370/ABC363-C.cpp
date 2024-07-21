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

// ABC363 https://atcoder.jp/contests/abc363

// next_permutation()を用いてSの並び替えを全探索。
// 各並び替えごとに、[i,i+k)の範囲の部分文字列が回文かどうかをチェック。全て非回文ならansに+1.

// 文字列sについて、stからk文字、つまり[st,st+k)の部分文字列が回文かを判定する
// 回文ならtrue
// st,kが未指定のときはs全体に対して判定する
bool is_palindrome(string &s, int st = -1, int k = -1)
{
	if(st == -1) { st = 0; k = (int)s.size(); }
	int ed = st+k-1;
	while(st <= ed)
	{
		if(s[st] != s[ed]) return false;
		st++; ed--;
	}
	return true;
}

int main(void)
{
	int i;
	int N, K; cin >> N >> K;
	string s; cin >> s;
	sort(s.begin(), s.end());

	int ans = 0;
	do
	{
		bool isP = false;  // 長さKの回文が含まれるならtrue
		for(i = 0; i <= N-K; i++)
		{
			if(is_palindrome(s, i, K)) isP = true;
		}
		if(!isP) ans++;
	} while (next_permutation(s.begin(), s.end()));
	cout << ans << endl;

	return 0;
}

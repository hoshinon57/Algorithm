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

// ABC380 https://atcoder.jp/contests/abc380

/*
 * いくつか実験してみると、以下が分かる。
 *   クエリKとlen=|S|から、出力すべき文字がSの何ループ目かが分かる。
 *   これをlpとすると popcount(lp) が奇数ならSを反転した状態、偶数ならそのままの状態となる。
 * 2進数表記で考えたとき、先頭のbitが1であれば
 * 以降のbitを反転させたものを末尾に連結する操作になっていることから分かる。
 * 
 * また、公式解説にある再帰的な方針でも実装してみた。solve2()の部分。
 *   https://atcoder.jp/contests/abc380/editorial/11362
 */

// [verify]ABC350-F
// 大文字小文字を反転して返す
// cが英字以外の挙動は未確認のため注意
char revLowUp(char c) {
	if(islower(c)) return toupper(c);
	else return tolower(c);
}

void solve2(void)
{
	string s; cin >> s;
	ll len = s.size();
	auto f = [](ll lp) -> int  // ループlp目(0-indexed)が正転か反転か
	{
		ll i;
		int rev = 0;
		for(i = 60; i >= 0; i--)
		{
			if( ((lp>>i)&1) == 1) rev = 1-rev;
		}
		return rev;
	};
	vector<char> ans;
	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		ll k; cin >> k; k--;
		int rev = f(k/len);
		char tmp = s[k%len];
		if(rev == 1) tmp = revLowUp(tmp);
		ans.push_back(tmp);
	}
	int sz_ = (int)ans.size();
	for(int i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;
}

int main(void)
{
#if 0
	solve2();
	return 0;
#endif
	string s; cin >> s;
	ll len = s.size();
	int Q; cin >> Q;
	vector<char> ans;
	while(Q > 0)
	{
		Q--;
		ll k; cin >> k; k--;
		ll lp = k/len;  // クエリkはSの何ループ目にあるか
		char tmp = s[k%len];
		// ループ番号にてbitが立っている個数が奇数なら、そのループはSを反転
		if(__builtin_popcountll(lp)%2 == 1)
		{
			tmp = revLowUp(tmp);
		}
		ans.push_back(tmp);
	}

	int sz_ = (int)ans.size();
	for(int i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}

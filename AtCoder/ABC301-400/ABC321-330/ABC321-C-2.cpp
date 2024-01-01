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

// ABC321 https://atcoder.jp/contests/abc321

/*
 * 公式解説の方法。
 *   https://atcoder.jp/contests/abc321/editorial/7261
 * 
 * 0～9は最大で1回までしか登場しない。
 * 各数字について使う/使わないをbit全探索で列挙し、各組み合わせについて321-like numberを作っていく。
 * 
 */

bool isbiton(ll x, int b) { return ((x>>b)&1); }  // xのbビット目が立っていればtrue (bは0-indexed)

int main(void)
{
	int i, j;
	ll K; cin >> K;
	vector<ll> list;
	for(i = 2; i < 1<<10; i++)  // i=0(空文字列), i=1(値0) は除外
	{
		string s;
		for(j = 9; j >= 0; j--)
		{
			if(!isbiton(i, j)) continue;
			s += to_string(j);
		}
		list.push_back(stoll(s));  // この時点でsは降順に並んでいる
	}
	sort(list.begin(), list.end());
	cout << list[K-1] << endl;

	return 0;
}

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <array>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC411 https://atcoder.jp/contests/abc411

/*
 * 公式解説の方針で実装してみた版。
 * クエリ先読み＆逆順処理で解く。
 *   https://atcoder.jp/contests/abc411/editorial/13359
 * 
 * 基本は上記解説の通り。
 * 「より具体的にアルゴリズムを記述すると」よりも、その手前のf(t,i)に関する立式の方が分かりやすかった。
 * これを愚直に実装していくか形。
 * 
 * 文字列について適切に反転をするのはABC411-D.cppと同じ。
 * 
 * うーん、これはABc411-D.cppのgit的なやり方の方がピンと来るかなあ。
 * f(t,i)に関する立式を整理するのが大変。
 * しいていえば、「置き換える」という処理はクエリ逆順を考えてみる、という形か。
**/

int main(void)
{
	// 0:サーバ, 1～N:PC
	ll i;
	ll N, Q; cin >> N >> Q;
	vector<int> q(Q), p(Q);
	vector<string> s(Q);
	for(i = 0; i < Q; i++)
	{
		cin >> q[i] >> p[i];
		if(q[i] == 2)
		{
			cin >> s[i];
		}
	}

	int idx = 0;
	vector<string> as;
	for(i = Q-1; i >= 0; i--)
	{
		if(q[i] == 1)
		{
			if(idx == p[i])
			{
				idx = 0;
			}
		}
		if(q[i] == 2)
		{
			if(idx == p[i])
			{
				as.push_back(s[i]);
			}
		}
		if(q[i] == 3)
		{
			if(idx == 0)
			{
				idx = p[i];
			}
		}
	}
	string ans;
	reverse(as.begin(), as.end());
	for(auto &e : as) ans += e;
	cout << ans << endl;

	return 0;
}

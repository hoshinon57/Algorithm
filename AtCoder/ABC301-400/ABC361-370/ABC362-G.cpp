#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <atcoder/all>

using namespace atcoder;
using namespace std;

typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC362 https://atcoder.jp/contests/abc362

/*
 * 自力で解けず、解説を見た。また初めてAC Libraryを使った。
 *   https://atcoder.jp/contests/abc362/editorial/10389
 *   https://www.youtube.com/watch?v=FrKaLxcMjkI
 *   https://x.com/kyopro_friends/status/1812122565483397590
 *   https://atcoder.github.io/ac-library/master/document_ja/string.html
 * 
 * 解説の通りで、suffix arrayを構築してその上で二分探索を行えばよい。
 * suffix arrayの構築：O(|S|)とのこと。
 * 1クエリあたり：O(|T|log|S|). Σ|T|の制約により、間に合う。
 * 
 * 各クエリにて、TおよびT+'~'がsuffix arrayのどこに位置するかをそれぞれ求めると、その差分が答となる。
 */

int main(void)
{
	string S; cin >> S;
	int slen = (int)S.size();
	vector<int> sa = suffix_array(S);

	auto check = [&](string &x) -> int
	{
		int l = -1, r = slen;
		while(r - l > 1)  // 二分探索の計算量はO(log|S|)
		{
			int mid = (l+r)/2;
			if(x <= S.substr(sa[mid], (int)x.size())) r = mid;  // 1回の比較はO(|T|)かな
//			if(x <= S.substr(sa[mid])) r = mid;  // これだとTLEした 最悪ケースで1回あたりO(|S|)になるため
			else l = mid;
		}
		return r;
	};

	int Q; cin >> Q;
	while(Q > 0)
	{
		Q--;
		string T; cin >> T;
		int tmp1 = check(T);
		T += '~';  // 英小文字より後ろにある文字
		int tmp2 = check(T);
		cout << tmp2-tmp1 << endl;
	}

	return 0;
}

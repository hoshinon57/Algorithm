#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <stack>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC372 https://atcoder.jp/contests/abc372

/*
 * コンテスト時、苦戦した。
 * 「ヒストグラム中の最大長方形問題」のように解く。
 * 
 * 各iごとに条件を満たすビルの番号を列挙すると、単調増加になっている。
 * 末尾から逆方向に見ていって、ビルiについて
 *   現在のスタックのサイズがans[i].
 *   h[i]をスタックに入れたいので、単調増加を満たすようになるまで、
 *   つまり h[i]>st.top() のあいだ、スタックの先頭を削除していく。
 *   スタックにh[i]を入れる。
 * で解ける。
 * 
 * スタックへの出し入れが各ビルごとに1回までなので、全体でO(N)で抑えられるのがポイント。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・各iについて考えたときに、見るべき要素が単調増加/単調減少になっている。
 * ・iをずらしながら、各iについて同様の問題を解きたい。
 * ・こういうケースでstackを用いた「ヒストグラム中の最大正方形問題」を考えてみる。
 */

int main(void)
{
	int i;
	int N; cin >> N;
	vector<int> h(N); for(i = 0; i < N; i++) {cin >> h[i];}
	stack<int> st;
	st.push(INF32);  // 末尾として番兵

	vector<int> ans;
	for(i = N-1; i >= 0; i--)  // 逆方向から見ていく
	{
		ans.push_back(st.size()-1);  // 番兵の分1引く
		while(h[i] > st.top()) st.pop();
		st.push(h[i]);
	}

	reverse(ans.begin(), ans.end());
	int sz_ = (int)ans.size();
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}

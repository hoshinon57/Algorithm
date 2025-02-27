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

// ABC062 https://atcoder.jp/contests/abc062

/*
 * 難しい。丁寧にパターン整理する問題。
 * 基本は各種解説の通りだが、自分はO(1)で実装していた。
 *   https://img.atcoder.jp/arc074/editorial.pdf
 *   https://drken1215.hatenablog.com/entry/2020/01/29/022100
 * 
 * (1)横にズバッとぶった切る直線を2つ作り、横長の長方形を3つ作るパターン
 * (2)切り方がいわゆるト型、つまり縦に1本引いて2分割し、右側を上限に分けるパターン
 * の2パターンしかない。
 * 縦にズバッと直線2本は、H,Wをswapして同じことをすればよいので。
 * 
 * (1)
 * 整数を3で割ったときの切り捨ても加味して、3等分する。
 * できた3つの面積をソートしてmax-minすればよい。これは(2)についても同様。
 * 
 * (2)
 * 縦に1本は、w/3とw/3+1の2パターンを試すと良さそう。
 * (wが3k+1か3k+2かで区別すれば1パターンにできそうだが、確証無かったためやめた)
 * 横に1本は、h/2でやれば良い。
 * 
 * (1),(2)それぞれ、入力値が2のケースに注意。
 * 例えば(1)はh=2のときは作れない。
 * (2)はw=2のとき、w/3=0になってしまうため除外する。
**/

int cal(int h, int w)
{
	int ret = INF32;
	vector<int> li;

	// できた3つの面積のmax-minを返す
	auto f = [](vector<int> &l) -> int
	{
		sort(l.begin(), l.end());
		return l.back() - l[0];
	};

	// 縦方向3等分
	int tmp1, tmp2;
	if(h != 2)
	{
		tmp1 = h/3;
		li.push_back(tmp1*w);
		tmp2 = (h*2)/3;
		li.push_back((tmp2-tmp1)*w);
		li.push_back((h-tmp2)*w);
		chmin(ret, f(li));
	}

	// ト型
	li.clear();
	if(w != 2)  // w/3
	{
		tmp1 = w/3;
		li.push_back(h*tmp1);
		tmp2 = h/2;
		li.push_back(tmp2*(w-tmp1));
		li.push_back((h-tmp2)*(w-tmp1));
		chmin(ret, f(li));
	}

	// w/3+1
	li.clear();
	tmp1 = w/3+1;
	li.push_back(h*tmp1);
	tmp2 = h/2;
	li.push_back(tmp2*(w-tmp1));
	li.push_back((h-tmp2)*(w-tmp1));
	chmin(ret, f(li));

	return ret;
}

int main(void)
{
	int H, W; cin >> H >> W;
	int ans = INF32;
	chmin(ans, cal(H, W));
	swap(H, W);
	chmin(ans, cal(H, W));
	cout << ans << endl;

	return 0;
}

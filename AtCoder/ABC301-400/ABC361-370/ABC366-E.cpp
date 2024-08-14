#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC366 https://atcoder.jp/contests/abc366

/*
 * 自力で解けず、解説を見た。尺取り法で解く。
 *   https://x.com/kyopro_friends/status/1822268563790430330
 *   https://atcoder.jp/contests/abc366/submissions/56580439
 * 
 * x,y独立で考えることができ、それぞれ分けて計算する。
 * まず、func(x)=Σ|x-xi| と定義する。(y側も同様)
 * 制約よりmin(xi)-Dからmax(xi)+Dの範囲で計算すればよく、この範囲でループさせて
 *   初回はΣを愚直に計算
 *   2回目以降は差分に着目してO(1)で計算 (※)
 * することで求める。
 * (※)現在見ているxより右側、左側にある点をrcnt,lcntとすると、
 *     xが右に移動すると総和はrcntだけ減り、lcntだけ増える。
 * 詳細は実装を参照。
 * 
 * func(x),func(y)を求めたら、その組み合わせで和がD以下の個数が答となる。
 * funcの結果をソートしておくことで、尺取り法で解くことができる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「x,y独立」とか「45回転の典型は本問は違う」とか、方向性のイメージはできたが実装ができなかった。
 * ・xsum,ysumは独立なので、組み合わせする際にそれぞれをソートしても問題ない、という点に思考が至らなかった。
 *   独立のものを組み合わせる場合は、ソートを試してみる。
 */

// xは昇順ソート済み
// xの基準点をずらしていったときの、それぞれのΣ|x-xi|をxsumに格納する
// xsumはソートして返す
void func(vector<ll> &x, ll D, vector<ll> &xsum)
{
	ll i;
	ll lcnt = 0, rcnt = x.size();
	ll sum = 0;
	int idx = 0;
	for(i = x[0]-D; i <= x.back()+D; i++)
	{
		if(i == x[0]-D)  // 初回は愚直に計算
		{
			for(auto &e : x) sum += abs(i-e);
		}
		else
		{
			sum -= rcnt;
			sum += lcnt;
		}
		xsum.push_back(sum);
		// rcnt,lcntの更新
		while(idx != (int)x.size())
		{
			if(i == x[idx])
			{
				rcnt--; lcnt++;
				idx++;
			}
			else break;
		}
	}
	sort(xsum.begin(), xsum.end());
}

int main(void)
{
	int i;
	ll N, D; cin >> N >> D;
	vector<ll> x(N), y(N);
	for(i = 0; i < N; i++) cin >> x[i] >> y[i];
	sort(x.begin(), x.end());
	sort(y.begin(), y.end());
	vector<ll> xsum, ysum;
	func(x, D, xsum);
	func(y, D, ysum);

	ll ans = 0;
	int idx = (int)ysum.size()-1;
	for(auto &e : xsum)
	{
		// xsum[*]+ysum[idx]<=D となるまでずらしていく
		while(e + ysum[idx] > D && idx >= 0) idx--;
		if(idx < 0) break;
		ans += idx+1;  // ysumは[0,idx]の範囲で条件を満たす
	}
	cout << ans << endl;

	return 0;
}

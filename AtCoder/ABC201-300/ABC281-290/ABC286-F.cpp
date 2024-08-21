#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <atcoder/all>
#include <cassert>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC286 https://atcoder.jp/contests/abc286

/*
 * 自力で解けず、解説を見た。中国剰余定理を用いて解く。
 *   https://atcoder.jp/contests/abc286/editorial/5588
 *   https://x.com/kyopro_friends/status/1616796095597936644
 *   https://note.com/uyisl/n/n02eed8a3208c
 *   https://note.com/syamashi/n/nbc4bc75dc903
 * 
 * 基本は公式解説の通り。
 * iからAiへ辺を張るようなグラフを考えると、例えば周期3のサイクルを作ると
 * ジャッジからの応答を見ることで「Nを3で割った余り」が分かる。
 * 最初の出力を {2,3,1,5,6,7,4} とすると前半3つが周期3、後半4つが周期4のサイクルなので
 * 組み合わせることで「Nを12で割った余り」が分かる。
 * ⇒中国剰余定理を使う。
 * 
 * よって互いに素となるサイクルを作っていくことで、全要素のlcmまでは求められそう。
 * lcm>N となるようにしたい。
 * ただしサイクルを 2,3,5,7,11,13,17,19,23,29 とすると、この時点で和が110を超えるため整数列Aを作れない。
 * 2,3,5,7,11,13,17,19,23 までに減らすと、今度はlcm(all A)<10^9 となり、一意に定まらない。
 * 互いに素であれば良いので、2を2*2=4, 3を3*3=9に変えると
 * 4,9,5,7,11,13,17,19,23 となり、和が108, lcmが1,338,557,220 > 10^9 となり、上手くいきそう。
 * 
 * 後は上手いこと最初の出力Aを構築し、
 * ジャッジ応答から「Nをmで割った余りr」を取得していく。
 * {m,r}を取得したら、ACLのcrt()にて答が求まる。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・iをAiに置き換える操作を何度も行う、はグラフで考えてみる。
 * ・「Nをmで割った余りがr」という情報が何種類も取得できる場合、中国剰余定理を考えてみる。
 */

int main(void)
{
	int i;
	vector<int> c = {4,9,5,7,11,13,17,19,23};
	int M = reduce(c.begin(), c.end());
	assert(M == 108);
	vector<int> a;
	int ofs = 1;
	for(auto &e : c)
	{
		for(i = 1; i < e+1; i++)  // c=4なら2,3,4,1を出力
		{
			if(i != e) a.push_back(i+ofs);  // 末尾以外
			else a.push_back(ofs);  // 末尾
		}
		ofs += e;
	}
	cout << M << endl;
	int sz_ = (int)a.size();
	for(i = 0; i < sz_; i++) {
		cout << a[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	vector<int> b(M); for(i = 0; i < M; i++) {cin >> b[i];}
	ofs = 0;
	vector<ll> r,m;
	for(auto &e : c)
	{
		// Nをmで割った余りがr
		r.push_back(b[ofs]-ofs-1);
		m.push_back(e);
		ofs += e;
	}
	auto [y,z] = atcoder::crt(r, m);
	if(y == 0) y = z;  // 0<=y<z に対しNは1以上のため ただlcm(all m)>10^9なので、不要だったかも
	cout << y << endl;

	return 0;
}

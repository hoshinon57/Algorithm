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

// ABC343 https://atcoder.jp/contests/abc343

/*
 * コンテスト中に解けず、解説を見た。
 * 「基本は全探索」の考え方で解く。参考問題はABC307-C(Ideal Sheet), ABC322-D(Polyomino)など。
 *   
 * C1の始点を原点(0,0,0)に置く。
 * C2,C3の始点を-7～+7の範囲で置けば、全ての場合について探索できる。
 * -7～+7は15通りなので、xyzとC2C3について15^6=1.1*10^7通りであり、各置き方について体積の計算がO(1)でできれば間に合う。
 * 
 * 立方体のX方向の位置を[xl,xr)とする。xl+7=xrとなる。Y,Z方向も同様。
 * 立方体C1,C2の重なる部分は、X方向について
 *   xl=max(C1とC2のxl), xr=min(C1とC2のxr)
 *   ※xr<xlとなる場合、xl=xrとしておく。
 * となる。Y,Zも同様。
 * 重なった領域をそれぞれC12,C23,C31,C123とすると、包除原理より
 * V1に相当する体積：(C1+C2+C3)-(C12+C23+C31)*2+C123*3
 * V2に相当する体積：(C12+C23+C31)-C123*3
 * V3に相当する体積：C123
 * となる。
 * これが入力のV1,V2,V3と一致すればOK.
 * 
 * [どう思考すれば解法にたどり着けるか / 注意点]
 * ・「基本は全探索」で考えてみる。
 * ・重なる部分の領域がO(1)で求められるかを考える。
 *   また、包除原理を使って「x個に含まれる領域」をO(1)で求められるかを考える。
 * ・【注意点】
 *   配置する系の全探索は、ベース位置から正負両方向について探索すること。
 *   本問の場合、ベース位置はC1を置く原点に相当する。
 *   C2,C3を0以上の範囲で置こうとすると、範囲をどう設定してもWAになるケースが存在する。
 *     https://twitter.com/e869120/status/1763931692245688445
 *   ※参考として挙げたABC307-C(Ideal Sheet)も同様だったはず。
 */

struct cube
{
	int xl, xr;  // [xl, xr)
	int yl, yr;
	int zl, zr;
};

// c1,c2にて重なる領域を返す
cube mix(cube c1, cube c2)
{
	cube ret;
	ret.xl = max(c1.xl, c2.xl);
	ret.xr = min(c1.xr, c2.xr);
	if(ret.xr < ret.xl) ret.xr = ret.xl = 0;
	ret.yl = max(c1.yl, c2.yl);
	ret.yr = min(c1.yr, c2.yr);
	if(ret.yr < ret.yl) ret.yr = ret.yl = 0;
	ret.zl = max(c1.zl, c2.zl);
	ret.zr = min(c1.zr, c2.zr);
	if(ret.zr < ret.zl) ret.zr = ret.zl = 0;
	return ret;	
}

int vol(cube c)
{
	int ret = 0;
	ret = (c.xr-c.xl)*(c.yr-c.yl)*(c.zr-c.zl);
	return ret;
}

int main(void)
{
	int V1, V2, V3; cin >> V1 >> V2 >> V3;
	cube c1 = {0, 7, 0, 7, 0, 7};  // 1個目の立方体は固定させる
	cube c2, c3;
	cube c12, c23, c31, c123;
	int v1, v2, v3, v12, v23, v31, v123;
	v1 = vol(c1);
	for(int x2 = -7; x2 <= 7; x2++)
	{
		for(int y2 = -7; y2 <= 7; y2++)
		{
			for(int z2 = -7; z2 <= 7; z2++)
			{
				c2 = {x2, x2+7, y2, y2+7, z2, z2+7};
				v2 = vol(c2);
				c12 = mix(c1, c2);
				v12 = vol(c12);
				for(int x3 = -7; x3 <= 7; x3++)
				{
					for(int y3 = -7; y3 <= 7; y3++)
					{
						for(int z3 = -7; z3 <= 7; z3++)
						{
							c3 = {x3, x3+7, y3, y3+7, z3, z3+7};
							c23 = mix(c2, c3);
							c31 = mix(c3, c1);
							c123 = mix(c12, c3);
							v3 = vol(c3);
							v23 = vol(c23);
							v31 = vol(c31);
							v123 = vol(c123);
							int v1_ = v1 + v2 + v3 - (v12+v23+v31)*2 + v123*3;
							int v2_ = v12 + v23 + v31- v123*3;
							int v3_ = v123;
							if(V1 == v1_ && V2 == v2_ && V3 == v3_)
							{
								cout << "Yes" << endl;
								cout << "0 0 0 " << x2 << " " << y2 << " " << z2 << " " << x3 << " " << y3 << " " << z3 << endl;
								return 0;
							}
						}
					}
				}
			}
		}
	}
	cout << "No" << endl;

	return 0;
}

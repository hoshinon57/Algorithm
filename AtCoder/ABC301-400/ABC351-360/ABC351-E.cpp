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

// ABC351 https://atcoder.jp/contests/abc351

/*
 * コンテスト中に解けず、解説を見た。
 * 考え方はevima氏の動画を、提出コードは公式解説を参考にした。
 *   https://www.youtube.com/watch?v=RKcioGk8XfM
 *   https://atcoder.jp/contests/abc351/editorial/9890
 * 
 * 斜め移動なので、「マンハッタン距離は45度回転」の典型を参考に、45度回転してみる。
 * 入力x,yに対して
 *   X=x+y
 *   Y=x-y
 * と置き換えると、元の移動について
 *   (x+1,y+1):Xを+2, Yは同じ
 *   (x+1,y-1):X同じ, Yを+2
 *   (x-1,y+1):X同じ, Yを-2
 *   (x-1,y-1):Xを-2, Yは同じ
 * となり、X,Yについて独立に考えることができる。
 * 例えば2点(x,y)=(0,0),(1,3)のとき、(X,Y)=(0,0),(4,-2)なので、
 *   X方向：4/2=2
 *   Y方向：2/2=1
 * の計3回移動となる。
 * 
 * 後は、(x,y)の和の偶奇の2パターンで分けて計算すればよい。
 * XiとXjについて |Xi-Xj|/2 となるが、
 * これは昇順ソートしておくことで絶対値を外すことができる。
 * i番目のXiについて Xi-Xj の加算側、減算側が何回出てくるかを考えると、
 *   加算側：i個  ※X0～X(i-1)のi個
 *   減算側：(size-i-1)個
 * となる。いわゆる主客転倒かな。
 * 
 * 移動量は2なので、最後に2で割れば答。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・「マンハッタン距離は45度回転」は思い浮かんだものの、そこから進められなかった。
 *   (X,Y)へ回転させた後、元のナナメ移動はX,Yについて独立になる、という考え方をしっかり覚えておく。
 * ・各要素Xi,Xjについて |Xi-Xj| を計算したいとき、昇順ソートをすれば絶対値を外せるという考え方。
 *   またこのとき、i番目のXiについては
 *     加算回数：i回
 *     減算回数：(size-i-1)回
 *   となる(主客転倒)。
 */

int main(void)
{
	ll i, j;
	ll N; cin >> N;
	vector<vector<ll>> xy(4);  // x+yの偶奇と、x+y,x-yでの計4パターン
	for(i = 0; i < N; i++)
	{
		ll x, y; cin >> x >> y;
		if((x+y)%2 == 0)
		{
			xy[0].push_back(x+y);
			xy[1].push_back(x-y);
		}
		else
		{
			xy[2].push_back(x+y);
			xy[3].push_back(x-y);
		}
	}

	ll ans = 0;
	for(i = 0; i < 4; i++)
	{
		sort(xy[i].begin(), xy[i].end());
		ll sz = (ll)xy[i].size();
		for(j = 0; j < sz; j++)
		{
//			ll tmp = (j-(sz-j-1)) * xy[i][j] / 2;  // ここで/2はダメ 最後に/2する
			ll tmp = (j-(sz-j-1)) * xy[i][j];
			ans += tmp;
		}
	}
	ans /= 2;
	cout << ans << endl;

	return 0;
}

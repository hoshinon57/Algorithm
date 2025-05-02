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

const double eps = 0.000000001;
// const double eps = 0.0000001;  // これだとWAだった

// ABC033 https://atcoder.jp/contests/abc033

/*
 * 偏角ソートを用いて解く。典型90に類題があったやつ。
 * ACした後にWEB検索して参考になった記事は以下。
 *   公式解説
 *   https://kyopro.hateblo.jp/entry/2019/02/06/010004
 *   https://atcoder.jp/contests/abc033/submissions/5912162
 * 
 * 各点iについて、そこの角度が直角ないし鈍角になるものがあるか探していく。
 * 「直角が2つ」「鈍角が2つ」な三角形は存在しないことから、
 *   直角三角形の数：直角の数と同じ
 *   鈍角三角形の数：鈍角の数と同じ
 *   鋭角三角形の数：作れる三角形の数から上記を引いたもの
 * として求められる。
 * 
 * 点iについて、そこを中心に残りの点の偏角を求めてソートする。
 * このとき2周分持たせると実装が楽。[円環は2周持たせる典型]
 * ソートした各要素について、その要素から+90°の要素があるかを探して、あればそれが直角となる。
 * ★このとき、制約より3点は同一直線上には無いので、直角になるのは高々1つ、として実装してよい。
 * 
 * 同じく+90°～+180°の要素があれば、それが鈍角。
 * こちらは複数個存在するので二分探索を2回行う。
 * 
 * 以上をEPSを用いて実装した。
 * (※)EPSの値をどうするかを考えたとき、
 *    x,yの範囲より3点(-10000,-10000),(10000,-9999),(-10000,10000)の「ほぼ直角」の角度がいくつかを
 *    (ChatGPTで)計算した感じ、10^(-7)でいけそうと思ったがWA.
 *    10^(-9)でACだった。
**/

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val未満の最右のidx, val未満の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_miman(vector<T> &a, T val) {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}

int main(void)
{
	int i, j;
	ll N; cin >> N;
	vector<double> x(N), y(N); for(i = 0; i < N; i++) {cin >> x[i] >> y[i];}

	ll ans1 = 0, ans2 = 0, ans3 = 0;  // 鋭角, 直角, 鈍角
	for(i = 0; i < N; i++)  // 点iについて
	{
		vector<double> li;  // 偏角リスト
		for(j = 0; j < N; j++)
		{
			if(i == j) continue;
			double tx, ty;
			tx = x[j] - x[i];
			ty = y[j] - y[i];
			double th = atan2(ty, tx) + M_PI;  // [0, 2π]
			li.push_back(th);
			li.push_back(th + M_PI*2);
		}
		sort(li.begin(), li.end());
		for(j = 0; j < N-1; j++)
		{
			auto t = *lower_bound(li.begin(), li.end(), li[j]+M_PI_2-eps);  // +pi/2
			if(abs(li[j]+M_PI_2-t) < eps)
			{
				ans2++;
			}
			// pi/2～pi
			auto[idx1,cnt1] = bi_idxnum_miman(li, li[j]+M_PI-eps);
			auto[idx2,cnt2] = bi_idxnum_miman(li, li[j]+M_PI_2+eps);
			ans3 += cnt1-cnt2;
		}
	}
	ans1 = N*(N-1)*(N-2)/6 - ans2 - ans3;
	cout << ans1 << " " << ans2 << " " << ans3 << endl;

	return 0;
}

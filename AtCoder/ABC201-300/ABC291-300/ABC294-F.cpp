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

// ABC294 https://atcoder.jp/contests/abc294

/*
 * 二分探索で解く。
 * 平均や濃度は二分探索。 [典型]
 * 大きい方からK番目の要素、も二分探索。 [典型]
 * 
 * 「濃度x%以上となる組み合わせがK個以上あるか？」の判定問題を考える。
 * 砂糖aグラムと水bグラムのある容器を考えると、
 *   100*a/(a+b) >= x  を式変形して、
 *   a-x(a+b)/100 >= 0  が得られる。
 * この左辺が、目標x%に対して砂糖が何グラム足りてるか or 不足しているかを表している。
 * これをリード値とすると、複数容器を混ぜたときに、各リード値の和が0以上ならx%以上になる。
 * 
 * あとは2人の砂糖水を混ぜる処理を考える。
 * まず青木君のM本について、それぞれリード値を計算してソートしておく。
 * 次に高橋君のから1本を選んでリード値を計算し、
 * 青木君のと加算したときにリード値が正になる組み合わせの数をlower_boundで求める。
 * 組み合わせの数がK以上なら、判定問題はtrue.
 * 
 * 二分探索の回数は、判定問題の計算量がO(NlogM)とかなので、200回ぐらいが上限かなと考えた。
 */

int main(void)
{
	int i;
	ll N, M, K; cin >> N >> M >> K;
	vector<double> a(N), b(N), c(M), d(M);  // 砂糖a, 水b,  砂糖c, 水d
	for(i = 0; i < N; i++) cin >> a[i] >> b[i];
	for(i = 0; i < M; i++) cin >> c[i] >> d[i];

	// 濃度x%以上の組み合わせがK個以上あれば、true
	auto check = [&](double x) -> bool
	{
		vector<double> ao;
		for(i = 0; i < M; i++)
		{
			double tmp = c[i] - x*(c[i]+d[i])/100.0;
			ao.push_back(tmp);
		}
		sort(ao.begin(), ao.end());

		ll ret = 0;
		for(i = 0; i < N; i++)
		{
			double ta = a[i] - x*(a[i]+b[i])/100.0;
			ret += ao.end() - lower_bound(ao.begin(), ao.end(), -ta);  // 高橋、青木で足して0以上の組み合わせ
		}
		return (ret >= K);
	};

	double ok = 0.0, ng = 100.0;
	for(int _ = 0; _ < 200; _++)
	{
		double m = (ok+ng)/2.0;
		if(check(m)) ok = m;
		else ng = m;
	}
	cout << std::fixed << std::setprecision(15);
	cout << ok << endl;

	return 0;
}

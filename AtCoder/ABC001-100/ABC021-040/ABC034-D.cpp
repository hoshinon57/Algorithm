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

// ABC034 https://atcoder.jp/contests/abc034

/*
 * 濃度(平均値)の最大かは二分探索。[典型]
 * 
 * 「濃度x%以上の食塩水を作れるか？」の判定問題を考える。
 * 各容器について (pi-x)*wi だけ目標からリードできる、というイメージで、この値を求めていく。
 * 降順ソートして上から順にK個選び、総和が正であれば作成可能。
 * 
 * 上記の式はイメージで求めたが、数学的に導くなら以下。
 *   K個選んだときの濃度は (p1w1+p2w2+...+pKwK)/(w1+w2+...+wK) なので、これがxより大きければ良い。
 *   整理すると (p1-x)*w1 + (p2-x)*w2 + ... + (pK-x)*wK > 0 となり、上記が導かれる。
 */

int main(void)
{
	int i;
	ll N, K; cin >> N >> K;
	vector<double> w(N), p(N);
	for(i = 0; i < N; i++) cin >> w[i] >> p[i];

	// 濃度x%以上を作れるならtrue
	auto check = [&](double x) -> bool
	{
		vector<double> ld;
		for(i = 0; i < N; i++)
		{
			ld.push_back((p[i]-x)*w[i]);
		}
		sort(ld.begin(), ld.end(), greater<double>());
		double sum = 0.0;
		for(i = 0; i < K; i++) sum += ld[i];

		return sum>0.0;
	};

	double ok = 0.0, ng = 100.0;
	for(int _ = 0; _ < 1000; _++)
	{
		double m = (ok+ng) / 2.0;
		if(check(m)) ok = m;
		else ng = m;
	}

	cout << std::fixed << std::setprecision(15);
	cout << ok << endl;

	return 0;
}

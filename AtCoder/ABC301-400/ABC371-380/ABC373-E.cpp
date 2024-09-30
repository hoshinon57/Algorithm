#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC373 https://atcoder.jp/contests/abc373

/*
 * コンテスト時にコーナーケースのWAが取り切れなかった。
 * 二分探索で解く。結構な重実装問題だった。
 * 
 * まず、二分探索のためにAiをソートしておく。
 * 
 * i番目の候補者について以下の判定問題を考える。
 *   得票がx票になったとき、当確か？
 * x票で当確なら(x+1)票でも当確であるので、二分探索で解ける。
 * 
 * 判定問題を解くのが難しい。
 * x票になったとき、他に(x+1)票取れる人がM人いたらNG.
 * よって自分以外の上位M人を(x+1)票にできるか、を考えてみる。
 * まず既に(x+1)票以上がM人いたらNG.
 * 上位M人を「(x+1)票以上」と「x票以下」で分ける。これは二分探索で求める。
 * 後者の現状の得票数(=sum)を求める。これは事前に累積和を計算することでO(1)で可能。
 * 
 * (1)sumに残票数を足したもの
 * (2)(x+1)*(x票以下の人数)
 * を比較し、(1)>=(2)であれば「自分以外の上位M人を(x+1)票にできる」となり、NGである。
 * 
 * 本問の難しい点として、上記のsumを求める際に自分自身が含まれるケースがある。
 * 含まれていれば自分自身を除外し、1つ下の得票数を加算する必要がある。
 * ここは詳しくは実装を参照。
 * 
 * …しかし、コンテスト時に1WAがどうしても取れなかった。
 * N=Mのケースを見逃しており、このとき全員が当確なのでALL 0を出力する必要がある。
 * ※複数人の実装を見たところ、N=Mを判定して別処理としている人が多そうだった。
 * 
 * [どう思考すれば解法にたどり着けるか]
 * ・二分探索はすぐに見えた。
 *   二分探索の初期値も適切に決められた。
 * ・実装がかなり大変だったが、デバッグしながら何とかやり切れた。
 * ・N=Mのコーナーケースを見つけられなかったのが悔しい。
 *   1<=M<=N という部分からM=Nのケースに気付けるようにする。
 */

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {valより大きい最左のidx, valより大きい要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_koeru(vector<T> &a, T val) {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}

int main(void)
{
	ll i;
	ll N, M, K; cin >> N >> M >> K;
	vector<ll> a(N);
	vector<pair<ll,ll>> a_idx(N);  // {val,idx}
	ll totrem = K;  // 残票数
	for(i = 0; i < N; i++)
	{
		cin >> a[i];
		a_idx[i] = {a[i], i};
		totrem -= a[i];
	}
	sort(a.begin(), a.end());
	sort(a_idx.begin(), a_idx.end());
	vector<ll> a_sum(a.size()+1); for(int kk = 0; kk < (int)a.size(); kk++) {a_sum[kk+1] = a_sum[kk] + a[kk];}

	// コーナーケース 全員が何もせずとも当確
	if(N == M)
	{
		vector<ll> ans(N);
		int sz_ = (int)ans.size();
		for(i = 0; i < sz_; i++) {
			cout << ans[i];
			if(i != sz_-1) cout << " ";
		}
		cout << endl;
		return 0;
	}

	// idx番目の人が総数x票になったとき、当確ならtrue
	auto check = [&](ll idx, ll x) -> bool
	{
		auto [overx_idx, overx_num] = bi_idxnum_koeru(a, x);
		if(overx_num >= M) return false;  // すでにx票より多い人数がM人以上いるので当選不可
		// [N-M, overx_idx)
		ll sum = a_sum[overx_idx] - a_sum[N-M];
		// 自分自身が含まれていれば、除外して1つ下の得票数を加算
		if(a[N-M] <= a[idx])
		{
			sum -= a[idx];
			sum += a[N-M-1];
		}
		ll tmp1 = sum + (totrem - (x-a[i]));  // sumに残票数を足したもの
		ll tmp2 = ((overx_idx) - (N-M)) * (x+1);  // (x+1)*(x票以下の人数)
		if(tmp1 >= tmp2) return false;
		else return true;
	};

	vector<ll> ans(N);
	for(i = 0; i < N; i++)
	{
		ll l = a[i]-1;
		ll r = a[i]+totrem+1;
		// i番目の候補者がmid票になった場合の当確を判定
		while(r - l > 1)
		{
			ll mid = (l+r)/2;
			if(check(i, mid)) r = mid;
			else l = mid;
		}

		if(r == a[i]+totrem+1) ans[a_idx[i].second] = -1;  // 当確にはならない
		else ans[a_idx[i].second] = r - a[i];
	}

	int sz_ = (int)ans.size();
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;

	return 0;
}

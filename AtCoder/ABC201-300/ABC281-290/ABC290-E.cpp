#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;
typedef long long ll;
const ll INF64 = 1LL << 60;
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC290 https://atcoder.jp/contests/abc290

/*
 * idx[i]を、
 *   数列Aのうち、値iが出てくる位置を列挙したもの
 * とする。ここでは1-indexedとした。
 * A={5,2,1,2,2}であれば、idx[5]={1}, idx[2]={2,4,5}となる。
 * またidx[i]の累積和をidx_sum[i]とする。ただしidx[i][0]=0とする。
 * 
 * ここで、(i,j)についてAi,Ajが等しいとき、何回判定されるかを考える。
 * N=5で(1,2)ならば1回、(2,3)ならば2回、(4,5)ならば1回などとなり、
 *   iの左側の要素数と、jの右側の要素数のうち、小さい方
 * の回数だけ判定されることが分かる。
 * jを固定したときに、jの右側の要素数をrとすると、idx[a[j]]において
 *   r未満の要素i：iの左側の要素数が判定回数なので、idx[a[j]][i]の和
 *   r以上の要素i：jの右側の要素数が判定回数なので、その要素数*r
 * を求めればよい。
 * 前者の計算には累積和を用いる。
 * これをXとする。
 * 
 * 同じような計算を全要素の比較回数としても計算し、これをYとすると、
 * Y-Xが答となる。
 * 
 * 自身はlower_bound()を用いながらO(NlogN)解法となったが、
 * 公式解説ではO(N)解法だった。
 * 
 * [ACまでの思考の流れ]
 * ・数え上げは主客転倒というのを考えてみる。
 * ・(i,j)のjを固定したときに、各iについて何回判定されるかを考える。
 * ・i,jの位置によって判定回数が変わるため、それを計算するにはどうするかを考える。
 *   ⇒lower_bound()や累積和にたどり着いた
 */

int main(void)
{
	int i;
	ll N; cin >> N;
	vector<ll> a(N); for(i = 0; i < N; i++) {cin >> a[i];}

	vector<vector<ll>> idx(200005);  // idx[i]={x1, x2, x3, ...} aにてiがある位置 1-indexed
	vector<vector<ll>> idx_sum(200005+1, vector<ll>(1, 0));  // idx[i]の累積和 各要素、0を初期値として持たせておく
	vector<ll> tot;  // 全要素の比較回数を計算する用
	vector<ll> tot_sum(1, 0);
	ll ans = 0;
	// 自身の右側に要素がr個あるとき、arrの各要素とはトータル何回比較するかを返す
	// arr[]のうち、r未満の要素については累積和を、r以上については要素数*rを加算
	auto calc = [](vector<ll> &arr, vector<ll> &arr_sum, ll r) -> ll
	{
		ll l_sum, r_sum;
		ll tmp = lower_bound(arr.begin(), arr.end(), r) - arr.begin();
		l_sum = arr_sum[tmp];
		r_sum = ((int)arr.size() - tmp) * r;
		return l_sum + r_sum;
	};
	for(i = 0; i < N; i++)  // a[i]について、それより手前の要素とのペアを考える
	{
		ll r = N - i;  // 自身含め、a[i]の右側に要素が何個あるか

		ans -= calc(idx[a[i]], idx_sum[a[i]], r);
		idx[a[i]].push_back(i+1);
		idx_sum[a[i]].push_back(idx_sum[a[i]].back() + i+1);

		ans += calc(tot, tot_sum, r);
		tot.push_back(i+1);
		tot_sum.push_back(tot_sum.back() + i+1);

	}
	cout << ans << endl;

	return 0;
}

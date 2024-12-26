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

// ABC364 https://atcoder.jp/contests/abc364

/*
 * 答で二分探索。ダブル二分探索で解く。 
 * 
 * 距離x以内に点Aがk個以上あるか？ の判定問題を考える。
 * これはxを大きくしていくと判定結果が false,false,...,false,true,true,... とf/tが切り替わるので、二分探索で解ける。
 * 判定問題自体も二分探索にて。実装を参照。
 * 
 * [ACまでの思考の流れ]
 * ・k番目の要素を答える、は「条件xのときにk個以上か？」の二分探索を考えてみる。[典型]
 */

// ソート済のvector<T>a に対して、(未満,以下,より大きい,以上)となるidxと要素数を返す
// {val以下の最右のidx, val以下の要素数} 要素が無ければidx=-1
template<typename T> pair<ll,ll> bi_idxnum_ika(vector<T> &a, T val)   {ll idx=upper_bound(a.begin(),a.end(),val)-a.begin(); return{idx-1,idx};}
// {val以上の最左のidx, val以上の要素数} 要素が無ければidx=N
template<typename T> pair<ll,ll> bi_idxnum_ijou(vector<T> &a, T val)  {ll idx=lower_bound(a.begin(),a.end(),val)-a.begin(); return{idx,(ll)a.size()-idx};}

int main(void)
{
	int i;
	int N, Q; cin >> N >> Q;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	sort(a.begin(), a.end());

	// posから距離x以下の点Aの個数が、k個以上ならtrue
	auto check = [&](int pos, int k, int x) -> bool
	{
		auto[idx1, dmy1] = bi_idxnum_ijou(a, pos-x);
		auto[idx2, dmy2] = bi_idxnum_ika(a, pos+x);
		return (idx2-idx1+1) >= k;
	};
	for(int _ = 0; _ < Q; _++)
	{
		int b, k; cin >> b >> k;
		int ng = -1;  // 距離0が答となるケースがあるため
		int ok = INF32;
		while(ok - ng > 1)
		{
			int mid = (ng+ok)/2;
			if(check(b, k, mid)) ok = mid;
			else ng = mid;
		}
		cout << ok << endl;
	}
	return 0;
}

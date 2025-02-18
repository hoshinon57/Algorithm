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

// ABC124 https://atcoder.jp/contests/abc124

/*
 * ランレングス圧縮と尺取り法で解いた。
 * 
 * 連続した0をまとめて1にするのが最適だろうな、と思った。
 * 丁寧な説明はけんちょん氏の記事が詳しかった。
 *   https://drken1215.hatenablog.com/entry/2019/04/14/222900
 * 
 * ランレングス圧縮して、0のブロックを順にK個ひっくり返したときの、
 * 前後の(元からある)1のブロックも含めた、1の連続する数を考える。
 * これをひっくり返す場所を変えながら計算し、その最大値が答。
 * 
 * ここからの実装詰めに苦戦した。
 * 累積和が良いかなーと思ったが、先頭が0か1かで場合分けが面倒そう。
 * 結局、尺取り法で実装した。詳細はソースコードを参照。
**/

// 文字列strをランレングス圧縮して {文字,長さ} のpairの列挙で返す
// [補足]配列に対して扱いたい場合、引数をstring->vectorに変えるだけで良い
//       "長さ"は呼び出し元で乗算する可能性があるため、オーバーフロー防止でll型としている (ABC369-C)
// auto enc = rle<char>(s); といった形で呼び出す
// for(auto [c,len] : enc) が使い方の一例
// [参考]https://algo-logic.info/run-length/  [verify]ABC019-B,ABC380-B
template <typename T> vector<pair<T,ll>> rle(string &str) {
	vector<pair<T,ll>> ret;
	int n = str.size();
	for(int l = 0, r = 0; l < n; ) {  // 尺取り法っぽく [l,r)が条件を満たす
		while(r < n && str[l] == str[r]) r++;
		ret.push_back({str[l], r-l});
		l = r;
	}
	return ret;
}

int main(void)
{
	int N, K; cin >> N >> K;
	string s; cin >> s;
	auto enc = rle<char>(s);
	int sz = (int)enc.size();
	int ans = 0;
	int sum = 0;  // 現時点の1が連続する数
	int cnt = 0;  // 現時点の操作回数

	int l, r;
	r = 0;  // [l,r)
	for(l = 0; l < sz; l++)
	{
		// 次に見ようとするブロックが、1なら無条件で採用可能、
		// 0なら操作回数がK未満なら採用可能。
		while(r < sz && (enc[r].first == '1' || (enc[r].first == '0' && cnt < K)) )
		{
			sum += enc[r].second;
			if(enc[r].first == '0') cnt++;
			r++;
		}
		chmax(ans, sum);

		sum -= enc[l].second;
		if(enc[l].first == '0') cnt--;
	}
	cout << ans << endl;

	return 0;
}

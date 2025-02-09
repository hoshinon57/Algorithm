#include <iostream>
#include <vector>
#include <algorithm>
#include <random>  // rng_64
#include <chrono>  // rng_64
#include <fstream>  // cin差し替え
#include <sstream>  // cout差し替え
#include <filesystem> // C++17以降
#include <map>
using namespace std;
typedef long long ll;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

/*
 * 愚直解を書くテクニック
 * 
 * やり方：
 * ・main()の"ランダムデータ生成"部分を書き換えて、入力用データを生成する。
 *   rng_64, rnd_out, rnd_vecなどを活用する。
 * ・WAになる原因を探したいコードを、solve()に移動させる。
 * ・naive()にて愚直解を書く。
 * ・naive.exeを実行する。不一致になるケースがあれば wa1.txt, wa2.txt,... へ順に出力してくれる。
 * 
 * 参考：
 * https://betrue12.hateblo.jp/entry/2019/09/07/171628
 */

// [lo,hi]の区間の乱数を返す(閉区間)
// 必要なincludeは以下2つ
//   #include <random>
//   #include <chrono>
// 型はlong longで決め打ちしている。(mt19937_64を使っており、生成される値は64bit)
long long rng_64(long long lo, long long hi) {
	// static std::random_device rd;  // random_deviceを使うのは避けた方が良い環境があるらしい
	// static std::mt19937_64 rng(rd());
	static auto seed = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
	static std::mt19937_64 rng(seed);
	return std::uniform_int_distribution<long long>(lo, hi)(rng);  // [lo,hi]の範囲での乱数を生成
}

// 可変長引数にて、値argsたちをoに出力
// args間は半角スペース、最後に改行を入れる
// 例)rnd_out(o, H, W, N);  // 出力は"H W N"となる。最後に改行
template<typename... Args>
void rnd_out(ofstream &o, Args... args) {
	int len = (int)sizeof...(args);
	int a[] = {args...};
	for(int i = 0; i < len; i++) {
		o << a[i];
		if(i != len-1) o << " ";
	}
	o << endl;
}

// 要素N個、値の区間[lo,hi]でランダム生成してoに出力
// diff=trueなら重複無し (区間数よりNの方が大きい場合、無限ループになるため注意)
void rnd_vec(ofstream &o, int N, ll lo, ll hi, bool diff = false) {
	map<ll,bool> exist;
	for(int i = 0; i < N; i++) {
		while(true) {
			ll val = rng_64(lo, hi);
			if(diff && exist.count(val) > 0) continue;
			exist[val] = true;
			o << val;
			break;
		}
		if(i != N-1) o << " ";
	}
	o << endl;
}

void solve(void)
{
	int i;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	*max_element(a.begin(), a.end()) = -100;
	if(N==10) a.push_back(INF32);
	cout << max_element(a.begin(), a.end()) - a.begin() + 1 << endl;
}

void naive(void)
{
	int i;
	int N; cin >> N;
	vector<pair<int,int>> a(N);
	for(i = 0; i < N; i++)
	{
		cin >> a[i].first;
		a[i].second = i+1;
	}
	sort(a.begin(), a.end());
	reverse(a.begin(), a.end());
	cout << a[1].second << endl;
}

int main(void)
{
	streambuf* org_buf = std::cout.rdbuf();
	const string inp = "input.txt";
	const int loop = 1000;
	for(int cnt = 0; cnt < loop; cnt++)
	{
		// ランダムデータ生成
		ofstream o(inp);
		int H=6, W=6, N=4;
		rnd_out(o, H, W, N);
		for(int i = 0; i < N; i++)
		{
			int r = i+1;
			int c = rng_64(1, W);
			int l = rng_64(1, W);
			chmin(l, W-c+1);
			rnd_out(o, r, c, l);
		}
		o.close();

		ifstream in;
		ostringstream oss;
		string s1, s2;
		for(int i = 0; i < 2; i++)
		{
			in.open(inp);
			cin.rdbuf(in.rdbuf());
			cout.rdbuf(oss.rdbuf());  // coutの出力先を変更
			if(i == 0) { solve(); s1 = oss.str(); }
			else       { naive(); s2 = oss.str(); }
			oss.str("");  // これでバッファが空になるらしい
			oss.clear();
			in.close();
		}

		if(s1 != s2)
		{
			string ren;
			for(int i = 0; i < 10; i++)  // 最大10ファイルまで、リネーム先が被らないよう"簡易"チェック
			{
				ren = "wa" + to_string(i) + ".txt";
				if(filesystem::exists(ren)) continue;
				std::rename(inp.c_str(), ren.c_str());
				break;
			}
			cout.rdbuf(org_buf);
			cout << "find WA! -> " << ren << endl;
			ofstream file(ren, std::ios::app);  // 追記モードにて、エラーとなった出力を入力ファイルへ書き出し
			file << endl << "solve:" << endl << s1 << endl << "naive:" << endl << s2 << endl;
			break;
		}
	}

	return 0;
}

// 愚直解サンプル
void naive_ABC336_D(void)
{
	int i, j;
	int N; cin >> N;
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i];}
	int ans = 0;
	for(i = 0; i < N; i++)
	{
		for(j = 1; j <= a[i]; j++)  // iを中央に、高さjのピラミッドを生成
		{
			// 左側
			int h = j;
			int k = i;
			bool ok = true;
			while(k >= 0)
			{
				if(k == 0 && h > 1)
				{
					ok = false;
					break;
				}
				if(a[k] < h)
				{
					ok = false;
					break;
				}
				k--;
				h--;
			}
			// 右側
			h = j;
			k = i;
			while(k < N)
			{
				if(k == N-1 && h > 1)
				{
					ok = false;
					break;
				}
				if(a[k] < h)
				{
					ok = false;
					break;
				}
				k++;
				h--;
			}
			if(ok) chmax(ans, j);
		}
	}
	cout << ans << endl;
}

// 愚直解サンプル
void naive_ABC382_F(void)
{
	// 0-indexed
	int i, j;
	int H, W, N; cin >> H >> W >> N;
	vector<vector<int>> mp(H, vector<int>(W, 0));  // mp[H][W]  ブロックがあれば1
	vector<int> r(N), c(N), l(N); for(i = 0; i < N; i++) {cin >> r[i] >> c[i] >> l[i]; r[i]--; c[i]--;}
	for(i = 0; i < N; i++)
	{
		for(j = c[i]; j < c[i]+l[i]; j++)
		{
			mp[r[i]][j] = 1;
		}
	}
	while(true)
	{
		bool chg = false;
		for(i = 0; i < N; i++)
		{
			bool moveok = true;
			if(r[i] == H-1) continue;  // 最下部
			for(j = c[i]; j < c[i]+l[i]; j++)
			{
				if(mp[r[i]+1][j] == 1) moveok = false;
			}
			if(moveok)
			{
				for(j = c[i]; j < c[i]+l[i]; j++)
				{
					mp[r[i]][j] = 0;
					mp[r[i]+1][j] = 1;
				}
				r[i]++;
				chg = true;
			}
		}
		if(!chg) break;
	}
	for(auto &e : r) cout << e+1 << endl;
}

// 愚直解サンプル
void naive_ABC373_E(void)
{
	int i, j, k;
	int N, M, K; cin >> N >> M >> K;
	int kzan = K;  // 残票
	vector<int> a(N); for(i = 0; i < N; i++) {cin >> a[i]; kzan -= a[i];}
	vector<int> ans;
	for(i = 0; i < N; i++)
	{
		auto a_bk = a;
		for(j = 0; j <= kzan; j++)  // 人iに追加j票でどうか
		{
			int cnt = 0;
			int x = a[i]+j;  // 人iはx票になった
			int zan = kzan-j;
			a[i] = INF32;
			sort(a.begin(), a.end());
			reverse(a.begin(), a.end());  // 降順
			for(k = 1; k < N; k++)
			{
				if(a[k] <= x)
				{
					int use = min(zan, x-a[k]+1);
					zan -=use;
					a[k] += use;
				}
				if(a[k] > x) cnt++;
			}
			a = a_bk;
			if(cnt < M) break;
		}
		if(j == kzan+1) ans.push_back(-1);
		else ans.push_back(j);
		a = a_bk;
	}

	int sz_ = (int)ans.size();
	for(i = 0; i < sz_; i++) {
		cout << ans[i];
		if(i != sz_-1) cout << " ";
	}
	cout << endl;
}

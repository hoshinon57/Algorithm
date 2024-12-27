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
 *   rng_64, rnd_1, rnd_vecなどを活用する。
 * ・WAになる原因を探したいコードを、solve()に移動させる。
 * ・naive()にて愚直解を書く。
 * ・naive.exeを実行する。不一致になるケースがあれば wa1.txt, wa2.txt,... へ順に出力してくれる。
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

// 値valをoに出力
void rnd_1(ofstream &o, int val) {
	o << val <<endl;
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
		int N = rng_64(2, 10);
		rnd_1(o, N);
		rnd_vec(o, N, 1, 10, true);
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
			string tmp;
			for(int i = 0; i < 10; i++)  // 最大10ファイルまで、リネーム先が被らないよう"簡易"チェック
			{
				tmp = "wa" + to_string(i) + ".txt";
				if(filesystem::exists(tmp)) continue;
				std::rename(inp.c_str(), tmp.c_str());
				break;
			}
			cout.rdbuf(org_buf);
			cout << "find WA! -> " << tmp << endl;
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

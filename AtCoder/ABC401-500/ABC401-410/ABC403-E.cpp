#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <cassert>
using namespace std;
typedef long long ll;
// const ll INF64 = 1LL << 60;
const ll INF64 = ((1LL<<62)-(1LL<<31));  // 10^18より大きく、かつ2倍しても負にならない数
const int INF32 = 0x3FFFFFFF;  // =(2^30)-1 10^9より大きく、かつ2倍しても負にならない数
template<class T> inline bool chmin(T &a, T b) { if(a > b) { a = b; return true; } return false; }
template<class T> inline bool chmax(T &a, T b) { if(a < b) { a = b; return true; } return false; }
#define YesNo(T) cout << ((T) ? "Yes" : "No") << endl;  // T:bool

// ABC403 https://atcoder.jp/contests/abc403

/*
 * Trie木＆いもす法で解いた。
 * ※公式解説では同等の方針が無かったが、Twitterで"ABC403 imos"で検索するといくつか出てきた。
 * 
 * クエリ先読みしておく。
 * その後T=1のクエリについて、クエリ番号と合わせてTrie木に登録する。
 * するとT=2の各クエリについて、その文字列がいつまでX内に接頭辞が無いか、
 * ざっくり言い換えると「いつまで生きているか」が分かる。
 * 
 * 登場/消滅のタイミングが分かるので、その区間はいもす法。
**/

// https://algo-logic.info/trie-tree/
// Trie<26, 'A'> trie; のように定義する
// (1)insert(word) : 文字列wordをトライ木に追加する
// (2)search(word, prefix=true/false) : 文字列がトライ木に追加されているか検索
// (3)word_count() : 追加した単語数を返す
// (4)size() : 頂点の総数
template <int char_size, int base>  // 文字の種類と、0に対応する文字
struct Trie {
private:
	struct Node {
		vector<int> next;  // next[ch]:文字chである子の頂点番号 存在しなければ-1
		vector<int> accept;  // 末端がこの頂点である文字列のidたち
		int c;  // この頂点が持つ文字を、base基点で数値で表したもの 'A'なら0, 'B'なら1, など
		int depth;  // 根からの距離
		int common;  // この頂点を共有している文字列の個数
		Node(int c_, int dep_) : c(c_), depth(dep_), common(0) {
			next.assign(char_size, -1);
		}
	};
	vector<Node> nodes;  // トライ木本体
	int root;  // 根 (コンストラクタでしか使わないが、明示的に)

	// 単語wordをid番目として追加
	void insert(const string &word, int id)	{
		int node_id = 0;  // node_id=0は根(文字が入っていない)であることに注意
		for(int i = 0; i < (int)word.size(); i++) {
			// 以下、node_idから文字cへたどった先がnext_idになる
			int c = (int)(word[i] - base);
			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) {  // 文字cが木に存在しなければ追加
				next_id = (int)nodes.size();
				nodes.push_back(Node(c, nodes[node_id].depth+1));
			}
			nodes[node_id].common++;
			node_id = next_id;
		}
		nodes[node_id].common++;
		nodes[node_id].accept.push_back(id);
	}

public:
	Trie() : root(0) {
		nodes.push_back(Node(root, 0));  // 始めは根のみ
	}
	// wordをトライ木に追加
	void insert(const string &word) {
		insert(word, nodes[0].common);
	}
	// ABC403-E用
	void insert2(const string &word, int id) {
		insert(word, id);
	}
	// wordの探索
	// prefix=false : wordがTrie木に存在すればtrue
	// prefix=true : wordそのものでなくとも、wordを接頭辞(prefix)として持つ単語が存在すればtrue
	bool search(const string &word, int &ed, bool prefix = false) {
		int node_id = 0;
		for(int i = 0; i < (int)word.size(); i++) {
			// for ABC403-E
			auto &ac = nodes[node_id].accept;
			if(ac.size() > 0)  // そこで終わる単語があれば
			{
				chmin(ed, ac[0]);
			}

			int c = (int)(word[i] - base);
			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) {
				return false;  // 次の頂点が存在しない
			}
			node_id = next_id;
		}

		// for ABC403-E
		auto &ac = nodes[node_id].accept;
		if(ac.size() > 0)  // そこで終わる単語があれば
		{
			chmin(ed, ac[0]);
		}

		if(prefix) {
			return true;  // 接頭辞であれば、ここまで来た時点で確実に存在する
		}
		else {
			return nodes[node_id].accept.size() > 0;  // 検索終端が末端となるような頂点があるか
		}
	}
	// 追加した単語数
	int word_count() const {
		return nodes[0].common;
	}
	// 頂点の総数 (根を含むので文字数+1になる)
	int size(void) const {
		return (int)nodes.size();
	}
};

int main(void)
{
	int i;
	int Q; cin >> Q;
	Trie<26, 'a'> trie;
	using dt = pair<int,string>;
	vector<dt> d(Q);
	for(i = 0; i < Q; i++) cin >> d[i].first >> d[i].second;

	for(i = 0; i < Q; i++)
	{
		// クエリ番号と合わせて登録
		if(d[i].first == 1) trie.insert2(d[i].second, i);
	}

	vector<int> imos(Q+5);
	for(i = 0; i < Q; i++)
	{
		if(d[i].first == 2)
		{
			int st = i;
			int ed = INF32;
			trie.search(d[i].second, ed);
			if(st > ed) continue;  // 最初からダメ
			if(ed == INF32) ed = Q;  // 最後まで生き残る
			imos[st]++;
			imos[ed]--;
		}
	}
	for(i = 0; i < Q; i++)
	{
		if(i != 0) imos[i] += imos[i-1];
		cout << imos[i] << endl;
	}

	return 0;
}

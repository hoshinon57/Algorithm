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

// ABC287 https://atcoder.jp/contests/abc287

// Trie木のライブラリを改造して解く版。
// Trie木にSを登録していった後、
// 各Sに対して先頭から見ていき、「2単語以上で使われている頂点のうち、最も遠いもの」が答となる。

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
		vector<int> accept;  // 末端がこの頂点である文字列の頂点番号たち
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
		int node_id = 0;
		for(int i = 0; i < (int)word.size(); i++) {
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
	// wordの探索
	// prefix=false : wordがTrie木に存在すればtrue
	// prefix=true : wordそのものでなくとも、wordを接頭辞(prefix)として持つ単語が存在すればtrue
	bool search(const string &word, bool prefix = false) {
		int node_id = 0;
		for(int i = 0; i < (int)word.size(); i++) {
			int c = (int)(word[i] - base);
			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) {
				return false;  // 次の頂点が存在しない
			}
			node_id = next_id;
		}
		if(prefix) {
			return true;  // 接頭辞であれば、ここまで来た時点で確実に存在する
		}
		else {
			return nodes[node_id].accept.size() > 0;  // 検索終端が末端となるような頂点があるか
		}
	}

	int query(const string &word) {
		int node_id = 0;
		for(int i = 0; i < (int)word.size(); i++) {
			int c = (int)(word[i] - base);
			int &next_id = nodes[node_id].next[c];
			if(next_id == -1) break;  // 次の頂点が存在しない
			if(nodes[next_id].common < 2) break;  // 次の頂点を使っているのが1単語のみ
			node_id = next_id;
		}
		// nodes[node_id]は2単語以上で使われている頂点のうち、最も根から遠いもの
		return nodes[node_id].depth;
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
	int N; cin >> N;
	Trie<26, 'a'> trie;
	vector<string> s(N);
	for(i = 0; i < N; i++) {
		cin >> s[i];
		trie.insert(s[i]);
	}

	for(i = 0; i < N; i++)
	{
		int ans = trie.query(s[i]);
		cout << ans << endl;
	}

	return 0;
}

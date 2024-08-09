#include "Trie.h"

void Trie::insert(const std::string &key, const std::vector<int> &values)
{
    TrieNode *node = root;
    for (char c : key)
    {
        if (!node->children[c])
        {
            node->children[c] = new TrieNode();
        }
        node = node->children[c];
    }
    node->values = values; // 存储值
}

std::vector<int> Trie::findPrefix(const std::string &prefix)
{
    TrieNode *node = root;
    for (char c : prefix)
    {
        if (!node->children[c])
        {
            return {}; // 没有找到前缀
        }
        node = node->children[c];
    }
    return node->values; // 返回与前缀相关的值
}

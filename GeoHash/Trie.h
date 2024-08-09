#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

struct TrieNode
{
    std::unordered_map<char, TrieNode *> children;
    std::vector<int> values; // 存储与该前缀相关的值
};

// 前缀树，在 O(k) 的时间内查找前缀，其中 k 是查找的字符串的长度
class Trie
{
public:
    Trie() : root(new TrieNode()) {}
    void insert(const std::string &key, const std::vector<int> &values); // 插入
    std::vector<int> findPrefix(const std::string &prefix);              // 查询

private:
    TrieNode *root;
};
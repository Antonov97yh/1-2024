#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <queue>
#include <bitset>
using namespace std;
using deck = bitset<68>;
using heap = bitset<544>;
int Top(const heap& d, int i) {
    return d[68 * i] + d[68 * i + 1] * 2 + d[68 * i + 2] * 4 + d[68 * i + 3] * 8;
}
heap full;
vector<heap> Move(const heap& d) {
    vector<heap> ans;
    for (int i = 0; i < 8; ++i) {
        if (((d >> (68 * i)) & full) == full) {
            ans.push_back(d ^ (full << (68 * i)));
            return ans;
        }
    }
    heap help;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if ((Top(d, i) < Top(d, j) || Top(d, j) == 0) && Top(d, i) != 0) {
                auto block1 = d << (68 * (7 - i)) >> (68 * 7);
                auto block2 = d << (68 * (7 - j)) >> (68 * 7);
                help = d ^ (block1 << 68 * i) ^ (block2 << 68 * j);
                block2 <<= 4;
                block2[0] = block1[0];
                block2[1] = block1[1];
                block2[2] = block1[2];
                block2[3] = block1[3];
                block1 >>= 4;
                help |= (block1 << 68 * i) | (block2 << 68 * j);
                ans.push_back(help);
            }
        }
    }
    return ans;
}

int main() {
    for (size_t i = 9; i > 0; --i) {
        full <<= 4;
        full[0] = i & 1;
        full[1] = i & 2;
        full[2] = i & 4;
        full[3] = i & 8;
    }
    vector<int> count(10);
    unordered_map<string, int> change = {{"6", 1}, {"7", 2}, {"8", 3}, {"9", 4}, {"10", 5}, {"J", 6}, {"Q", 7}, {"K", 8}, {"A", 9}};
    heap have;
    string c;
    int help;
    for (size_t j = 0; j < 8; ++j) {
        for (size_t i = 0; i < 9; ++i) {
            std::cin >> c;
            if (change.find(c) == change.end()) {
                std::cout << "this is not card\n";
                return 0;
            }
            help = change[c];
            ++count[help];
            have[68 * j + 32 - 4 * i] = help & 1;
            have[68 * j + 32 - 4 * i + 1] = help & 2;
            have[68 * j + 32 - 4 * i + 2] = help & 4;
            have[68 * j + 32 - 4 * i + 3] = help & 8;
        }
    }
    for (int i = 1; i < 10; ++i) {
        if (count[i] != 8) {
            std::cout << "these are not two decks\n";
            return 0;
        }
    }
    unordered_map<heap, int> res;
    queue<heap> q;
    res[have] = 0;
    q.push(have);
    while (!q.empty()) {
        auto look = q.front();
        q.pop();
        for (const auto& w : Move(look)) {
            if (res.find(w) == res.end()) {
                res[w] = res[look] + 1;
                q.push(w);
            }
        }
    }
    if (res.find(heap()) != res.end()) {
        std::cout << "It can be solved in " << res[heap()] << " steps\n";
    } else {
        std::cout << "It can not be solved\n";
    }
    return 0;
}

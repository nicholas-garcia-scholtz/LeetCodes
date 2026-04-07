/*
Given a string s, rearrange the characters of s so that any two adjacent characters are not the same.

Return any possible rearrangement of s or return "" if not possible.

 

Example 1:

Input: s = "aab"
Output: "aba"
Example 2:

Input: s = "aaab"
Output: ""
 

Constraints:

1 <= s.length <= 500
s consists of lowercase English letters.
*/


#include <utility>
#include <string>
#include <vector>
#include <queue>

using std::vector;
using std::string;

class Solution {
public:
    string reorganizeString(string s) {
        vector<int> buckets(26, 0);

        for (const char c : s) ++buckets[c - 'a'];

        std::priority_queue<std::pair<int, int>> pq;
        for (int i = 0; i < 26; ++i) {
            if (buckets[i]) pq.emplace(buckets[i], i);
        }

        string ans;
        ans.reserve(s.size());
        int prev = -1;


        while (!pq.empty()) {
            std::pair<int, int> pr = pq.top();
            pq.pop();
            ans.push_back('a' + pr.second);
            --buckets[pr.second];
            if (prev != -1 && buckets[prev]) pq.emplace(buckets[prev], prev);
            prev = pr.second;
        }

        return s.size() == ans.size() ? ans : "";
    }
};
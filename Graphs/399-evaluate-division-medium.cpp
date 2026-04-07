/*
You are given an array of variable pairs equations and an array of real numbers values, where equations[i] = [Ai, Bi] and values[i] represent the equation Ai / Bi = values[i]. Each Ai or Bi is a string that represents a single variable.

You are also given some queries, where queries[j] = [Cj, Dj] represents the jth query where you must find the answer for Cj / Dj = ?.

Return the answers to all queries. If a single answer cannot be determined, return -1.0.

Note: The input is always valid. You may assume that evaluating the queries will not result in division by zero and that there is no contradiction.

Note: The variables that do not occur in the list of equations are undefined, so the answer cannot be determined for them.

 

Example 1:

Input: equations = [["a","b"],["b","c"]], values = [2.0,3.0], queries = [["a","c"],["b","a"],["a","e"],["a","a"],["x","x"]]
Output: [6.00000,0.50000,-1.00000,1.00000,-1.00000]
Explanation: 
Given: a / b = 2.0, b / c = 3.0
queries are: a / c = ?, b / a = ?, a / e = ?, a / a = ?, x / x = ? 
return: [6.0, 0.5, -1.0, 1.0, -1.0 ]
note: x is undefined => -1.0
Example 2:

Input: equations = [["a","b"],["b","c"],["bc","cd"]], values = [1.5,2.5,5.0], queries = [["a","c"],["c","b"],["bc","cd"],["cd","bc"]]
Output: [3.75000,0.40000,5.00000,0.20000]
Example 3:

Input: equations = [["a","b"]], values = [0.5], queries = [["a","b"],["b","a"],["a","c"],["x","y"]]
Output: [0.50000,2.00000,-1.00000,-1.00000]
*/

#include <unordered_map>
#include <string>

using std::vector;
using std::string;

class Solution {
private:
    // Node, (Parent, distance)
    std::unordered_map<string, std::pair<string, double>> parents;
    std::unordered_map<string, int> size;
public:
    vector<double> calcEquation(vector<vector<string>>& equations, vector<double>& values, vector<vector<string>>& queries) {
        int i = 0;
        vector<double> ans;
        ans.reserve(queries.size());
        for (vector<string>& equation : equations) {
            if (parents.find(equation[0]) == parents.end()) {
                parents[equation[0]] = std::pair<string, double>{equation[0], 1.0};
                size[equation[0]] = 1;
            }
            if (parents.find(equation[1]) == parents.end()) {
                parents[equation[1]] = std::pair<string, double>{equation[1], 1.0};
                size[equation[1]] = 1;
            }

            unite(equation[0], equation[1], values[i]);
            ++i;
        }

        for (vector<string>& query : queries) {
            if (parents.find(query[0]) == parents.end() || parents.find(query[1]) == parents.end()) {
                ans.emplace_back(-1.0);
                continue;
            }
            std::pair<string, double>& a = find(query[0]);
            std::pair<string, double>& b = find(query[1]);

            if (a.first != b.first) {
                ans.emplace_back(-1.0);
                continue;
            }

            ans.emplace_back(a.second / b.second);
        }

        return ans;
    }

    std::pair<string, double>& find(string& node) {
        auto it = parents.find(node);
        if (it->second.first != node) {
            // Path compression
            std::pair<string, double>& parent = find(it->second.first);
            it->second.first = parent.first;
            it->second.second *= parent.second;
        }

        return it->second;
    }

    void unite(string& a, string& b, double value) {
        auto& rA = find(a);
        auto& rB = find(b);

        if (rA.first == rB.first) return;

        if (size[rA.first] > size[rB.first]) {
            // Attach root B under root A
            double ratio = rA.second / (value * rB.second);
            parents[rB.first] = {rA.first, ratio};
            size[rA.first] += size[rB.first];
        } else {
            // Attach root A under root B
            double ratio = value * rB.second / rA.second; 
            parents[rA.first] = {rB.first, ratio};
            size[rB.first] += size[rA.first];
        }
    }
};
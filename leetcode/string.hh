#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::max;
using std::min;
using std::queue;
using std::stack;
using std::string;
using std::unordered_map;
using std::vector;

/**
 * https://leetcode.com/problems/longest-substring-without-repeating-characters/
 */
int lengthOfLongestSubstring(string s) {
    vector<int> index(256, -1);
    int i = 0, res = 0, start = -1;
    for (auto c : s) {
        start = std::max(start, index[c]);
        res = std::max(res, i - start);
        index[c] = i++;
    }
    return res;
}

int expand(const string &s, int i, int j) {
    int k = 0;
    while (--i >= 0 && j++ < s.length() && s[i] == s[j]) ++k;
    return k;
}

/**
 * https://leetcode.com/problems/longest-palindromic-substring/
 */
string longestPalindrome(string s) {
    int begin = 0, len = 0;
    for (int i = 0; i < s.length(); i++) {
        int ex1 = expand(s, i, i), ex2 = expand(s, i + 1, i);
        if (2 * ex1 + 1 > len) begin = i - ex1, len = 2 * ex1 + 1;
        if (2 * ex2 > len) begin = i + 1 - ex2, len = 2 * ex2;
    }
    return s.substr(begin, len);
}

/**
 * https://leetcode.com/problems/zigzag-conversion/
 */
string convert(string s, int numRows) {
    if (numRows < 2) return s;
    vector<string> rows(numRows, "");
    int r = 0, step = 1;
    for (auto c : s) {
        rows[r] += c;
        if (r == 0) step = 1;
        if (r == numRows - 1) step = -1;
        r += step;
    }
    string res;
    for (auto &str : rows) res += str;
    return res;
}

/**
 * https://leetcode.com/problems/string-to-integer-atoi/
 */
int myAtoi(string str) {
    int i = 0, n = str.length(), flag = 1;
    while (i < n && str[i] == ' ') i++;
    if (i == n) return 0;
    if (str[i] == '+' || str[i] == '-') flag = str[i++] == '+' ? 1 : -1;

    long x = 0;
    while (i < n && isdigit(str[i])) {
        x = x * 10 + str[i++] - '0';
        if (x * flag > INT32_MAX) return INT32_MAX;
        if (x * flag < INT32_MIN) return INT32_MIN;
    }
    return flag * x;
}

bool isMatch(const string &s, const string &p, int i, int j) {
    for (bool flag; j < p.length(); i++) {
        flag = j + 1 < p.length() && '*' == p[j + 1];
        if (flag && isMatch(s, p, i, j + 2)) return true;
        if (i >= s.length() || (s[i] != p[j] && p[j] != '.')) return false;
        if (!flag) j++;
    }
    return i >= s.length();
}
/**
 * https://leetcode.com/problems/regular-expression-matching/
 * '.' Matches any single character.
 * '*' Matches zero or more of the preceding element.
 */
bool isMatch(string s, string p) { return isMatch(s, p, 0, 0); }

/**
 * https://leetcode.com/problems/wildcard-matching/
 * '?' Matches any single character.
 * '*' Matches any sequence of characters (including the empty sequence).
 */
bool isMatch2(string s, string p) {
    int star = -1, i = 0, i2 = 0, j = 0;
    while (i < s.length()) {
        if (j < p.length() && (s[i] == p[j] || p[j] == '?')) {
            i++, j++;
            continue;
        }

        if (j < p.length() && p[j] == '*') {
            star = j++, i2 = i;
            continue;
        }

        if (star > -1) {
            j = star + 1, i = ++i2;
            continue;
        }

        return false;
    }

    while (j < p.length() && p[j] == '*') j++;
    return j == p.length();
}

string intToRoman(int n, char c10, char c5, char c1) {
    if (n >= 10 || n <= 0) return "";
    if (n == 9) return string("") + c10 + c1;
    if (n >= 5) return c5 + string(n - 5, c1);
    if (n == 4) return string("") + c1 + c5;
    return string(n, c1);
}

/**
 * https://leetcode.com/problems/integer-to-roman/
 */
string intToRoman(int num) {
    string res = string((num % 10000) / 1000, 'M');
    res += intToRoman((num % 1000) / 100, 'M', 'D', 'C');
    res += intToRoman((num % 100) / 10, 'C', 'L', 'X');
    res += intToRoman((num % 10), 'X', 'V', 'I');
    return res;
}

/**
 * https://leetcode.com/problems/roman-to-integer/
 */
int romanToInt(string s) {
    map<char, int> roman{{'I', 1},   {'V', 5},   {'X', 10},  {'L', 50},
                         {'C', 100}, {'D', 500}, {'M', 1000}};
    int res = 0, i;
    for (i = 0; i < s.length() - 1; i++)
        res += roman[s[i]] * (roman[s[i]] < roman[s[i + 1]] ? -1 : 1);
    res += roman[s[i]];
    return res;
}

/**
 * https://leetcode.com/problems/longest-common-prefix/
 */
string longestCommonPrefix(vector<string> &strs) {
    if (strs.empty()) return "";
    string res;
    for (int i = 0; i < strs[0].size(); i++) {
        for (int j = 1; j < strs.size(); j++)
            if (i >= strs[j].length() || strs[j][i] != strs[0][i]) return res;
        res += strs[0][i];
    }
    return res;
}

void letterCombinations(vector<string> &res, string &combination,
                        const vector<string> &letters, const string &digits,
                        int i) {
    if (i >= digits.length()) {
        res.push_back(combination);
        return;
    }

    for (auto c : letters[digits[i] - '2']) {
        combination.push_back(c);
        letterCombinations(res, combination, letters, digits, i + 1);
        combination.pop_back();
    }
}

/**
 * https://leetcode.com/problems/letter-combinations-of-a-phone-number/
 */
vector<string> letterCombinations(string digits) {
    const vector<string> letters = {"abc", "def",  "ghi", "jkl",
                                    "mno", "pqrs", "tuv", "wxyz"};
    if (digits.empty()) return {};
    vector<string> res;
    string combination;
    letterCombinations(res, combination, letters, digits, 0);
    return res;
}

/**
 * https://leetcode.com/problems/valid-parentheses/
 */
bool isValid(string s) {
    stack<char> stk;
    for (auto c : s)
        if (!stk.empty() && c != stk.top() && abs(c - stk.top()) <= 2)
            stk.pop();
        else
            stk.push(c);
    return stk.empty();
}

void generateParenthesis(vector<string> &res, string &combination, int n,
                         int m) {
    if (n == 0 && m == 0) {
        res.push_back(combination);
        return;
    }

    if (n > 0) {
        combination.push_back('(');
        generateParenthesis(res, combination, n - 1, m + 1);
        combination.pop_back();
    }
    if (m > 0) {
        combination.push_back(')');
        generateParenthesis(res, combination, n, m - 1);
        combination.pop_back();
    }
}

/**
 * https://leetcode.com/problems/generate-parentheses/
 */
vector<string> generateParenthesis(int n) {
    vector<string> res;
    string combination;
    generateParenthesis(res, combination, n, 0);
    return res;
}

vector<int> getNext(const string &needle) {
    vector<int> next(needle.size(), 0);
    for (int i = 1, k = 0; i < needle.size(); i++) {
        while (k > 0 && needle[k] != needle[i]) k = next[k - 1];
        if (needle[i] == needle[k]) k++;
        next[i] = k;
    }
    return next;
}

/**
 * https://leetcode.com/problems/implement-strstr/
 * KMP
 * -1 not found, 0 empty
 */
int strStr(string haystack, string needle) {
    if (needle.empty()) return 0;
    auto next = getNext(needle);

    int n = haystack.size(), m = needle.size();
    for (int i = 0, j = 0; i < n && j < m;) {
        while (i < n && j < m && haystack[i] == needle[j]) ++i, ++j;
        if (j == m) return i - m;
        j == 0 ? ++i : j = next[j - 1];
    }
    return -1;
}

/**
 * https://leetcode.com/problems/substring-with-concatenation-of-all-words/
 */
vector<int> findSubstring(string s, vector<string> &words) {
    if (words.empty()) return {};
    int len = words[0].length();
    vector<int> res;

    unordered_map<string, int> count;
    for (auto w : words) count[w]++;

    for (int i = 0; i < len; i++) {
        queue<string> que;
        unordered_map<string, int> cnt;

        for (int j = i; j + len <= s.size(); j += len) {
            auto w = s.substr(j, len);

            if (count.find(w) == count.end()) {
                queue<string>().swap(que);
                unordered_map<string, int>().swap(cnt);
                continue;
            }

            que.push(w);
            cnt[w]++;

            if (que.size() == words.size()) {
                if (cnt == count) res.push_back(j - len * (words.size() - 1));
                cnt[que.front()]--;
                que.pop();
            }
        }
    }
    return res;
}

/**
 * https://leetcode.com/problems/longest-valid-parentheses/
 */
int longestValidParentheses(string s) {
    stack<int> stk;
    int maxlen = 0;
    for (int i = 0; i < s.length(); i++) {
        if (!stk.empty() && s[stk.top()] == '(' && s[i] == ')') {
            stk.pop();
            maxlen = max(maxlen, i - (stk.empty() ? -1 : stk.top()));
        } else
            stk.push(i);
    }
    return maxlen;
}

/**
 * https://leetcode.com/problems/count-and-say/
 */
string countAndSay(int n) {
    string s = "1", tmp;
    while (--n) {
        int i, j;
        for (i = 0, j = 1; j < s.length(); j++) {
            if (s[j] == s[i]) continue;
            tmp += std::to_string(j - i) + s[i];
            i = j;
        }
        s = tmp + std::to_string(j - i) + s[i], tmp = "";
    }
    return s;
}

/**
 * https://leetcode.com/problems/multiply-strings/
 */
string multiply(string num1, string num2) {
    string res(num1.size() + num2.size(), '0');
    int sum, extra;
    for (int i = num1.size() - 1, vi; i >= 0; i--) {
        extra = 0, vi = num1[i] - '0';
        for (int j = num2.size() - 1; j >= 0; j--) {
            sum = vi * (num2[j] - '0') + res[i + j + 1] - '0' + extra;
            res[i + j + 1] = sum % 10 + '0';
            extra = sum / 10;
        }
        res[i] += extra;
    }

    auto zero = res.find_first_not_of("0");
    if (zero != string::npos) return res.substr(zero);
    return "0";
}

/**
 * https://leetcode.com/problems/group-anagrams/
 */
vector<vector<string>> groupAnagrams(vector<string> &strs) {
    unordered_map<string, vector<string>> strMap;
    for (auto &s : strs) {
        auto str = s;
        sort(str.begin(), str.end());
        strMap[str].push_back(s);
    }

    vector<vector<string>> res;
    for (auto &kv : strMap) res.push_back(kv.second);
    return res;
}

/**
 * https://leetcode.com/problems/length-of-last-word/
 */
int lengthOfLastWord(string s) {
    int j = s.length() - 1;
    while (j >= 0 && s[j] == ' ') j--;
    int i = j;
    while (i >= 0 && s[i] != ' ') i--;
    return j - i;
}

/**
 * https://leetcode.com/problems/add-binary/
 */
string addBinary(string a, string b) {
    string res;
    int i = a.length() - 1, j = b.length() - 1, extra = 0, sum;
    while (i >= 0 || j >= 0 || extra > 0) {
        sum = (i >= 0 ? a[i] - '0' : 0) + (j >= 0 ? b[j] - '0' : 0) + extra;
        extra = sum / 2, res += '0' + sum % 2;
        i = i >= 0 ? i - 1 : -1;
        j = j >= 0 ? j - 1 : -1;
    }
    reverse(res.begin(), res.end());
    return res;
}

// [i, j)
string fullJustify(vector<string> &words, int i, int j, int maxWidth,
                   bool lastline = false) {
    string res;
    int n = j - i;

    if (n == 1 || lastline) {
        for (; i < j; i++) {
            res += words[i] + " ";
            maxWidth -= (words[i].length() + 1);
        }
        if (maxWidth > 0) res += string(maxWidth, ' ');
        if (maxWidth == -1) res.pop_back();
        return res;
    }

    for (int k = i; k < j; k++) maxWidth -= words[k].length();
    int base = maxWidth / (n - 1), a = maxWidth - base * (n - 1);

    for (int k = 0; k < a; k++) res += words[i + k] + string(base + 1, ' ');
    for (int k = a; k < n - 1; k++) res += words[i + k] + string(base, ' ');
    res += words[j - 1];

    return res;
}

/**
 * https://leetcode.com/problems/text-justification/
 */
vector<string> fullJustify(vector<string> &words, int maxWidth) {
    if (words.empty()) return {};

    vector<string> res;
    int i = 0, j = 1, count = words[0].size() + 1;
    for (; j < words.size(); j++) {
        if ((count + words[j].size()) > maxWidth) {
            res.push_back(fullJustify(words, i, j, maxWidth));
            i = j, count = words[i].size() + 1;
        } else
            count += (words[j].size() + 1);
    }

    if (j > i) res.push_back(fullJustify(words, i, j, maxWidth, true));
    return res;
}

/**
 * https://leetcode.com/problems/simplify-path/
 */
string simplifyPath(string path) {
    vector<string> stk;
    string res, s;
    std::stringstream ss(path);

    while (getline(ss, s, '/')) {
        if (s == "" or s == ".") continue;
        if (s == ".." and !stk.empty()) stk.pop_back();
        if (s != "..") stk.push_back(s);
    }
    for (auto w : stk) res += "/" + w;
    return res.empty() ? "/" : res;
}

/**
 * https://leetcode.com/problems/decode-ways/
 */
int numDecodings(string s) {
    if (s.empty() or s[0] == '0') return 0;
    vector<int> dp(s.length() + 1, 0);

    dp[0] = 1;
    for (int i = 1; i < s.length(); i++) {
        dp[i] = s[i] == '0' ? 0 : dp[i - 1];
        if (s[i - 1] == '1' || (s[i - 1] == '2' && s[i] < '7'))
            dp[i] += i >= 2 ? dp[i - 2] : 1;
    }
    return dp[s.length() - 1];
}

string decodeString(const string &s, int &i) {
    string res;
    while (i < s.length() && s[i] != ']') {
        if (isalpha(s[i])) {
            res += s[i++];
        } else {
            int n = 0;
            while (isdigit(s[i])) n = n * 10 + s[i++] - '0';

            i++;  // '['
            auto t = decodeString(s, i);
            i++;  // ']'

            while (n--) res += t;
        }
    }
    return res;
}

/**
 * https://leetcode.com/problems/decode-string/
 */
string decodeString(string s) {
    int i = 0;
    return decodeString(s, i);
}
#include <algorithm>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::max;
using std::min;
using std::sort;
using std::stack;
using std::string;
using std::swap;
using std::vector;

template <typename T>
void print(vector<T> &vec) {
    for (auto &v : vec) cout << v << " ";
    cout << endl;
}

template <typename T>
void print(vector<vector<T>> &matrix) {
    for (auto &vec : matrix) print(vec);
}

/**
 * https://leetcode.com/problems/two-sum/
 */
vector<int> twoSum(vector<int> &nums, int target) {
    map<int, int> indexMap;
    for (int i = 0; i < nums.size(); indexMap[target - nums[i]] = i, i++)
        if (indexMap.count(nums[i])) return {indexMap[nums[i]], i};
    return {};
}

/**
 * https://leetcode.com/problems/3sum/
 */
vector<vector<int>> threeSum(vector<int> &nums) {
    vector<vector<int>> res;
    sort(nums.begin(), nums.end());

    int a, b, c, i, j, k, n = nums.size(), sum;
    for (i = 0; i < n && (a = nums[i]) <= 0; i++) {
        if (i > 0 && a == nums[i - 1]) continue;
        j = i + 1, k = n - 1;
        while (j < k) {
            b = nums[j], c = nums[k], sum = a + b + c;
            if (sum == 0) {
                res.push_back({a, b, c});
                while (j < k && nums[j] == b) ++j;
                while (j < k && nums[k] == c) --k;
            } else
                sum < 0 ? ++j : --k;
        }
    }
    return res;
}

/**
 * https://leetcode.com/problems/3sum-closest/
 */
int threeSumClosest(vector<int> &nums, int target) {
    int n = nums.size();
    if (n < 3) return 0;
    sort(nums.begin(), nums.end());
    int res = nums[0] + nums[1] + nums[2], i, j, k, sum;
    for (i = 0; i < n; i++) {
        if (i > 0 && nums[i] == nums[i - 1]) continue;
        j = i + 1, k = n - 1;
        while (j < k) {
            sum = nums[i] + nums[j] + nums[k];
            if (abs(sum - target) < abs(res - target)) res = sum;
            if (sum == target) return target;
            sum < target ? ++j : --k;
        }
    }
    return res;
}

/**
 * https://leetcode.com/problems/4sum/
 */
vector<vector<int>> fourSum(vector<int> &nums, int target) {
    vector<vector<int>> res;
    sort(nums.begin(), nums.end());

    int a, b, c, d, i, j, k, m, n = nums.size(), sum;
    for (i = 0; i < n; i++) {
        a = nums[i];
        if (a * 4 > target) break;
        if (i > 0 && a == nums[i - 1]) continue;
        for (j = i + 1; j < n; j++) {
            b = nums[j];
            if (a + 3 * b > target) break;
            if (j > i + 1 && b == nums[j - 1]) continue;
            k = j + 1, m = n - 1;
            while (k < m) {
                c = nums[k], d = nums[m], sum = a + b + c + d;
                if (sum == target) {
                    res.push_back({a, b, c, d});
                    while (k < m && nums[k] == c) ++k;
                    while (k < m && nums[m] == d) --m;
                } else
                    sum < target ? ++k : --m;
            }
        }
    }
    return res;
}

/**
 * valid k is 1->n
 */
int kth(vector<int> &nums1, vector<int> &nums2, int k) {
    int n1 = nums1.size(), n2 = nums2.size();
    if (k <= 0 || k > n1 + n2) return -1;

    int i1 = 0, i2 = 0, i1next, i2next;
    while (i1 < n1 - 1 && i2 < n2 - 1 && k > 3) {
        i1next = min(i1 + k / 2 - 1, n1 - 1);
        i2next = min(i2 + k / 2 - 1, n2 - 1);
        if (nums1[i1next] < nums2[i2next])
            k -= (i1next - i1), i1 = i1next;
        else
            k -= (i2next - i2), i2 = i2next;
    }

    while (i1 < n1 && i2 < n2 && --k) nums1[i1] < nums2[i2] ? i1++ : i2++;

    if (i1 == n1) return nums2[i2 + k - 1];
    if (i2 == n2) return nums1[i1 + k - 1];
    return nums1[i1] < nums2[i2] ? nums1[i1] : nums2[i2];
}

/**
 * https://leetcode.com/problems/median-of-two-sorted-arrays/
 */
double findMedianSortedArrays(vector<int> &nums1, vector<int> &nums2) {
    int n1 = nums1.size(), n2 = nums2.size(), k = (n1 + n2) / 2;
    int v1 = kth(nums1, nums2, k + 1);
    return (n1 + n2) % 2 == 1 ? v1 : (double)(v1 + kth(nums1, nums2, k)) / 2;
}

/**
 * https://leetcode.com/problems/container-with-most-water/
 */
int maxArea(vector<int> &height) {
    int res = 0, i = 0, j = height.size() - 1, h;
    while (i < j) {
        h = min(height[i], height[j]);
        res = max(res, h * (j - i));
        while (i < j && height[i] <= h) i++;
        while (i < j && height[j] <= h) j--;
    }
    return res;
}

/**
 * https://leetcode.com/problems/remove-duplicates-from-sorted-array/
 */
int removeDuplicates(vector<int> &nums) {
    if (nums.empty()) return 0;
    int i = 0;
    for (auto v : nums)
        if (v != nums[i]) nums[++i] = v;
    return i + 1;
}

/**
 * https://leetcode.com/problems/remove-duplicates-from-sorted-array-ii/
 * at most twice and return the new length.
 */
int removeDuplicates2(vector<int> &nums) {
    if (nums.size() <= 2) return nums.size();
    int i = 1;
    for (int j = i + 1; j < nums.size(); j++)
        if (nums[j] != nums[i - 1]) nums[++i] = nums[j];
    return i + 1;
}

/**
 * https://leetcode.com/problems/remove-element/
 */
int removeElement(vector<int> &nums, int val) {
    int i = -1;
    for (auto v : nums)
        if (v != val) nums[++i] = v;
    return i + 1;
}

/**
 * https://leetcode.com/problems/next-permutation/
 */
void nextPermutation(vector<int> &nums) {
    int n = nums.size(), i, j;
    if (n <= 1) return;

    for (i = n - 2; i >= 0 && nums[i] >= nums[i + 1];) i--;

    if (i >= 0) {
        for (j = n - 1; j > 0 && nums[j] <= nums[i];) j--;
        std::swap(nums[i], nums[j]);
    }
    std::reverse(nums.begin() + i + 1, nums.end());
}

/**
 * https://leetcode.com/problems/search-in-rotated-sorted-array/
 * https://leetcode.com/problems/search-in-rotated-sorted-array-ii/
 */
int search(vector<int> &nums, int target) {
    int i = 0, j = nums.size() - 1, mid, v;
    while (i <= j) {
        mid = (i + j) / 2, v = nums[mid];
        if (v == target) return mid;
        if (v < nums[i])
            target >= v &&target <= nums[j] ? i = mid + 1 : j = mid - 1;
        else if (nums[mid] > nums[i])
            target >= nums[i] &&target <= v ? j = mid - 1 : i = mid + 1;
        else
            i++;
    }
    return -1;
}

int lower_bound(vector<int> &nums, int target) {
    if (nums.empty() || nums.back() < target) return nums.size();
    int i = 0, j = nums.size() - 1, mid;
    while (i < j) mid = (i + j) / 2, nums[mid] < target ? i = mid + 1 : j = mid;
    return i;
}

/**
 * https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/
 */
vector<int> searchRange(vector<int> &nums, int target) {
    int i = lower_bound(nums, target);
    if (i == nums.size() || nums[i] != target) return {-1, -1};
    return {i, lower_bound(nums, target + 1) - 1};
}

/**
 * https://leetcode.com/problems/search-insert-position/
 * no duplicates
 */
int searchInsert(vector<int> &nums, int target) {
    return lower_bound(nums, target);
}

void combinationSum(vector<int> &candidates, vector<vector<int>> &res,
                    vector<int> &combination, int target, int i) {
    if (target == 0) res.push_back(combination);
    if (target <= 0) return;

    for (; i < candidates.size(); i++) {
        combination.push_back(candidates[i]);
        combinationSum(candidates, res, combination, target - candidates[i], i);
        combination.pop_back();
    }
}

/**
 * https://leetcode.com/problems/combination-sum/
 * (candidates) (without duplicates) unlimited number of times.
 * all positive (candidates, target)
 */
vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
    vector<vector<int>> res;
    vector<int> combination;
    combinationSum(candidates, res, combination, target, 0);
    return res;
}

void combinationSum2(vector<int> &candidates, vector<vector<int>> &res,
                     vector<int> &combination, int target, int i) {
    if (target == 0) res.push_back(combination);
    if (target <= 0) return;

    for (int k = i; k < candidates.size() && candidates[k] <= target; k++) {
        combination.push_back(candidates[k]);
        if (k == i || candidates[k] != candidates[k - 1])
            combinationSum2(candidates, res, combination,
                            target - candidates[k], k + 1);
        combination.pop_back();
    }
}

/**
 * https://leetcode.com/problems/combination-sum-ii/
 * Each number in candidates may only be used once in the combination.
 * all positive (have duplicates)
 */
vector<vector<int>> combinationSum2(vector<int> &candidates, int target) {
    std::sort(candidates.begin(), candidates.end());
    vector<vector<int>> res;
    vector<int> combination;
    combinationSum2(candidates, res, combination, target, 0);
    return res;
}

void combinationSum3(vector<vector<int>> &res, vector<int> &combination, int k,
                     int n, int i) {
    if (n == 0 && k == 0) res.push_back(combination);
    if (n <= 0 || k <= 0) return;

    for (; i < 10 && i <= n; i++) {
        combination.push_back(i);
        combinationSum3(res, combination, k - 1, n - i, i + 1);
        combination.pop_back();
    }
}

/**
 * https://leetcode.com/problems/combination-sum-iii/
 * k numbers from 1 to 9 that add up to a number n (no repeates)
 */
vector<vector<int>> combinationSum3(int k, int n) {
    vector<vector<int>> res;
    vector<int> combination;
    combinationSum3(res, combination, k, n, 1);
    return res;
}

int combinationSum4(vector<int> &nums, vector<int> &dp, int target) {
    if (target < 0) return 0;
    if (dp[target] >= 0) return dp[target];
    dp[target] = 0;
    for (int i = 0; i < nums.size() && nums[i] <= target; i++)
        dp[target] += combinationSum4(nums, dp, target - nums[i]);
    return dp[target];
}

/**
 * https://leetcode.com/problems/combination-sum-iv/
 * all positive number(no duplicates), no limited times
 */
int combinationSum4(vector<int> &nums, int target) {
    sort(nums.begin(), nums.end());
    vector<int> dp(target + 1, -1);
    dp[0] = 1;
    return combinationSum4(nums, dp, target);
}

void combine(vector<vector<int>> &res, vector<int> &combination, int n, int k,
             int i) {
    if (k == 0) res.push_back(combination);
    if (k <= 0) return;

    for (; i <= n; i++) {
        combination.push_back(i);
        combine(res, combination, n, k - 1, i + 1);
        combination.pop_back();
    }
}
/**
 * https://leetcode.com/problems/combinations/
 * combinations of k numbers out of 1 ... n.
 */
vector<vector<int>> combine(int n, int k) {
    vector<vector<int>> res;
    vector<int> combination;
    combine(res, combination, n, k, 1);
    return res;
}

/**
 * https://leetcode.com/problems/first-missing-positive/
 */
int firstMissingPositive(vector<int> &nums) {
    for (int i = 0; i < nums.size(); i++) {
        while (nums[i] > 0 && nums[i] <= nums.size() &&
               nums[nums[i] - 1] != nums[i])
            swap(nums[i], nums[nums[i] - 1]);
    }
    for (int i = 0; i < nums.size(); i++)
        if (nums[i] != i + 1) return i + 1;
    return nums.size() + 1;
}

/**
 * https://leetcode.com/problems/trapping-rain-water/
 */
int trap(vector<int> &height) {
    int l = 0, r = height.size() - 1, water = 0, level = 0;
    while (l < r) {
        int lower = height[height[l] < height[r] ? l++ : r--];
        level = max(level, lower);
        water += level - lower;
    }
    return water;
}

/**
 * https://leetcode.com/problems/jump-game/
 */
bool canJump(vector<int> &nums) {
    int i = 0, reach = 0;
    for (reach = 0; i <= reach && i < nums.size(); i++)
        reach = max(reach, i + nums[i]);
    return i == nums.size();
}

/**
 * https://leetcode.com/problems/jump-game-ii/
 */
int jump(vector<int> &nums) {
    if (nums.size() < 2) return 0;
    int end = nums[0], nextend = end, step = 1;
    for (int i = 1; i <= end && i < nums.size() - 1; i++) {
        nextend = max(nextend, i + nums[i]);
        if (i == end) end = nextend, step++;
    }
    return step;
}

/**
 * https://leetcode.com/problems/rotate-image/
 */
void rotate(vector<vector<int>> &matrix) {
    reverse(matrix.begin(), matrix.end());
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < i; j++) std::swap(matrix[i][j], matrix[j][i]);
}

/**
 * https://leetcode.com/problems/maximum-subarray/
 */
int maxSubArray(vector<int> &nums) {
    int res = INT32_MIN, tmp = INT32_MIN;
    for (auto v : nums) {
        tmp < 0 ? tmp = v : tmp += v, res = max(tmp, res);
    }
    return res;
}

/**
 * https://leetcode.com/problems/spiral-matrix/
 */
vector<int> spiralOrder(vector<vector<int>> &matrix) {
    if (matrix.empty()) return {};
    int m = matrix.size(), n = matrix[0].size();

    vector<int> res;
    int i = 0, j = 0, left = 0, right = n - 1, top = 0, bottom = m - 1;
    while (true) {
        while (j <= right) res.push_back(matrix[i][j++]);
        --j, ++i, ++top;
        if (i > bottom) break;

        while (i <= bottom) res.push_back(matrix[i++][j]);
        --i, --j, --right;
        if (j < left) break;

        while (j >= left) res.push_back(matrix[i][j--]);
        ++j, --i, --bottom;
        if (i < top) break;

        while (i >= top) res.push_back(matrix[i--][j]);
        ++i, ++j, ++left;
        if (j > right) break;
    }
    return res;
}

/**
 * https://leetcode.com/problems/spiral-matrix-ii/
 */
vector<vector<int>> generateMatrix(int n) {
    if (n <= 0) return {};
    vector<vector<int>> matrix(n, vector<int>(n, 0));

    int i = 0, j = 0, left = 0, right = n - 1, top = 0, bottom = n - 1, v = 1;
    while (true) {
        while (j <= right) matrix[i][j++] = v++;
        --j, ++i, ++top;
        if (i > bottom) break;

        while (i <= bottom) matrix[i++][j] = v++;
        --i, --j, --right;
        if (j < left) break;

        while (j >= left) matrix[i][j--] = v++;
        ++j, --i, --bottom;
        if (i < top) break;

        while (i >= top) matrix[i--][j] = v++;
        ++i, ++j, ++left;
        if (j > right) break;
    }
    return matrix;
}

/**
 * https://leetcode.com/problems/merge-intervals/
 */
vector<vector<int>> merge(vector<vector<int>> &intervals) {
    if (intervals.empty()) return {};
    sort(intervals.begin(), intervals.end(),
         [](vector<int> &x, vector<int> &y) { return x[0] < y[0]; });

    vector<vector<int>> res = {intervals[0]};
    for (auto &v : intervals)
        if (v[0] <= res.back()[1])
            res.back()[1] = max(res.back()[1], v[1]);
        else
            res.push_back(v);
    return res;
}

/**
 * https://leetcode.com/problems/insert-interval/
 */
vector<vector<int>> insert(vector<vector<int>> &intervals,
                           vector<int> &newInterval) {
    vector<vector<int>> res;
    auto cur = newInterval;
    for (auto &v : intervals) {
        if (cur.empty() || v[1] < cur[0])
            res.push_back(v);
        else if (v[0] > cur[1]) {
            res.push_back(cur), vector<int>().swap(cur);
            res.push_back(v);
        } else
            cur[0] = min(v[0], cur[0]), cur[1] = max(v[1], cur[1]);
    }
    if (!cur.empty()) res.push_back(cur);
    return res;
}

/**
 * https://leetcode.com/problems/unique-paths
 */
int uniquePaths(int m, int n) {
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    dp[0][1] = 1;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++) dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
    return dp[m][n];
}

/**
 * https://leetcode.com/problems/unique-paths-ii/
 */
int uniquePathsWithObstacles(vector<vector<int>> &obstacleGrid) {
    int m = obstacleGrid.size(), n = obstacleGrid[0].size();
    vector<vector<long>> dp(m + 1, vector<long>(n + 1, 0));
    dp[0][1] = 1;
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            if (!obstacleGrid[i - 1][j - 1])
                dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
    return dp[m][n];
}

/**
 * https://leetcode.com/problems/minimum-path-sum/
 * non-negative
 */
int minPathSum(vector<vector<int>> &grid) {
    if (grid.empty()) return 0;
    int m = grid.size(), n = grid[0].size();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, INT32_MAX));
    dp[0][0] = dp[0][1] = dp[1][0] = 0;
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i - 1][j - 1];
    return dp[m][n];
}

/**
 * https://leetcode.com/problems/valid-sudoku/
 */
bool isValidSudoku(vector<vector<char>> &board) {
    vector<short> row(9, 0), col(9, 0), blk(9, 0);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] != '.') {
                int k = i / 3 * 3 + j / 3, idx = 1 << (board[i][j] - '0');
                if (row[i] & idx || col[j] & idx || blk[k] & idx) return false;
                row[i] |= idx, col[j] |= idx, blk[k] |= idx;
            }
    return true;
}

bool check(vector<vector<char>> &board, int i, int j, char c) {
    for (int k = 0; k < 9; k++)
        if (board[i][k] == c || board[k][j] == c ||
            board[i / 3 * 3 + k / 3][j / 3 * 3 + k % 3] == c)
            return false;
    return true;
}

bool solveSudoku(vector<vector<char>> &board, int i, int j) {
    if (i == 9) return true;
    if (j == 9) return solveSudoku(board, i + 1, 0);
    if (board[i][j] != '.') return solveSudoku(board, i, j + 1);

    for (auto c = '1'; c <= '9'; c++) {
        if (check(board, i, j, c)) {
            board[i][j] = c;
            if (solveSudoku(board, i, j + 1)) return true;
            board[i][j] = '.';
        }
    }
    return false;
}
/**
 * https://leetcode.com/problems/sudoku-solver/
 */
void solveSudoku(vector<vector<char>> &board) { solveSudoku(board, 0, 0); }

/**
 * https://leetcode.com/problems/plus-one/
 */
vector<int> plusOne(vector<int> &digits) {
    int extra = 1, sum;
    for (int i = digits.size() - 1; i >= 0; i--)
        sum = digits[i] + extra, extra = sum / 10, digits[i] = sum % 10;
    if (extra) digits.insert(digits.begin(), extra);
    return digits;
}

/**
 * https://leetcode.com/problems/set-matrix-zeroes/
 */
void setZeroes(vector<vector<int>> &matrix) {
    if (matrix.empty()) return;

    int m = matrix.size(), n = matrix[0].size();

    int k = -1, flag = 0;
    for (int i = 0; i < m; i++) {
        flag = 0;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 0) {
                flag = 1;
                if (k < 0) k = i;
                if (i != k) matrix[k][j] = 0;
            }
        }
        if (k >= 0 && i > k && flag)
            fill(matrix[i].begin(), matrix[i].end(), 0);
    }

    if (k == -1) return;
    for (int j = 0; j < n; j++)
        if (matrix[k][j] == 0)
            for (int i = 0; i < m; i++) matrix[i][j] = 0;

    fill(matrix[k].begin(), matrix[k].end(), 0);
}

/**
 * https://leetcode.com/problems/search-a-2d-matrix/
 */
bool searchMatrix(vector<vector<int>> &matrix, int target) {
    if (matrix.empty() || matrix[0].empty()) return false;
    int i = 0, j = matrix.size() - 1, mid;
    while (i < j - 1) {
        mid = (i + j) / 2;
        if (matrix[mid][0] == target) return true;
        matrix[mid][0] > target ? j = mid - 1 : i = mid;
    }
    int k = matrix[j][0] <= target ? j : i;
    return binary_search(matrix[k].begin(), matrix[k].end(), target);
}

/**
 * https://leetcode.com/problems/sort-colors/
 */
void sortColors(vector<int> &nums) {
    for (int k = 0, i = 0, j = nums.size() - 1; k <= j; k++) {
        while (nums[k] == 2 && k <= j) swap(nums[k], nums[j--]);
        while (nums[k] == 0 && i <= k) swap(nums[k], nums[i++]);
    }
}

/**
 * https://leetcode.com/problems/climbing-stairs/
 */
int climbStairs(int n) {
    vector<int> dp(n + 1, 1);
    for (int i = 2; i <= n; i++) dp[i] = dp[i - 1] + dp[i - 2];
    return dp[n];
}

void subsets(vector<vector<int>> &res, vector<int> &nums, vector<int> &vec,
             int i) {
    if (i == nums.size()) {
        res.push_back(vec);
        return;
    }

    subsets(res, nums, vec, i + 1);
    vec.push_back(nums[i]);
    subsets(res, nums, vec, i + 1);
    vec.pop_back();
}
/**
 * https://leetcode.com/problems/subsets/
 */
vector<vector<int>> subsets(vector<int> &nums) {
    vector<vector<int>> res;
    vector<int> vec;
    subsets(res, nums, vec, 0);
    return res;
}

void subsetsWithDup(vector<vector<int>> &res, vector<int> &nums,
                    vector<int> &vec, int i) {
    if (i == nums.size()) {
        res.push_back(vec);
        return;
    }

    vec.push_back(nums[i]);
    subsetsWithDup(res, nums, vec, i + 1);
    vec.pop_back();

    ++i;
    while (i < nums.size() && nums[i] == nums[i - 1]) ++i;
    subsetsWithDup(res, nums, vec, i);
}

/**
 * https://leetcode.com/problems/subsets-ii/
 */
vector<vector<int>> subsetsWithDup(vector<int> &nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> res;
    vector<int> vec;
    subsetsWithDup(res, nums, vec, 0);
    return res;
}

bool exist(vector<vector<char>> &board, vector<vector<bool>> &visited, int m,
           int n, int i, int j, string &word, int k) {
    if (k == word.size()) return true;
    if (i < 0 || i >= m || j < 0 || j >= n || visited[i][j] ||
        board[i][j] != word[k])
        return false;
    visited[i][j] = true;
    if (exist(board, visited, m, n, i - 1, j, word, k + 1) ||
        exist(board, visited, m, n, i + 1, j, word, k + 1) ||
        exist(board, visited, m, n, i, j - 1, word, k + 1) ||
        exist(board, visited, m, n, i, j + 1, word, k + 1))
        return true;
    visited[i][j] = false;
    return false;
}
/**
 * https://leetcode.com/problems/word-search/
 */
bool exist(vector<vector<char>> &board, string word) {
    if (board.empty()) return false;
    int m = board.size(), n = board[0].size();
    vector<vector<bool>> visited(m, vector<bool>(n, false));

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (exist(board, visited, m, n, i, j, word, 0)) return true;
    return false;
}

/**
 * https://leetcode.com/problems/largest-rectangle-in-histogram/
 */
int largestRectangleArea(vector<int> &heights) {
    heights.push_back(0);
    stack<int> stk;
    int i = 0, res = 0, h;
    while (i < heights.size()) {
        if (stk.empty() || heights[i] >= heights[stk.top()])
            stk.push(i++);
        else {
            h = stk.top(), stk.pop();
            res = max(res, heights[h] * (stk.empty() ? i : i - stk.top() - 1));
        }
    }
    return res;
}

int expand(vector<vector<char>> &matrix, int i, int j, int m, int n) {
    int res = m * n, flag = 0;
    int x = i + m, y = j + n, k;
    if (x < matrix.size()) {
        for (k = 0, flag = 0; k < n; k++)
            if (matrix[x][j + k] == '0') {
                flag = 1;
                break;
            }
        if (!flag) res = max(res, expand(matrix, i, j, m + 1, n));
    }
    if (y < matrix[0].size()) {
        for (k = 0, flag = 0; k < m; k++)
            if (matrix[i + k][y] == '0') {
                flag = 1;
                break;
            }
        if (!flag) res = max(res, expand(matrix, i, j, m, n + 1));
    }
    return res;
}

/**
 * https://leetcode.com/problems/maximal-rectangle/
 */
int maximalRectangle(vector<vector<char>> &matrix) {
    if (matrix.empty()) return 0;
    int res = 0;
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix[0].size(); j++)
            if (matrix[i][j] == '1') res = max(res, expand(matrix, i, j, 1, 1));
    return res;
}

/**
 * https://leetcode.com/problems/merge-sorted-array/
 */
void merge(vector<int> &nums1, int m, vector<int> &nums2, int n) {
    while (n > 0)
        nums1[m + n - 1] =
            (m < 1 || nums1[m - 1] < nums2[n - 1]) ? nums2[--n] : nums1[--m];
}


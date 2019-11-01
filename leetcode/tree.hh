#include <algorithm>
#include <iostream>
#include <queue>
#include <stack>
#include <vector>

using std::max;
using std::cout;
using std::queue;
using std::stack;
using std::vector;

/**
 * Definition for a binary tree node.
 */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Node {
   public:
    int val;
    Node *left;
    Node *right;
    Node *next;

    Node() {}

    Node(int _val, Node *_left, Node *_right, Node *_next) {
        val = _val;
        left = _left;
        right = _right;
        next = _next;
    }
};

template <class Func>
void inorderTraversal(TreeNode *root, Func &&f) {
    TreeNode *p = root, *x;
    stack<TreeNode *> stk;
    while (p || !stk.empty()) {
        while (p) stk.push(p), p = p->left;
        p = stk.top(), stk.pop();
        x = p, p = p->right;
        f(x);
    }
}

template <class Func>
void preorderTraversal(TreeNode *root, Func &&f) {
    if (!root) return;
    TreeNode *x;
    stack<TreeNode *> stk;
    stk.push(root);
    while (!stk.empty()) {
        x = stk.top(), stk.pop();
        if (x->right) stk.push(x->right);
        if (x->left) stk.push(x->left);
        f(x);
    }
}

template <class Func>
void postorderTraversal(TreeNode *root, Func &&f) {
    if (!root) return;
    stack<TreeNode *> post;
    TreeNode *x;
    stack<TreeNode *> stk;
    stk.push(root);
    while (!stk.empty()) {
        x = stk.top(), stk.pop();
        if (x->left) stk.push(x->left);
        if (x->right) stk.push(x->right);
        post.push(x);
    }
    while (!post.empty()) f(post.top()), post.pop();
}

template <class Func>
void levelOrder(TreeNode *root, Func &&f) {
    if (!root) return;
    vector<TreeNode *> level = {root};
    while (!level.empty()) {
        vector<TreeNode *> next;
        for (auto &x : level) {
            if (x->left) next.push_back(x->left);
            if (x->right) next.push_back(x->right);
        }
        f(level);
        level.swap(next);
    }
}

/**
 * https://leetcode.com/problems/binary-tree-inorder-traversal/
 */
vector<int> inorderTraversal(TreeNode *root) {
    vector<int> res;
    inorderTraversal(root, [&res](TreeNode *x) { res.push_back(x->val); });
    return res;
}

/**
 * https://leetcode.com/problems/unique-binary-search-trees/
 */
int numTrees(int n) {
    if (n <= 1) return 1;
    vector<int> dp(n + 1, 0);
    dp[0] = dp[1] = 1;
    for (int i = 2; i <= n; i++)
        for (int k = 0; k < i; k++) dp[i] += dp[k] * dp[i - k - 1];
    return dp[n];
}

vector<TreeNode *> generateTrees(int i, int j) {
    if (i > j) return {NULL};
    if (i == j) return {new TreeNode(i)};

    vector<TreeNode *> res;
    for (int k = i; k <= j; k++)
        for (auto &left : generateTrees(i, k - 1))
            for (auto &right : generateTrees(k + 1, j)) {
                res.push_back(new TreeNode(k));
                res.back()->left = left, res.back()->right = right;
            }
    return res;
}

/**
 * https://leetcode.com/problems/unique-binary-search-trees-ii/
 */
vector<TreeNode *> generateTrees(int n) {
    if (n < 1) return {};
    return generateTrees(1, n);
}

/**
 * https://leetcode.com/problems/validate-binary-search-tree/
 */
bool isValidBST(TreeNode *root) {
    TreeNode *prev = NULL;
    bool flag = true;
    inorderTraversal(root, [&prev, &flag](TreeNode *x) {
        if (prev && x->val <= prev->val) flag = false;
        prev = x;
    });
    return flag;
}

/**
 * https://leetcode.com/problems/same-tree/
 */
bool isSameTree(TreeNode *p, TreeNode *q) {
    if (!p && !q) return true;
    if (!p || !q || p->val != q->val) return false;
    return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}

bool isSymmetric(TreeNode *p, TreeNode *q) {
    if (!p && !q) return true;
    if (!p || !q || p->val != q->val) return false;
    return isSymmetric(p->left, q->right) && isSymmetric(p->right, q->left);
}

/**
 * https://leetcode.com/problems/symmetric-tree/
 */
bool isSymmetric(TreeNode *root) {
    if (!root) return true;
    return isSymmetric(root->left, root->right);
}

/**
 * https://leetcode.com/problems/binary-tree-level-order-traversal/
 */
vector<vector<int>> levelOrder(TreeNode *root) {
    if (!root) return {};
    vector<vector<int>> res;
    levelOrder(root, [&res](vector<TreeNode *> &level) {
        res.push_back({});
        for (auto &x : level) res.back().push_back(x->val);
    });
    return res;
}

/**
 * https://leetcode.com/problems/binary-tree-zigzag-level-order-traversal/
 */
vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
    if (!root) return {};
    vector<vector<int>> res;
    bool flag = true;
    levelOrder(root, [&res, &flag](vector<TreeNode *> &level) {
        int n = level.size();
        res.push_back(vector<int>(n, 0));
        for (int i = 0; i < n; i++)
            res.back()[flag ? i : n - i - 1] = level[i]->val;
        flag = !flag;
    });
    return res;
}

/**
 * https://leetcode.com/problems/maximum-depth-of-binary-tree/
 */
int maxDepth(TreeNode *root) {
    if (!root) return 0;
    return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}

TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder, int i, int j,
                    int n) {
    if (n <= 0) return NULL;
    if (n == 1) return new TreeNode(preorder[i]);

    int k = 0;
    while (k < n && preorder[i] != inorder[j + k]) k++;

    auto p = new TreeNode(preorder[i]);
    p->left = buildTree(preorder, inorder, i + 1, j, k);
    p->right = buildTree(preorder, inorder, i + k + 1, j + k + 1, n - k - 1);
    return p;
}
/**
 * https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/
 */
TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
    return buildTree(preorder, inorder, 0, 0, preorder.size());
}

/**
 * https://leetcode.com/problems/construct-binary-tree-from-inorder-and-postorder-traversal/
 */
TreeNode *buildTree2(vector<int> &postorder, vector<int> &inorder, int i, int j,
                     int n) {
    if (n <= 0) return NULL;
    if (n == 1) return new TreeNode(postorder[i]);

    int k = 0;
    while (k < n && postorder[i + n - 1] != inorder[j + k]) k++;

    auto p = new TreeNode(postorder[i + n - 1]);
    p->left = buildTree2(postorder, inorder, i, j, k);
    p->right = buildTree2(postorder, inorder, i + k, j + k + 1, n - k - 1);
    return p;
}
/**
 * https://leetcode.com/problems/construct-binary-tree-from-preorder-and-inorder-traversal/
 */
TreeNode *buildTree2(vector<int> &inorder, vector<int> &postorder) {
    return buildTree2(postorder, inorder, 0, 0, postorder.size());
}

/**
 * https://leetcode.com/problems/binary-tree-level-order-traversal-ii/
 */
vector<vector<int>> levelOrderBottom(TreeNode *root) {
    if (!root) return {};
    vector<vector<int>> res;
    levelOrder(root, [&res](vector<TreeNode *> &level) {
        res.push_back({});
        for (auto &x : level) res.back().push_back(x->val);
    });
    reverse(res.begin(), res.end());
    return res;
}

TreeNode *sortedArrayToBST(vector<int> &nums, int i, int j) {
    if (i > j) return NULL;
    if (i == j) return new TreeNode(nums[i]);

    int k = (i + j) / 2;
    auto p = new TreeNode(nums[k]);
    p->left = sortedArrayToBST(nums, i, k - 1);
    p->right = sortedArrayToBST(nums, k + 1, j);
    return p;
}
/**
 * https://leetcode.com/problems/convert-sorted-array-to-binary-search-tree/
 */
TreeNode *sortedArrayToBST(vector<int> &nums) {
    return sortedArrayToBST(nums, 0, nums.size() - 1);
}

int isBalanced2(TreeNode *root) {
    if (!root) return 0;
    int left = isBalanced2(root->left), right = isBalanced2(root->right);
    if (left == -1 || right == -1 || abs(left - right) > 1) return -1;
    return std::max(left, right) + 1;
}
/**
 * https://leetcode.com/problems/balanced-binary-tree/
 */
bool isBalanced(TreeNode *root) { return isBalanced2(root) != -1; }

/**
 * https://leetcode.com/problems/minimum-depth-of-binary-tree/
 */
int minDepth(TreeNode *root) {
    if (!root) return 0;
    int h = 0, flag = -1;
    levelOrder(root, [&h, &flag](vector<TreeNode *> &level) {
        if (flag != -1) return;
        for (auto v : level)
            if (!v->left && !v->right) flag = 0;
        h++;
    });
    return h;
}

/**
 * https://leetcode.com/problems/path-sum/
 */
bool hasPathSum(TreeNode *root, int sum) {
    if (!root) return false;
    if (!root->left && !root->right && root->val == sum) return true;
    return hasPathSum(root->left, sum - root->val) ||
           hasPathSum(root->right, sum - root->val);
}

void pathSum(TreeNode *root, vector<vector<int>> &res, vector<int> &vec,
             int sum) {
    if (!root) return;

    vec.push_back(root->val);
    if (!root->left && !root->right && root->val == sum) res.push_back(vec);
    pathSum(root->left, res, vec, sum - root->val);
    pathSum(root->right, res, vec, sum - root->val);
    vec.pop_back();
    return;
}

/**
 * https://leetcode.com/problems/path-sum-ii/
 */
vector<vector<int>> pathSum(TreeNode *root, int sum) {
    if (!root) return {};
    vector<vector<int>> res;
    vector<int> vec;
    pathSum(root, res, vec, sum);
    return res;
}

/**
 * https://leetcode.com/problems/flatten-binary-tree-to-linked-list/
 */
void flatten(TreeNode *root) {
    if (!root) return;
    TreeNode *prev = root;
    preorderTraversal(root, [&prev](TreeNode *x) {
        x->left = x->right = NULL;
        prev->right = x, prev = x;
    });
    prev->right = NULL;
}

/**
 * https://leetcode.com/problems/populating-next-right-pointers-in-each-node/
 * https://leetcode.com/problems/populating-next-right-pointers-in-each-node-ii/
 */
Node *connect(Node *root) {
    if (!root) return NULL;

    queue<Node *> que;
    que.push(root);

    while (!que.empty()) {
        Node *prev = que.front(), *x;
        int n = que.size();
        for (; n > 0; n--) {
            x = que.front();
            if (x->left) que.push(x->left);
            if (x->right) que.push(x->right);
            prev = prev->next = x, que.pop();
        }
        prev->next = NULL;
    }
    return root;
}

int maxPathSum(TreeNode *root, int &res) {
    if (!root) return 0;

    int left = max(0, maxPathSum(root->left, res));
    int right = max(0, maxPathSum(root->right, res));

    res = max(res, left + right + root->val);
    return max(left, right) + root->val;
}
/**
 * https://leetcode.com/problems/binary-tree-maximum-path-sum/
 */
int maxPathSum(TreeNode *root) {
    if (!root) return 0;
    int res = INT32_MIN;
    maxPathSum(root, res);
    return res;
}

int sumNumbers(TreeNode *root, int parent) {
    if (!root) return 0;
    parent = parent * 10 + root->val;
    if (!root->left && !root->right) return parent;
    return sumNumbers(root->left, parent) + sumNumbers(root->right, parent);
}
/**
 * https://leetcode.com/problems/sum-root-to-leaf-numbers/
 */
int sumNumbers(TreeNode *root) { return sumNumbers(root, 0); }

/**
 * https://leetcode.com/problems/binary-tree-preorder-traversal/
 */
vector<int> preorderTraversal(TreeNode *root) {
    vector<int> res;
    preorderTraversal(root, [&res](TreeNode *x) { res.push_back(x->val); });
    return res;
}

/**
 * https://leetcode.com/problems/binary-tree-postorder-traversal/
 */
vector<int> postorderTraversal(TreeNode *root) {
    if (!root) return {};
    vector<int> res;
    postorderTraversal(root, [&res](TreeNode *x) { res.push_back(x->val); });
    return res;
}
#include <iostream>
#include <stack>
#include <vector>

using std::cout;
using std::endl;
using std::stack;
using std::vector;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

// Definition for a Node.
class Node {
   public:
    int val;
    Node *next;
    Node *random;

    Node() {}

    Node(int _val, Node *_next, Node *_random) {
        val = _val;
        next = _next;
        random = _random;
    }
};

void print(ListNode *head) {
    for (; head; head = head->next) cout << head->val << "->";
    cout << endl;
}

void print(Node *head) {
    for (auto node = head; node; node = node->next) cout << node->val << "->";
    cout << endl;
}

/**
 * https://leetcode.com/problems/add-two-numbers/
 * List stored in reverse order
 */
ListNode *addTwoNumbers(ListNode *l1, ListNode *l2) {
    ListNode res(0), *prev(&res);
    for (int extra = 0, sum; l1 || l2 || extra; extra = sum / 10) {
        sum = (l1 ? l1->val : 0) + (l2 ? l2->val : 0) + extra;
        prev->next = new ListNode(sum % 10), prev = prev->next;
        l1 = l1 ? l1->next : NULL;
        l2 = l2 ? l2->next : NULL;
    }
    return res.next;
}

/**
 * https://leetcode.com/problems/remove-nth-node-from-end-of-list/
 */
ListNode *removeNthFromEnd(ListNode *head, int n) {
    auto p = head, fast = head;
    int i;
    for (i = 0; i < n && fast; i++) fast = fast->next;
    if (!fast) return i == n ? head->next : head;

    for (; fast->next; fast = fast->next) p = p->next;
    p->next = p->next->next;
    return head;
}

/**
 * https://leetcode.com/problems/remove-linked-list-elements/
 */
ListNode *removeElements(ListNode *head, int val) {
    while (head && head->val == val) head = head->next;
    if (!head) return NULL;

    for (auto p = head; p->next;)
        p->next->val == val ? p->next = p->next->next : p = p->next;
    return head;
}

void deleteNode(ListNode *node) { *node = *node->next; }

/**
 * https://leetcode.com/problems/remove-duplicates-from-sorted-list/
 */

ListNode *deleteDuplicates(ListNode *head) {
    for (ListNode *prev = head, *p; prev && (p = prev->next);
         prev->next = p, prev = p)
        while (p && p->val == prev->val) p = p->next;
    return head;
}

/**
 * https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
 */
ListNode *deleteDuplicates2(ListNode *head) {
    ListNode res(0), *prev(&res), *p = head, *q;
    prev->next = head;
    while (p && (q = p->next)) {
        if (p->val == q->val) {
            while (q->next && q->next->val == q->val) q = q->next;
            p = q->next, prev->next = p;
        } else
            prev = p, p = p->next;
    }
    prev->next = p;
    return res.next;
}

/**
 * https://leetcode.com/problems/merge-two-sorted-lists/
 */
ListNode *mergeTwoLists(ListNode *l1, ListNode *l2) {
    auto res = new ListNode(0), p = res, p1 = l1, p2 = l2;
    for (; p1 && p2; p = p->next)
        p1->val < p2->val ? (p->next = p1, p1 = p1->next)
                          : (p->next = p2, p2 = p2->next);
    p->next = p1 ? p1 : p2;
    return res->next;
}

/**
 * https://leetcode.com/problems/merge-k-sorted-lists/
 */
ListNode *mergeKLists(std::vector<ListNode *> &lists) {
    for (int n = lists.size(); n > 1; n = (n + 1) / 2)
        for (int i = 0; i < n / 2; i++)
            lists[i] = mergeTwoLists(lists[i], lists[n - i - 1]);
    return lists.empty() ? NULL : lists[0];
}

/**
 * https://leetcode.com/problems/swap-nodes-in-pairs/
 */
ListNode *swapPairs(ListNode *head) {
    if (!head || !head->next) return head;
    ListNode res(0), *prev = &res, *p, *q;
    for (p = head; p && (q = p->next); prev = p, p = p->next) {
        prev->next = q;
        p->next = q->next;
        q->next = p;
    }
    return res.next;
}

/**
 * https://leetcode.com/problems/reverse-linked-list/
 */
ListNode *reverseList(ListNode *head) {
    ListNode res(0), *x;
    while (head) {
        x = head, head = head->next;
        x->next = res.next, res.next = x;
    }
    return res.next;
}

/**
 * https://leetcode.com/problems/reverse-linked-list-ii/
 */
ListNode *reverseBetween(ListNode *head, int m, int n) {
    ListNode res(0), *prev = &res, *p, *x, *tail;
    prev->next = head;
    for (int i = 0; i < m - 1; i++) prev = prev->next;

    tail = p = prev->next;
    for (int i = m; i < n + 1; i++) {
        x = p, p = p->next;
        x->next = prev->next, prev->next = x;
    }
    tail->next = p;
    return res.next;
}

/**
 * https://leetcode.com/problems/reverse-nodes-in-k-group/
 * 1-2-3-4-5 k=3 => 3-2-1-4-5
 */
ListNode *reverseKGroup(ListNode *head, int k) {
    ListNode res(0), *prev = &res, *x, *tail;

    int len = 0;
    for (auto p = head; p; p = p->next) ++len;

    for (int j = 0; j < len / k; j++) {
        tail = head;
        for (int i = 0; i < k && head; i++) {
            x = head, head = head->next;
            x->next = prev->next, prev->next = x;
        }
        prev = tail;
    }
    prev->next = head;
    return res.next;
}

/**
 * https://leetcode.com/problems/rotate-list/
 */
ListNode *rotateRight(ListNode *head, int k) {
    if (!head) return head;
    ListNode *tail, *p, *res;
    int n = 1, i;
    for (tail = head; tail->next; tail = tail->next) ++n;
    if ((k %= n) == 0) return head;

    for (i = 0, p = head; i < n - k - 1; i++) p = p->next;
    res = p->next, p->next = NULL, tail->next = head;
    return res;
}

/**
 * https://leetcode.com/problems/palindrome-linked-list/
 */
bool isPalindrome(ListNode *head) {
    stack<int> stk;
    for (auto p = head; p; p = p->next) stk.push(p->val);
    for (auto p = head; p; p = p->next, stk.pop())
        if (stk.top() != p->val) return false;
    return true;
}

/**
 * https://leetcode.com/problems/odd-even-linked-list/
 */
ListNode *oddEvenList(ListNode *head) {
    ListNode res1(0), res2(0), *prev1(&res1), *prev2(&res2);

    for (; head; head = head->next->next) {
        prev1 = prev1->next = head;
        if (!head->next) break;
        prev2 = prev2->next = head->next;
    }
    prev1->next = res2.next, prev2->next = NULL;
    return res1.next;
}

/**
 * https://leetcode.com/problems/split-linked-list-in-parts/
 */
vector<ListNode *> splitListToParts(ListNode *root, int k) {
    vector<ListNode *> res(k, NULL);
    int len = 0;
    for (auto p = root; p; p = p->next) len++;

    ListNode *tail;
    for (int n = len / k, m = len % k, i = 0; i < k; i++) {
        res[i] = tail = root;
        for (int j = 0; j < n + (i >= m ? -1 : 0) && tail; j++)
            tail = tail->next;
        if (!tail) break;
        root = tail->next, tail->next = NULL;
    }
    return res;
}

/**
 * https://leetcode.com/problems/partition-list/
 */
ListNode *partition(ListNode *head, int x) {
    ListNode res1(0), res2(0), *prev1(&res1), *prev2(&res2);
    for (; head; head = head->next)
        head->val < x ? (prev1 = prev1->next = head)
                      : (prev2 = prev2->next = head);

    prev1->next = res2.next, prev2->next = NULL;
    return res1.next;
}

/**
 * https://leetcode.com/problems/copy-list-with-random-pointer/
 */
Node *copyRandomList(Node *head) {
    if (!head) return head;
    for (auto p = head; p; p = p->next->next)
        p->next = new Node(p->val, p->next, NULL);

    for (auto p = head; p; p = p->next->next)
        p->next->random = p->random ? p->random->next : NULL;

    Node *res = head->next, *prev = res, *prev2 = head;
    for (auto p = head->next->next; p; p = p->next->next)
        prev2 = prev2->next = p, prev = prev->next = p->next;
    prev2->next = NULL;
    return res;
}

/**
 * https://leetcode.com/problems/linked-list-cycle/
 */
bool hasCycle(ListNode *head) {
    for (auto p = head, fast = head; fast && fast->next;)
        if ((p = p->next) == (fast = fast->next->next)) return true;
    return false;
}

/**
 * https://leetcode.com/problems/linked-list-cycle-ii/
 */
ListNode *detectCycle(ListNode *head) {
    ListNode *p, *fast;
    for (p = fast = head; fast && fast->next;)
        if ((p = p->next) == (fast = fast->next->next)) break;
    if (!fast || !fast->next) return NULL;

    for (fast = head; fast != p;) fast = fast->next, p = p->next;
    return p;
}

/**
 * https://leetcode.com/problems/reorder-list/
 */
void reorderList(ListNode *head) {
    ListNode res(0), *prev(&res), *p1, *p2, *p1end;

    for (p1 = p2 = head; p2 && p2->next;) p1 = p1->next, p2 = p2->next->next;
    p1end = p1;
    p2 = reverseList(p1);

    for (p1 = head; p1 != p1end && p2;) {
        prev = prev->next = p1, p1 = p1->next;
        prev = prev->next = p2, p2 = p2->next;
    }
    prev = prev->next = p1 ? p1 : p2;
    prev->next = NULL;
}

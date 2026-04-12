//@compile g++ part5/09_02.cpp -std=c++20 -Wall -Wextra -Wpedantic
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////

class Tree
{
public:

    struct Node
    {
        int value = 0;

        std::shared_ptr < Node > left;
        std::shared_ptr < Node > right;
        std::weak_ptr   < Node > parent;

        explicit Node(int v) : value(v)
        {
            std::cout << "[Node] constructed: " << value << '\n';
        }

        ~Node()
        {
            std::cout << "[Node] destructed:  " << value << '\n';
        }
    };

    //  -------------------------------------------------------------------------------------------

    std::shared_ptr < Node > root;

    //  -------------------------------------------------------------------------------------------

    void traverse_v1() const
    {
        if (!root) return;

        std::queue < std::shared_ptr < Node > > queue;
        queue.push(root);

        std::cout << "BFS:";

        while (!queue.empty())
        {
            auto node = queue.front();
            queue.pop();

            std::cout << ' ' << node->value;

            if (node->left)  queue.push(node->left);
            if (node->right) queue.push(node->right);
        }

        std::cout << '\n';
    }

    void traverse_v2() const
    {
        std::cout << "DFS:";
        dfs(root);
        std::cout << '\n';
    }

    void print() const
    {
        if (!root) return;

        std::unordered_map < const Node *, int > col;
        int idx = 0;
        inorder_assign(root, col, idx);

        using Level = std::vector < std::shared_ptr < Node > >;
        std::vector < Level > levels;
        std::queue  < std::shared_ptr < Node > > q;
        q.push(root);

        while (!q.empty())
        {
            int sz = static_cast < int > (q.size());
            levels.emplace_back();
            for (int i = 0; i < sz; ++i)
            {
                auto n = q.front(); q.pop();
                levels.back().push_back(n);
                if (n->left)  q.push(n->left);
                if (n->right) q.push(n->right);
            }
        }

        const int cw    = 4;
        const int width = idx * cw;

        auto center = [&](const Node * n) { return col.at(n) * cw + cw / 2; };

        for (int lvl = 0; lvl < static_cast < int > (levels.size()); ++lvl)
        {
            std::string line(width, ' ');
            for (auto & node : levels[lvl])
            {
                int         pos = center(node.get());
                std::string val = std::to_string(node->value);
                for (int i = 0; i < static_cast < int > (val.size()); ++i)
                    if (pos + i < width) line[pos + i] = val[i];
            }
            std::cout << line << '\n';

            if (lvl + 1 < static_cast < int > (levels.size()))
            {
                std::string br(width, ' ');
                for (auto & node : levels[lvl])
                {
                    int pc = center(node.get());
                    if (node->left)
                        br[(pc + center(node->left.get()))  / 2] = '/';
                    if (node->right)
                        br[(pc + center(node->right.get())) / 2] = '\\';
                }
                std::cout << br << '\n';
            }
        }
    }

private:

    void dfs(const std::shared_ptr < Node > & node) const
    {
        if (!node) return;

        std::cout << ' ' << node->value;
        dfs(node->left);
        dfs(node->right);
    }

    void inorder_assign(const std::shared_ptr < Node >      & node,
                        std::unordered_map < const Node *, int > & col,
                        int                                    & idx) const
    {
        if (!node) return;
        inorder_assign(node->left,  col, idx);
        col[node.get()] = idx++;
        inorder_assign(node->right, col, idx);
    }
};

///////////////////////////////////////////////////////////////////////////////////////////////

static std::shared_ptr < Tree::Node > make_node(int value,
                                                 std::shared_ptr < Tree::Node > parent = nullptr)
{
    auto node = std::make_shared < Tree::Node > (value);

    if (parent) node->parent = parent;

    return node;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Tree tree;

    tree.root          = make_node(1);
    tree.root->left    = make_node(2, tree.root);
    tree.root->right   = make_node(3, tree.root);

    tree.root->left->left   = make_node(4, tree.root->left);
    tree.root->left->right  = make_node(5, tree.root->left);
    tree.root->right->left  = make_node(6, tree.root->right);
    tree.root->right->right = make_node(7, tree.root->right);

    std::cout << '\n';
    tree.print();

    std::cout << '\n';
    tree.traverse_v1();
    tree.traverse_v2();
    std::cout << '\n';

    std::cout << "root use_count: " << tree.root.use_count() << '\n';
    std::cout << '\n';

    std::cout << "--- destroying tree ---\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////

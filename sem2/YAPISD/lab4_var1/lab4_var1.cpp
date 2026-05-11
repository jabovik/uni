#include <fstream>

struct Tree
{
    int key;
    Tree *left;
    Tree *right;
};

void add_tree(Tree *&root, int key)
{
    if (!root)
    {
        root = new Tree;
        root->key = key;
        root->left = nullptr;
        root->right = nullptr;
        return;
    }
    if (key <= root->key)
        add_tree(root->left, key);
    else
        add_tree(root->right, key);
}

Tree *read_tree(std::ifstream &input)
{
    int key;
    Tree *root = nullptr;
    while (input >> key)
        add_tree(root, key);
    return root;
}

void invert_tree(Tree *root)
{
    if (!root)
        return;
    Tree *temp = root->left;
    root->left = root->right; // swap
    root->right = temp;
    invert_tree(root->left);
    invert_tree(root->right);
}

void print_tree(std::ofstream &output, Tree *root)
{
    if (!root)
        return;
    output << root->key << " "; // прямой обход: корень, левое поддерево, правое поддерево
    print_tree(output, root->left);
    print_tree(output, root->right);
}

void delete_tree(Tree *root)
{
    if (!root)
        return;
    delete_tree(root->left); // обратный обход: левое поддерево, правое поддерево, корень
    delete_tree(root->right);
    delete root;
}

int main(int argc, char const *argv[])
{
    std::ifstream input("input.txt");
    Tree *root = read_tree(input);
    input.close();
    if (root)
    {
        std::ofstream output("output.txt");
        invert_tree(root);
        print_tree(output, root);
        output.close();
        delete_tree(root);
    }
    return 0;
}

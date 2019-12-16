#include"tree.cpp"

void lrswap(node_t* node)
{
    if(node->left == nullptr && node->right == nullptr)
        return;

    node_t* tmp = node->right;
    node->right = node->left;
    node->left = tmp;

    lrswap(node->left);
    lrswap(node->right);
}

int main(int argc, char* argv[])
{
    if(argc < 3)
        return printf("Error!\n");

    FILE* outfile = fopen(argv[2], "w");

    assert(outfile != nullptr);

    node_t* root = nullptr;

    NodeReadDump(root, argv[1]);
    lrswap(root);
    NodeWriteDump(root, outfile);

    fclose(outfile);
}
#include"tree.hpp"
#include"reader.cpp"

const char* RIGHTNODECOLOR = "green";
const char* LEFTNODECOLOR  = "red";
const char* RIGHTNODEWORD  = "Yes";
const char* LEFTNODEWORD   = "No";

const char* DOTDUMPFILE = "dump.dot";

enum Pointers
{
    PointerInit         = 1,
    PointerFinalized    = 2,
};

#define PTR(name, type) (type*) Pointer##name

enum ReturnCodes
{
    ReturnCodeOk        = 0,
    ReturnCodeAllocErr  = 17,
    ReturnCodeNoNodes   = 18,
};

#define RETCODE(name) ReturnCode##name;

// Tree functions

int TreeInit(tree_t* &self)
{
    self = (tree_t*) calloc(sizeof(tree_t), 1);

    if(self == nullptr)
        return RETCODE(AllocErr);

    self->root = nullptr;

    return RETCODE(Ok);
}

int TreeFinalize(tree_t* &self)
{
    if(self->root != nullptr)
        NodeFinalize(self->root);
    
    self->root = nullptr;

    return RETCODE(Ok);
}

int TreeDotDump(const tree_t* self, const char* path)
{
    if(self->root == nullptr)
        return RETCODE(NoNodes);

    return NodeDotDump(self->root, path);
}

int TreeReadDump(tree_t* self, const char* path)
{
    node_t* root = nullptr;

    if(NodeReadDump(root, path) == 1)
    {
        self->root = nullptr;
        return 1;
    }

    self->root = root;

    return 0;
}

int TreeWriteDump(tree_t* self, FILE* dump)
{
    return NodeWriteDump(self->root, dump);
}

// Node functions

int NodeInit(node_t* &self, const char* data)
{
    size_t len = strlen(data);

    self = (node_t*) calloc(NodeCalcSize_(len), 1);

    if(self == nullptr)
        return RETCODE(AllocErr);
    
    self->left  = nullptr;
    self->right = nullptr;
    strcpy(self->data, data);

    return RETCODE(Ok);
}

int NodeFinalize(node_t* &self)
{
    if(self == nullptr)
        return RETCODE(Ok);
    
    if(self->right != nullptr)
        NodeFinalize(self->right);

    if(self->left != nullptr)
        NodeFinalize(self->left);

    free(self);
    self = nullptr;

    return RETCODE(Ok);
}

int NodeAddRight(node_t* self, const char* data)
{
    if(self->right != nullptr)
        NodeFinalize(self->right);

    return NodeInit(self->right, data);
}

int NodeAddLeft(node_t* self, const char* data)
{
    if(self->left != nullptr)
        NodeFinalize(self->left);

    return NodeInit(self->left, data);
}

int NodeDotDump(const node_t* self, const char* path)
{
    FILE* dotfile = fopen(path, "w");

    assert(dotfile != nullptr);

    fprintf(dotfile, "digraph Tree {\n");

    DotWriteNode_(self, dotfile);

    fprintf(dotfile, "\n");

    DotWriteCommunications_(self, dotfile);

    fprintf(dotfile, "}\n");

    fclose(dotfile);

    return RETCODE(Ok);
}

int NodeReadDump(node_t* &self, const char* path)
{
    FILE* dump = fopen(path, "r");
    assert(dump != nullptr);

    self = loadNode(dump);

    fclose(dump);

    if(self == ERRPTR(node_t))
        return 1;
    
    return 0;
}

int NodeWriteDump(node_t* node, FILE* dumpfile)
{
    static int indent = 0;

    printSpaces(dumpfile, indent);
    fprintf(dumpfile, "{\n");

    ++indent;

    printSpaces(dumpfile, indent);
    fprintf(dumpfile, "'%s'\n", node->data);

    if(node->right != nullptr)
        NodeWriteDump(node->right, dumpfile);
    
    else
        if(node->left != nullptr)
        {
            printSpaces(dumpfile, indent);
            fprintf(dumpfile, "nil\n");
        }
    
    if(node->left != nullptr)
        NodeWriteDump(node->left, dumpfile);
    
    --indent;

    printSpaces(dumpfile, indent);
    fprintf(dumpfile, "}\n");

    return 0;
}

size_t NodeCalcSize_(size_t len)
{
    return sizeof(node_t) + sizeof(char) * (len - 1);
}

int DotWriteNode_(const node_t* self, FILE* dotfile)
{
    fprintf(dotfile, "    node_%p [label=\"%s\"];\n", self, self->data);
    
    if(self->right != nullptr)
        DotWriteNode_(self->right, dotfile);

    if(self->left != nullptr)
        DotWriteNode_(self->left, dotfile);
    
    return RETCODE(Ok);
}

int DotWriteCommunications_(const node_t* self, FILE* dotfile)
{
    if(self->right != nullptr)
    {
        fprintf(dotfile, "    node_%p -> node_%p [label=\"%s\", color=\"%s\"];\n", self, self->right, RIGHTNODEWORD, RIGHTNODECOLOR);
        DotWriteCommunications_(self->right, dotfile);
    }

    if(self->left != nullptr)
    {
        fprintf(dotfile, "    node_%p -> node_%p [label=\"%s\", color=\"%s\"];\n", self, self->left, LEFTNODEWORD, LEFTNODECOLOR);
        DotWriteCommunications_(self->left, dotfile);
    }

    return RETCODE(Ok);
}

void printSpaces(FILE* outfile, int amount)
{
    for(int i = 0; i < amount; ++i)
        fprintf(outfile, "    ");
}
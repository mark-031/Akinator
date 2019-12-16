#include"reader.hpp"

#define ERRPTR(type) (type*) 1

tree_t* loadTree(FILE* dump, tree_t* tree)
{
    node_t* root = loadNode(dump);

    if(root == ERRPTR(node_t))
        return ERRPTR(tree_t);

    tree->root = root;

    return tree;
}

node_t* loadNode(FILE* dump)
{
    char* data = readFile(dump);
    const char* buf = data;

    skipSpaces(buf);

    node_t* node = readNode(buf);

    delete data;

    return node;
}

node_t* readNode(const char* &data)
{
    if(*data == 'n')
    {
        if(data[1] == 'i' && data[2] == 'l')
        {
            data += 3;
            skipSpaces(data);
            return nullptr;
        }
        else
            return ERRPTR(node_t);
    }

    if(*data != '{')
        return ERRPTR(node_t);
    
    ++data;

    skipSpaces(data);

    const char* text = readNodeText(data);

    if(text == ERRPTR(const char))
        return ERRPTR(node_t);
    
    skipSpaces(data);

    node_t* nodes[2];

    for(int i = 0; i < 2; ++i)
    {
        if(*data != '}')
        {
            nodes[i] = readNode(data);

            if(nodes[i] == ERRPTR(node_t))
            {
                delete text;
                return ERRPTR(node_t);
            }
        }

        else
            nodes[i] = nullptr;
    }

    if(*data != '}')
    {
        delete text;
        return ERRPTR(node_t);
    }

    node_t* node = nullptr;

    NodeInit(node, text);

    delete text;

    node->right = nodes[0];
    node->left  = nodes[1];

    ++data;
    skipSpaces(data);

    return node;
}

const char* readNodeText(const char* &data)
{
    if(*data != '\'' && *data != '"')
        return ERRPTR(const char);

    const char quote = *data;

    const char* start = ++data;

    while(*data != quote)
    {
        if(*data == '\0') return ERRPTR(const char);
        ++data;
    }

    size_t size = data - start + 1;
    ++data;

    char* text = new char[size];

    for(size_t i = 0; i < size - 1; ++i)
        text[i] = start[i];

    text[size - 1] = '\0';

    return text;
}

void skipSpaces(const char* &data)
{
    while(*data == ' ' || *data == '\n' || *data == '\t') ++data;
}

size_t getFileSize(FILE* file)
{
    size_t cur = ftell(file);

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, cur, SEEK_SET);

    return size;
}

char* readFile(FILE* file)
{
    size_t size = getFileSize(file);
    
    char* buffer = new char[size];
    assert(buffer != nullptr);

    fread(buffer, sizeof(char), size, file);

    return buffer;
}
tree_t*     loadTree(FILE* dump, tree_t* tree);
node_t*     loadNode(FILE* dump);
node_t*     readNode(const char* &data);
const char* readNodeText(const char* &data);
void        skipSpaces(const char* &data);
size_t      getFileSize(FILE* file);
char*       readFile(FILE* file);
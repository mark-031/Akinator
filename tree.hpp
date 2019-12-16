#include<assert.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct tree_t;
struct node_t;

struct tree_t
{
    node_t* root;
};

struct node_t
{
    node_t* right;
    node_t* left;

    char data[1];
};

int TreeInit(tree_t* &self);
int TreeFinalize(tree_t* &self);
int TreeDotDump(const tree_t* self, const char* path);
int TreeReadDump(tree_t* self, const char* path);
int TreeWriteDump(tree_t* self, FILE* dump);

int NodeInit(node_t* &self, const char* data);
int NodeFinalize(node_t* &self);
int NodeAddRight(node_t* self, const char* data);
int NodeAddLeft(node_t* self, const char* data);
int NodeDotDump(const node_t* self, const char* path);
int NodeReadDump(node_t* &self, const char* path);
int NodeWriteDump(node_t* node, FILE* dumpfile);

size_t NodeCalcSize_(size_t len);
int DotWriteNode_(const node_t* self, FILE* dotfile);
int DotWriteCommunications_(const node_t* self, FILE* dotfile);
void printSpaces(FILE* outfile, int amount);
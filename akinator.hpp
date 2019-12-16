#include"tree.cpp"
#include"way.cpp"

const char* AKINATORPATH  = "akinator.txt";
const int   LINEANSLENMAX = 128;

class Akinator
{
    private:
        node_t* root;

        void clear_();

        void skipLine_();

        int getAnswer_(const char* question, const char* prefix = nullptr);

        char* getLineAnswer_(const char* question);

        void addDifference_(node_t* node, node_t* parrent);

        int checkWay_(node_t* node, const char* target, Way* way, int depth);

        int isFoliage_(node_t* node);

        void enterToContinue_();

        void writeDump_(const char* path);

        void toPng_();

    public:
        Akinator(const char* path = AKINATORPATH);

        ~Akinator();

        int start();

        int play();

        int getinfo();

        int compare();

        Way* findWay(const char* name);
};
#include"akinator.hpp"

void Akinator::clear_()
{
    std::system("clear");
}

void Akinator::skipLine_()
{
    while(getchar() != '\n');
}

int Akinator::getAnswer_(const char* question, const char* prefix)
{
    char ans = '\0';

    while(ans != 'y' && ans != 'n')
    {
        this->clear_();

        if(prefix != nullptr)
            printf("%s ", prefix);

        printf("%s? [y/n]\n", question);

        ans = getchar();

        this->skipLine_();
    }

    if(ans == 'y')
        return 1;
    else
        return 0;
}

char* Akinator::getLineAnswer_(const char* question)
{
    char line[LINEANSLENMAX] = {};

    this->clear_();

    printf("%s? (%d symbols max)\n", question, LINEANSLENMAX - 1);
    scanf("%[^\n]", line);

    line[LINEANSLENMAX - 1] = '\0';

    this->skipLine_();

    size_t len = strlen(line) + 1;

    char* ans = new char[len];

    strcpy(ans, line);

    return ans;
}

void Akinator::addDifference_(node_t* node, node_t* parrent)
{
    assert(node    != nullptr);
    assert(parrent != nullptr);
    assert(this->isFoliage_(node));
    assert(parrent->right == node || parrent->left == node);

    if(!this->isFoliage_(node))
        return;

    char* rightans = this->getLineAnswer_("So what was the correct answer");

    const char* line1 = "And what was the difference between ";
    const char* line2 = " and ";

    size_t len = strlen(line1) + strlen(node->data)
               + strlen(line2) + strlen(rightans)
               + 1;
    
    char* question = new char[len];

    question[0] = '\0';
    
    strcat(question, line1);
    strcat(question, node->data);
    strcat(question, line2);
    strcat(question, rightans);

    char* dif = this->getLineAnswer_(question);

    delete question;

    node_t* difnode = nullptr;
    NodeInit(difnode, dif);

    node_t* rightnode = nullptr;
    NodeInit(rightnode, rightans);

    difnode->right = rightnode;
    difnode->left  = node;

    if(parrent->right == node)
        parrent->right = difnode;
    
    else
        parrent->left = difnode;
}

int Akinator::checkWay_(node_t* node, const char* target, Way* way, int depth)
{
    if(this->isFoliage_(node))
    {
        way->setAmount(depth);
        return !strcmp(node->data, target);
    }
    else
    {
        if(this->checkWay_(node->right, target, way, depth + 1))
            way->setRight(depth);
        
        else if(this->checkWay_(node->left, target, way, depth + 1))
            way->setLeft(depth);
        
        else
            return 0;
        
        return 1;
    }
}

int Akinator::isFoliage_(node_t* node)
{
    if(node->left == nullptr && node->right == nullptr)
        return 1;
    
    return 0;
}

void Akinator::enterToContinue_()
{
    printf("Press [ENTER] to continue...");
    getchar();
}

void Akinator::writeDump_(const char* path)
{
    FILE* dump = fopen(path, "w");
    assert(dump != nullptr);

    NodeWriteDump(this->root, dump);

    fclose(dump);
}

void Akinator::toPng_()
{
    NodeDotDump(this->root, "akinator.dot");

    std::system("dot -Tpng akinator.dot -o akinator.png");
}

Akinator::Akinator(const char* path)
{
    NodeReadDump(this->root, path);

    if(this->root == ERRPTR(node_t))
        this->root = nullptr;
}

Akinator::~Akinator()
{
    if(this->root != nullptr)
        NodeFinalize(this->root);
}

int Akinator::start()
{
    char game = '\0';
    while(true)
    {
        this->toPng_();
        this->clear_();

        printf("Akinator menu\n");
        printf("What do you want to choose?\n");
        printf("p - play\n");
        printf("i - info\n");
        printf("c - compare\n");

        printf("\ne - exit\n");

        game = getchar();
        this->skipLine_();

        switch(game)
        {
            case 'p':
                this->play();
                break;
            
            case 'i':
                this->getinfo();
                break;
            
            case 'c':
                this->compare();
                break;

            case 'e':
                printf("Goodbye!\n");

                this->writeDump_("akinator.txt");

                this->enterToContinue_();
                return 0;
            
            default:
                break;
        }
    }    
}

int Akinator::play()
{
    assert(this->root != nullptr);

    node_t* prev    = nullptr;
    node_t* current = this->root;

    while(current->left != nullptr && current->right != nullptr)
    {
        prev = current;
        if(this->getAnswer_(current->data) == 1)
            current = current->right;
        else
            current = current->left;
    }

    if(this->getAnswer_(current->data, "Is it") == 0)
        this->addDifference_(current, prev);

    return 0;
}

int Akinator::getinfo()
{
    this->clear_();

    char* target = this->getLineAnswer_("Who is the target");

    Way* way = this->findWay(target);

    delete target;

    if(way == nullptr)
    {
        printf("I don't know this one!\n");
        this->enterToContinue_();
        return 1;
    }

    node_t* current = this->root;
    int turn = 0;
    while((turn = way->getNextTurn()) >= 0)
    {
        if(turn > 0)
        {
            printf("[YES] %s\n", current->data);
            current = current->right;
        }
        else
        {
            printf("[NO]  %s\n", current->data);
            current = current->left;
        }
    }
    delete way;

    this->enterToContinue_();

    return 0;
}

int Akinator::compare()
{
    char* names[2] = {};
    Way*  ways[2] = {};

    for(int i = 0; i < 2; ++i)
    {
        this->clear_();
        names[i] = this->getLineAnswer_("Who");
        ways[i] = this->findWay(names[i]);

        if(ways[i] == nullptr)
        {
            printf("I know nothing about this one!\n");

            for(int j = 0; j < i; ++j)
                delete ways[i];

            this->enterToContinue_();

            return 1;
        }
    }

    this->clear_();

    int turns[2] = {};

    node_t* lastcommon = this->root;

    while(turns[0] >= 0 && turns[1] >= 0)
    {
        for(int i = 0; i < 2; ++i)
            turns[i] = ways[i]->getNextTurn();
        
        if(turns[0] == turns[1])
        {
            if(turns[0] > 0)
            {
                printf("    [YES] %s\n", lastcommon->data);
                lastcommon = lastcommon->right;
            }
            else if(turns[1] == 0)
            {
                printf("    [NO]  %s\n", lastcommon->data);
                lastcommon = lastcommon->left;
            }
            else
                break;
        }
        else
            break;
    }

    node_t* paths[2] = {lastcommon, lastcommon};

    for(int i = 0; i < 2; ++i)
    {
        if(turns[i] >= 0)
            printf("\nPath for \"%s\"\n", names[i]);

        while(turns[i] >= 0)
        {
            if(turns[i] > 0)
            {
                printf("    [YES] %s\n", paths[i]->data);
                paths[i] = paths[i]->right;
            }
            else
            {
                printf("    [No]  %s\n", paths[i]->data);
                paths[i] = paths[i]->left;
            }

            turns[i] = ways[i]->getNextTurn();
        }

        delete ways[i];
        delete names[i];
    }

    this->enterToContinue_();

    return 0;
}

Way* Akinator::findWay(const char* name)
{
    Way* way = new Way;

    if(this->checkWay_(this->root, name, way, 0) != 1)
    {
        delete way;

        return nullptr;
    }

    return way;
}


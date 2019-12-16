#include<assert.h>

const int MAXDEPTH = 128;

class Way
{
    private:
        int counter;
        int amount;
        char data[MAXDEPTH / 8];
    
    public:
        Way()
        {
            counter = 0;
            amount  = 0;
        }

        void setRight(int depth)
        {
            assert(depth < MAXDEPTH);

            data[depth / 8] |= (1 << (depth % 8));
        }

        void setLeft(int depth)
        {
            assert(amount < MAXDEPTH);

            data[depth / 8] &= ~(1 << (depth % 8));
        }

        void setAmount(int amount_)
        {
            assert(amount_ < MAXDEPTH);
            amount = amount_;
        }

        int getNextTurn()
        {
            if(counter >= amount)
                return -1;

            return data[counter / 8] & (1 << (counter++ % 8));
        }

        void rewind()
        {
            counter = 0;
        }
};
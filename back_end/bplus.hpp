//
//
//




#ifndef UNTITLED_BPLUS_HPP
#define UNTITLED_BPLUS_HPP
#include<fstream>


namespace sjtu {
    template<class Key, class Value>
    class BPPtree {
    private:
        const int M, L;
        FILE *file;
        node root;

    public:

        BPPtree(const char *F) {
            file = fopen(F, "a");
        }

        ~BPPtree() {
            fclose(file);
        }

        void read_disk() {

        }

        void remove() {


        }

        void insert(const Key &K, const Value &V) {


        }


        Value &search(const Key &K) {

        }



        void split() {}


    };


}










#endif //UNTITLED_BPLUS_HPP

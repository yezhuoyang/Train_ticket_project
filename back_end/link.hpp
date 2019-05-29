//
// Created by yezhuoyang on 2019-05-25.
//

#ifndef DEBUG_LINK_HPP
#define DEBUG_LINK_HPP
#include "../STLITE/vector.hpp"
#include "../STLITE/deque.hpp"
#include<vector>
#include "constant.h"
namespace sjtu{
    //这是一个用来存储外存某一块区域的大小、位置的结构体
    struct block{
        size_t Size;
        size_t pos;
        block(const size_t &S=0,const size_t& P=0):Size(S),pos(P){}
    };
    template<class value>
    class link{
        private:
            const size_t  blocksize;
            char  filename[FILENAME];
            FILE* F;
        public:
            link(const char* f):blocksize(sizeof(value)){
                strcpy(filename,f);
                F=fopen(filename,"rb+");
                if(F==NULL){
                    F=fopen(filename,"wb+");
                }
                fseek(F,0,SEEK_END);
            }
            ~link(){
                fclose(F);
            }
            block push_back(deque<value>& D){
                fseek(F,0,SEEK_END);
                size_t cp=ftell(F);
                value buffer[D.size()];
                size_t pos=0;
                size_t s=D.size();
                while(!D.empty()){
                       buffer[pos++]=D.front();
                       D.pop_front();
                }
                fwrite(buffer,blocksize,s,F);
                fflush(F);
                return block(s,cp/blocksize);
            }
            void read_block(const block&B,vector<value>& V){
                fseek(F,B.pos*blocksize,SEEK_SET);
                value buffer[B.Size];
                fread(buffer,blocksize,B.Size,F);
                for(int i=0;i<B.Size;++i){
                    V.push_back(buffer[i]);
                }
            }
            void clear(){
                freopen(filename,"w+",F);
            }
            void read_block(const block&B,std::vector<value>& V){
                fseek(F,B.pos*blocksize,SEEK_SET);
                value buffer[B.Size];
                fread(buffer,blocksize,B.Size,F);
                for(int i=0;i<B.Size;++i){
                    V.push_back(buffer[i]);
                }
            }
    };


    template<class value>
    class list{
    private:
        char  filename[FILENAME];
        FILE* F;
        int Sz;
        const size_t blocksize;
    public:
        list(const char* FN):blocksize(sizeof(value)){
            strcpy(filename,FN);
            F=fopen(filename,"rb+");
            if(F==NULL){
                F=fopen(filename,"wb+");
            }
            fseek(F,0,SEEK_END);
            Sz=ftell(F)/blocksize;
        }
        ~list(){
            fclose(F);
        }
        int Size(){
            return Sz;
        }
        void clear(){
            freopen(filename,"w+",F);
            Sz=0;
        }
        int push_back(const value& V){
            fseek(F,0,SEEK_END);
            fwrite(&V,blocksize,1,F);
            fflush(F);
            ++Sz;
            return 1;
        }
        int modify(const int&pos,const value& V){
            if(pos>=Sz) return 0;
            fseek(F,pos*blocksize,SEEK_SET);
            int cp=ftell(F);
            fwrite(&V,blocksize,1,F);
            fflush(F);
            return 1;
        }
        int find(const int&pos,value &V){
            if(pos>=Sz) return 0;
            fseek(F,pos*blocksize,SEEK_SET);
            fread(&V,blocksize,1,F);
            return 1;
        }
    };





}






#endif //DEBUG_LINK_HPP

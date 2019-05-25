//
// Created by zjhz-zyt 
//

#ifndef BTREE_BPTREE_HPP
#define BTREE_BPTREE_HPP
#include <iostream>
#include <functional>
#include <cstring>
#include <cmath>
#include "exception.hpp"
constexpr off_t invalid_off = 0xdeadbeef;
//
template <class key_t, class value_t, size_t node_size = 4096, size_t poolsize = 23, class Compare = std::less<key_t>>
class bptree{
    const char tree_t = '0';
    const char block_t = '1';
    //const char blank_t = '2';
private:
    off_t fa_link[100];
    size_t fa_link_pos[100];
    int pfa_link;

    size_t alsize;
    off_t blank_end;
    off_t blank_begin;
    off_t  head;
    off_t  tail;
    off_t  root;

    FILE * file;
    char * filename;

    struct tnode{
        //key_t key;
        off_t pos;   //应该可以删去
        size_t sz = 0;
        tnode( off_t pos = invalid_off, size_t sz = invalid_off)
                :pos(pos),sz(sz){}
    };
    struct block{
        off_t pos;
        size_t sz;
        off_t next;
        block(off_t pos = invalid_off, size_t sz = invalid_off, off_t next = invalid_off)
                :pos(pos), sz(sz), next(next){}
    };
    enum State{
        no_used, no_changed, changed
    };
    struct Bufferpool{
        State state = no_used;
        char buffer[node_size + sizeof(key_t) + sizeof(value_t)];
    }bufferpool[poolsize];


    char *buffer;

    const size_t tnode_max;
    const size_t block_max;
    const size_t tnode_min;
    const size_t block_min;
    Compare cmp = Compare();
    bool equal(const key_t &a, const key_t &b) {
        return !cmp(a, b) && !cmp(b, a);
    }


private:
    void write_out(off_t i){
        if(*fetcht(bufferpool[i].buffer) == tree_t){
            fseek(file, load_tnode(bufferpool[i].buffer)->pos, SEEK_SET);
            fwrite(bufferpool[i].buffer, node_size, 1, file);
        }else{
            fseek(file, load_block(bufferpool[i].buffer)->pos, SEEK_SET);
            fwrite(bufferpool[i].buffer, node_size, 1, file);
        }
        fflush(file);
    }
    void read_in(off_t i){
        buffer = bufferpool[i%poolsize].buffer;
        if(fetch_pos(buffer) != i && bufferpool[i%poolsize].state == changed){
            write_out(i%poolsize);
            fseek(file, i, SEEK_SET);
            fread(buffer, 1, node_size, file);
            bufferpool[i%poolsize].state = no_changed;
        }else if(fetch_pos(buffer) != i || bufferpool[i%poolsize].state == no_used){
            fseek(file, i, SEEK_SET);
            fread(buffer, 1, node_size, file);
            bufferpool[i%poolsize].state = no_changed;
        }
    }
    off_t fetch_pos(char *tbuffer){
        if(*fetcht(tbuffer) == tree_t)
            return load_tnode(tbuffer)->pos;
        else
            return load_block(tbuffer)->pos;
    }
    tnode* load_tnode(char *tbuffer){
        return (tnode*)(tbuffer + sizeof(char));
    }
    block* load_block(char *tbuffer){
        return (block*)(tbuffer + sizeof(char));
    }
    char* fetcht(char *tbuffer){
        return tbuffer;
    }
    //0 - sz-1
    key_t * fetchk_b(size_t n, char *tbuffer) {
        return (key_t *)(tbuffer + (sizeof(key_t) + sizeof(value_t))*n + sizeof(value_t) + sizeof(char) + sizeof(block));
    }
    value_t * fetchv_b(size_t n, char *tbuffer) {
        return (value_t *)(tbuffer + (sizeof(key_t) + sizeof(value_t))*n + sizeof(char) + sizeof(block));
    }
    //0 - sz-1
    key_t * fetchk_t(size_t n, char *tbuffer) {
        return (key_t *)(tbuffer + (sizeof(off_t) + sizeof(key_t))*n + sizeof(off_t) + sizeof(char) + sizeof(tnode));
    }
    //0 - sz
    off_t * fetchv_t(size_t n, char *tbuffer) {
        return (off_t *)(tbuffer + (sizeof(off_t) + sizeof(key_t))*n + sizeof(char) + sizeof(tnode));
    }

    off_t new_node(){
        off_t p = blank_begin;
        if(blank_begin == blank_end){
            blank_end += node_size;
            blank_begin += node_size;
            return p;
        }else{
            fseek(file, blank_begin, SEEK_SET);
            fread(&blank_begin, sizeof(off_t), 1, file);
            return p;
        }
    }
    //申请空节点，未写入外存
    inline block new_block(off_t sz = 0, off_t pos = invalid_off, off_t next = invalid_off){
        pos = new_node();
        return block(pos, sz, next);
    };
    inline tnode new_tnode(off_t sz = 0, off_t pos = invalid_off){
        pos = new_node();
        return tnode(pos, sz);
    }

    //提供给tnode的search
    size_t  t_binary_search(const key_t &key, const size_t &sz){
        size_t l = 0, r = sz, mid;
        while(l < r){
            mid = (l + r)/2;
            if(*fetchk_t(mid, buffer) <= key)
                l = mid + 1;
            else
                r = mid;
        }
        return l;
    }
    //提供给block的search
    size_t b_binary_search(const key_t &key, const size_t &sz){
        size_t l = 0, r = sz, mid;
        while(l < r){
            mid = (l + r)/2;
            if(*fetchk_b(mid, buffer) < key)
                l = mid + 1;
            else
                r = mid;
        }
        return r;
    }

//得到block且文件读入block,同时更新falink,返回缓存中block引用,结尾无保护
    block &find_block(const key_t &key){
        char c_type;
        tnode c_tnode;
        block c_block;
        pfa_link = 0;
        fa_link[0] = root;

        read_in(root);

        while(*fetcht(buffer) == tree_t){
            fa_link_pos[pfa_link] = t_binary_search(key, load_tnode(buffer)->sz);
            pfa_link++;
            fa_link[pfa_link] = *fetchv_t(fa_link_pos[pfa_link-1], buffer);

            read_in(fa_link[pfa_link]);
        }
        return *load_block(buffer);
    }
    //读入buffer,叶内插入，未改变外存
    void insert_in_leaf(block &c, const key_t &key, const value_t &v){
        size_t x = b_binary_search(key, c.sz);
        for(size_t i = c.sz; i > x; i--){
            *fetchk_b(i, buffer) = *fetchk_b(i-1, buffer);
            *fetchv_b(i, buffer) = *fetchv_b(i-1, buffer);
        }
        c.sz++;
        *fetchk_b(x, buffer) = key;
        *fetchv_b(x, buffer) = v;
        bufferpool[c.pos%poolsize].state = changed;
    }
    void insert_in_parent(tnode &P, const key_t &key, const off_t &N2){
        for(size_t i = P.sz; i > fa_link_pos[pfa_link]; i--){
            *fetchk_t(i, buffer) = *fetchk_t(i-1, buffer);
            *fetchv_t(i+1, buffer) = *fetchv_t(i, buffer);
        }
        P.sz++;
        *fetchk_t(fa_link_pos[pfa_link], buffer) = key;
        *fetchv_t(fa_link_pos[pfa_link]+1, buffer) = N2;
        bufferpool[P.pos%poolsize].state = changed;
    }


    void insert_parent(const off_t N, const key_t key, const off_t N2){
        if(N == root){
            tnode R = new_tnode(1);
            if(bufferpool[R.pos%poolsize].state == changed)
                write_out(R.pos%poolsize);
            buffer = bufferpool[R.pos%poolsize].buffer;
            memcpy(buffer, &tree_t, sizeof(char));
            memcpy(buffer + sizeof(char), &R, sizeof(tnode));
            memcpy(buffer + sizeof(char) + sizeof(tnode), &N, sizeof(off_t));
            memcpy(buffer + sizeof(char) + sizeof(tnode) + sizeof(off_t), &key, sizeof(key));
            memcpy(buffer + sizeof(char) + sizeof(tnode) + sizeof(off_t) + sizeof(key), &N2, sizeof(off_t));
            bufferpool[R.pos%poolsize].state = changed;

            root = R.pos;
        }else{
            pfa_link--;
            read_in(fa_link[pfa_link]);
            tnode &P = *load_tnode(buffer);
            if(P.sz < tnode_max){
                insert_in_parent(P, key, N2);
            }else{
                insert_in_parent(P, key, N2);

                tnode P2 = new_tnode(P.sz/2);
                P.sz -= (P2.sz+1);

                if((P.pos % poolsize) != (P2.pos % poolsize)){
                    if(bufferpool[P2.pos%poolsize].state == changed)
                        write_out(P2.pos%poolsize);
                    memcpy(bufferpool[P2.pos%poolsize].buffer, &tree_t, sizeof(char));
                    memcpy(bufferpool[P2.pos%poolsize].buffer + sizeof(char), &P2, sizeof(tnode));
                    memcpy(bufferpool[P2.pos%poolsize].buffer + sizeof(char) + sizeof(tnode),
                            buffer + sizeof(char) +sizeof(tnode) + (sizeof(key_t) + sizeof(off_t))*(P.sz+1),
                           (sizeof(key_t) + sizeof(off_t))*P2.sz +sizeof(off_t));
                    bufferpool[P2.pos%poolsize].state = changed;
                }else{
                    fseek(file, P2.pos, SEEK_SET);
                    fwrite(&tree_t, sizeof(char), 1, file);
                    fwrite(&P2, sizeof(tnode), 1, file);
                    fwrite(buffer + sizeof(char) + sizeof(tnode) + (sizeof(key_t) + sizeof(off_t))*(P.sz+1), (sizeof(key_t) + sizeof(off_t))*P2.sz +
                    sizeof(off_t), 1, file);
                    fflush(file);
                }

                insert_parent(P.pos, *fetchk_t(P.sz, buffer), P2.pos);
            }
        }
    }

    void remove_leaf(block &c, const key_t &key){
        size_t x = b_binary_search(key, c.sz);
        if(*fetchk_b(x) != key)
            throw not_found();
        for(size_t i = x; i < c.sz-1; i++){
            *fetchk_b(i) = *fetchk_b(i+1);
            *fetchv_b(i) = *fetchv_b(i+1);
        }
        c.sz--;
        bufferpool[c.pos%poolsize].state = changed;
    }
public:
    bptree(const char * fname)
            :tnode_max((node_size - sizeof(tnode) - sizeof(char) - sizeof(off_t))/ (sizeof(off_t) + sizeof(key_t))),
             block_max((node_size - sizeof(block) - sizeof(char))/ (sizeof(value_t) + sizeof(key_t))),
             tnode_min(ceil((tnode_max+1)/2)-1), block_min(ceil(block_max/2)){

        /*file = fopen(fname, "rb+");
        filename = new char[strlen(fname) + 1];
        strcpy(filename, fname);
        if(!file){
            file = fopen(fname, "wb+");

            alsize = 0;
            blank_begin = 0;
            blank_end = 0;
            head = tail = root = invalid_off;
            fseek(file, 0, SEEK_SET);//??????????
        }*/
        file = fopen(fname, "wb+");
        filename = new char[strlen(fname) + 1];
        strcpy(filename, fname);
        alsize = 0;
        blank_begin = 0;
        blank_end = 0;
        head = tail = root = invalid_off;
        fseek(file, 0, SEEK_SET);//??????????
    }
    //函数开始保护，结尾无保护
    void insert(const key_t &key, const value_t &v){
        if(alsize == 0){
            block p = new_block(1);

            if(bufferpool[p.pos%poolsize].state == changed)
                write_out(p.pos%poolsize);
            buffer = bufferpool[p.pos%poolsize].buffer;
            memcpy(buffer, &block_t, sizeof(char));
            memcpy(buffer + sizeof(char), &p, sizeof(block));
            memcpy(buffer + sizeof(char) + sizeof(block), &v, sizeof(value_t));
            memcpy(buffer + sizeof(char) + sizeof(block) + sizeof(value_t), &key, sizeof(key_t));
            bufferpool[p.pos%poolsize].state = changed;

            alsize++;
            head = tail = root = p.pos;
            return;
        }else{//保证过程运行期间不能改动buffer
            block &L = find_block(key);
            if(L.sz < block_max){
                insert_in_leaf(L, key, v);
                alsize++;
            }else{
                insert_in_leaf(L, key, v);
                block L2 = new_block(L.sz/2);
                L.sz -= L2.sz;
                L2.next = L.next;
                L.next = L2.pos;
                if(L.pos == tail)
                    tail = L2.pos;
                alsize++;

                if((L2.pos % poolsize) != (L.pos % poolsize)){
                    if(bufferpool[L2.pos%poolsize].state == changed)
                        write_out(L2.pos%poolsize);
                    memcpy(bufferpool[L2.pos%poolsize].buffer, &block_t, sizeof(char));
                    memcpy(bufferpool[L2.pos%poolsize].buffer + sizeof(char), &L2, sizeof(block));
                    memcpy(bufferpool[L2.pos%poolsize].buffer + sizeof(char) + sizeof(block),
                            buffer + (sizeof(value_t) +sizeof(key_t)) * L.sz + sizeof(char) + sizeof(block),
                            (sizeof(value_t) + sizeof(key_t)) * L2.sz);
                    bufferpool[L2.pos%poolsize].state = changed;
                }else{
                    fseek(file, L2.pos, SEEK_SET);
                    fwrite(&block_t, sizeof(char), 1, file);
                    fwrite(&L2, sizeof(block), 1, file);
                    fwrite(buffer + (sizeof(value_t) + sizeof(key_t)) * L.sz + sizeof(char) + sizeof(block),(sizeof(value_t) + sizeof(key_t)) * L2.sz, 1, file);
                    fflush(file);
                }

                insert_parent(L.pos, *fetchk_b(L.sz, buffer), L2.pos);
            }
        }
    }

    value_t find(const key_t &key, const value_t & no = value_t()){
        if(root == invalid_off)
            return no;
        block &c = find_block(key);
        size_t x = b_binary_search(key, c.sz);
        if(*fetchk_b(x, buffer) != key){
            return no;
        }else{
            return *fetchv_b(x, buffer);
        }
    }
    //函数开始有保护，结束无保护，fwrite结束
    void modify(const key_t &key, const value_t &v){
        if(root == invalid_off)
            throw not_found();
        block &c = find_block(key);
        size_t x = b_binary_search(key, c.sz);
        if(*fetchk_b(x, buffer) != key){
           throw not_found();
        }else{
            /**fetchv_b(x, buffer) = v;
            fseek(file, c.pos + (sizeof(key_t) + sizeof(value_t))*x + sizeof(char) + sizeof(block),SEEK_SET);
            fwrite(&v, sizeof(value_t), 1, file);*/
            *fetchv_b(x, buffer) = v;
            bufferpool[c.pos%poolsize].state = changed;
        }
    }
    void init(){
        alsize = 0;
        blank_begin = blank_end = 0;
        head = tail = root = invalid_off;
        fseek(file, 0, SEEK_SET);
        for(size_t i = 0; i < poolsize; i++){
            bufferpool[i].state = no_used;
        }
    }

    /*void remove(const key_t &key){
        if(root == invalid_off)
            throw not_found();
        block &c = find_block(key);
        remove_leaf(c, key);
        if(c.pos == root && c.sz == 0){
            init();
        }
        if(c.sz < block_min){
            pfa_link--;
            read_in(fa_link[pfa_link]);
            if()
        }
    }*/

    void traverse(){
        off_t p = head;
        fflush(file);
        while(p != tail){
            read_in(p);
            block& m = *load_block(buffer);
            std::cout << "The Node" << " sz " << m.sz << " pos " << m.pos  << " next " << m.next;
            for(size_t i = 0; i < m.sz; i++)
                std::cout << " ("<<(*fetchk_b(i, buffer)) << ", " << (*fetchv_b(i, buffer)) << ") ";
            std::cout << std::endl;
            p = m.next;
        }
    }
};
#endif //BTREE_BPTREE_HPP











#endif //UNTITLED_BPLUS_HPP

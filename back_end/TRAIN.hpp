//
// Created by yezhuoyang on 2019-04-30.
//
#ifndef SJTU_TRAIN_HPP
#define SJTU_TRAIN_HPP
#include "TIME.hpp"
#include <string.h>
#include<iostream>
#include<stdio.h>
namespace sjtu {
    struct Station{
        char loc[20];
        Time arrive_time,start_time,stop_time;
        double price[5];
        int type_num;
        Station& operator=(const Station& rhs){
            if(&rhs==this) return *this;
            arrive_time=rhs.arrive_time;
            start_time=rhs.start_time;
            stop_time=rhs.stop_time;
            type_num=rhs.type_num;
            for(int i=0;i<type_num;++i){
                price[i]=rhs.price[i];
            }
            strcpy(loc,rhs.loc);
            return *this;
        }
        Station(const Station& rhs){
            arrive_time=rhs.arrive_time;
            start_time=rhs.start_time;
            stop_time=rhs.stop_time;
            type_num=rhs.type_num;
            for(int i=0;i<type_num;++i){
                price[i]=rhs.price[i];
            }
            strcpy(loc,rhs.loc);
        }
        Station(){type_num=0;}
    };
    std::istream& operator>>(std::istream &is,Station& S){
        char tmp[15];
        is>>S.loc>>S.arrive_time>>S.start_time>>S.stop_time;
        for(int i=0;i<S.type_num;++i){
            is>>tmp;
            S.price[i]=strtod(tmp+2,NULL);
        }
        return is;
    }
    struct Trainkey{
            char train_id[10];
            Trainkey(const char*tid){
                strcpy(train_id,tid);
            }
            Trainkey(const Trainkey& rhs){
                strcpy(train_id,rhs.train_id);
            }
            Trainkey& operator=(const Trainkey& rhs){
                strcpy(train_id,rhs.train_id);
                return *this;
            }
            Trainkey()= default;
    };
    class compare_train{
    public:
        bool operator () (const Trainkey& T1,const Trainkey& T2) const {
            return strcmp(T1.train_id,T2.train_id)<0;
        }
    };
    struct Train {
            char name[10];
            char catalog[10];
            int station_num;
            /*
             * Whether the train is already forsale or not
             */
            bool For_sale;
            int price_num;
            char price_name[5][10];
            Train& operator=(const Train& rhs){
                if(&rhs==this) return *this;
                strcpy(name,rhs.name);
                strcpy(catalog,rhs.catalog);
                station_num=rhs.station_num;
                For_sale=rhs.For_sale;
                price_num=rhs.price_num;
                for(int i=0;i<price_num;++i){
                    strcpy(price_name[i],rhs.price_name[i]);
                }
                return *this;
            }
            Train(const Train& rhs){
                strcpy(name,rhs.name);
                strcpy(catalog,rhs.catalog);
                station_num=rhs.station_num;
                For_sale=rhs.For_sale;
                price_num=rhs.price_num;
                for(int i=0;i<price_num;++i){
                    strcpy(price_name[i],rhs.price_name[i]);
                }
            }
            Train(){station_num=0;For_sale=false;price_num=0;}
    };

    /*
     * The ticket structure to be stored in ticket bptree;
     * All train that passed by loc.
     */
    struct Ticketkey{
          char loc[10];
          const Date D;
          char catalog[10];
          Ticketkey(const char* lc,const char*cat,const Date d):D(d){
               strcpy(loc,lc);
               strcpy(catalog,cat);
          }
    };



    struct Ticket{
            /*
             * The remaining value of tickets from date june 1st to june thirtieth.
             * The kth column denotes the number of tickets of the kth price.
             */
            char catalog[10];
            char loc1[10];
            char loc2[10];
            int remain[5][30];
            Time arrive_time,start_time;
            int price_num;
            char price_name[5][10];
            double price[5];
            Ticket(){
                price_num=0;
                for(int i=0;i<5;++i)
                    for(int j=0;j<30;++j)
                        remain[i][j]=2000;
            }
            Ticket& operator=(const Ticket& rhs){
                if(&rhs==this) return *this;
                arrive_time=rhs.arrive_time;
                start_time=rhs.start_time;
                price_num=rhs.price_num;
                strcpy(catalog,rhs.catalog);
                strcpy(loc1,rhs.loc1);
                strcpy(loc2,rhs.loc2);
                for(int i=0;i<price_num;++i){
                    strcpy(price_name[i],rhs.price_name[i]);
                    price[i]=rhs.price[i];
                    for(int j=0;j<30;++j){
                        remain[i][j]=rhs.remain[i][j];
                    }
                }
                return *this;
            }
            Ticket(const Ticket& rhs){
                arrive_time=rhs.arrive_time;
                start_time=rhs.start_time;
                price_num=rhs.price_num;
                strcpy(catalog,rhs.catalog);
                strcpy(loc1,rhs.loc1);
                strcpy(loc2,rhs.loc2);
                for(int i=0;i<price_num;++i){
                    strcpy(price_name[i],rhs.price_name[i]);
                    price[i]=rhs.price[i];
                    for(int j=0;j<30;++j){
                        remain[i][j]=rhs.remain[i][j];
                    }
                }
            }
            /*
             * When a train is ready for sale, copy all trivial information.
             */
            void copy(const Train& T,const char* id){
                    price_num=T.price_num;
                    strcpy(catalog,T.catalog);
                    for(int i=0;i<price_num;++i){
                        strcpy(price_name[i],T.price_name[i]);
                    }
            }
            void copy_price(const Station& S){
                for(int i=0;i<price_num;++i){
                    price[i]=S.price[i];
                }
            }
    };
    /*
     * The ticket information to be stored in User bptree;
     */

    struct Orderkey{
            int Uid;
            Date D;
            char catalog[10];
    };
    class compare_order{
    public:
        bool operator () (const Orderkey& T1,const Orderkey& T2) const {
            {
                if(T1.Uid<T2.Uid) return true;
                else if(T1.Uid>T2.Uid) return false;
                if(T1.D<T2.D) return true;
                else if(T1.D>T2.D) return false;
                if(strcmp(T1.catalog,T2.catalog)<0) return true;
                return false;
            }
        }
    };
    struct Order{
        char loc1[20],loc2[20];
        char train_id[10];
        // The kth column denotes the number of tickets of the kth price.
        int  remain[5];
        // Total number of such tickets. If sum==0, the ticket is deleted.
        int  sum;
        /*
         * The remaining value of tickets from date june 1st to june thirtieth.
         */
        Time arrive_time,start_time;
        int price_num;
        char price_name[5][10];
        double price[5];
        /*
         * The list of price value.
         */
        Order(){
            sum=0;
            price_num=0;
            for(int i=0;i<5;++i) remain[i]=0;
        }
        Order(const Order& rhs){
            strcpy(loc1,rhs.loc1);
            strcpy(loc2,rhs.loc2);
            strcpy(train_id,rhs.train_id);
            sum=rhs.sum;
            arrive_time=rhs.arrive_time;
            start_time=rhs.start_time;
            price_num=rhs.price_num;
            for(int i=0;i<price_num;++i){
                strcpy(price_name[i],rhs.price_name[i]);
                price[i]=rhs.price[i];
                remain[i]=rhs.remain[i];
            }
        }
        Order& operator=(const Order &rhs){
            if(&rhs==this) return *this;
            strcpy(loc1,rhs.loc1);
            strcpy(loc2,rhs.loc2);
            strcpy(train_id,rhs.train_id);
            sum=rhs.sum;
            arrive_time=rhs.arrive_time;
            start_time=rhs.start_time;
            price_num=rhs.price_num;
            for(int i=0;i<price_num;++i){
                strcpy(price_name[i],rhs.price_name[i]);
                price[i]=rhs.price[i];
                remain[i]=rhs.remain[i];
            }
            return *this;
        }
        /*
         * Set the information when a new ticket is bought.
         */
        int buy(Ticket& T,const char *tid,const char* cat,const Date& Da,const char* ticket_kind,const int& num,const char*lc1,const char*lc2){
            return 1;
        }
    };
    /*
     * Print the information when a query for order occurs.
     */
    std::ostream& operator<<(std::ostream& os,const Order& T){
        os<<T.train_id<<" "<<T.loc1<<" "<<" "<<T.start_time<<" "<<T.loc2<<" "<<" "<<T.arrive_time<<" ";
        for(int i=0;i<T.price_num;++i){
            os<<T.price_name[i]<<" "<<T.remain[i]<<" "<<T.price[i]<<" ";
        }
        return os;
    }

    /*
     * The function print the information of a single station.
     */
    std::ostream &operator<<(std::ostream &os,const Station &S){
        os<<S.loc<<" "<<S.arrive_time<<" "<<S.start_time<<" "<<S.stop_time<<" ";
        for(int i=0;i<S.type_num;++i){
            os<<"¥"<<S.price[i]<<" ";
        }
        return os;
    }




}





#endif //SJTU_TRAIN_HPP

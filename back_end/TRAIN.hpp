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
            Station station[5];
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
                for(int i=0;i<station_num;++i){
                    station[i]=rhs.station[i];
                }
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
                for(int i=0;i<station_num;++i){
                    station[i]=rhs.station[i];
                }
                for(int i=0;i<price_num;++i){
                    strcpy(price_name[i],rhs.price_name[i]);
                }
            }
            Train(){station_num=0;For_sale=false;price_num=0;}
    };



    struct Ticketkey{
        char loc1[20],loc2[20];
        char train_id[10];
        char catalog[10];
        Ticketkey(const char* lc1,const char* lc2,const char* tid){
            strcpy(loc1,lc1);
            strcpy(loc2,lc2);
            strcpy(train_id,tid);
        }
        Ticketkey(const char* lc1,const char* lc2,const char* tid,const char* cat){
            strcpy(loc1,lc1);
            strcpy(loc2,lc2);
            strcpy(train_id,tid);
            strcpy(catalog,cat);
        }
        Ticketkey(const char*tid,const char* cata){
            strcpy(train_id,tid);
            strcpy(catalog,cata);
        }
        Ticketkey& operator=(const Ticketkey& rhs){
            if(&rhs==this) return *this;
            strcpy(loc1,rhs.loc1);
            strcpy(loc2,rhs.loc2);
            strcpy(train_id,rhs.train_id);
            strcpy(catalog,rhs.catalog);
            return *this;
        }
        Ticketkey(const Ticketkey& rhs){
            strcpy(loc1,rhs.loc1);
            strcpy(loc2,rhs.loc2);
            strcpy(train_id,rhs.train_id);
            strcpy(catalog,rhs.catalog);
        }
        Ticketkey()= default;
        void set_loc1(const char* lc1){
            strcpy(loc1,lc1);
        }
        void set_loc2(const char* lc2){
            strcpy(loc2,lc2);
        }
    };


    std::ostream& operator<<(std::ostream& os,const Ticketkey& key){
            os<<key.loc1<<" "<<key.loc2<<" "<<key.train_id<<" "<<key.catalog;
            return os;
    }


    class compare_ticket1{
    public:
        bool operator () (const Ticketkey& T1,const Ticketkey& T2) const {
            {
                 if(strcmp(T1.loc1,T2.loc1)<0) return true;
                 if(strcmp(T1.loc1,T2.loc1)>0) return false;
                 if(strcmp(T1.loc2,"\0")!=0&&strcmp(T2.loc2,"\0")!=0){
                    if(strcmp(T1.loc2,T2.loc2)<0) return true;
                    if(strcmp(T1.loc2,T2.loc2)>0) return false;
                 }
                 /*
                  * In a buy ticket command, catalog is set as "\0" and the compare function will neglect
                  * the effect of catalog.
                  */
                 if(strcmp(T1.catalog,"\0")!=0&&strcmp(T2.catalog,"\0")!=0)
                 {
                     if(strcmp(T1.catalog,T2.catalog)<0) return true;
                     if(strcmp(T1.catalog,T2.catalog)>0) return false;
                 }
                 if(strcmp(T1.train_id,T2.train_id)<0) return true;
                 if(strcmp(T1.train_id,T2.train_id)>0) return false;
                 return false;
            }
        }
    };



    class compare_ticket2{
    public:
        bool operator () (const Ticketkey& T1,const Ticketkey& T2) const {
            {
                if(strcmp(T1.loc2,T2.loc2)<0) return true;
                if(strcmp(T1.loc2,T2.loc2)>0) return false;
                if(strcmp(T1.loc1,"\0")!=0&&strcmp(T2.loc1,"\0")!=0){
                    if(strcmp(T1.loc1,T2.loc1)<0) return true;
                    if(strcmp(T1.loc1,T2.loc1)>0) return false;
                }
                /*
                 * In a buy ticket command, catalog is set as "\0" and the compare function will neglect
                 * the effect of catalog.
                 */
                if(strcmp(T1.catalog,"\0")!=0&&strcmp(T2.catalog,"\0")!=0)
                {
                    if(strcmp(T1.catalog,T2.catalog)<0) return true;
                    if(strcmp(T1.catalog,T2.catalog)>0) return false;
                }
                if(strcmp(T1.train_id,T2.train_id)<0) return true;
                if(strcmp(T1.train_id,T2.train_id)>0) return false;
                return false;
            }
        }
    };

    /*
     * The ticket structure to be stored in ticket bptree;
     */
    struct Ticket{
            /*
             * The remaining value of tickets from date june 1st to june thirtieth.
             * The kth column denotes the number of tickets of the kth price.
             */
            char catalog[10];
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
    struct Ticketbought{
        char loc1[20],loc2[20];
        char train_id[10];
        char catalog[10];
        // The kth column denotes the number of tickets of the kth price.
        int  remain[5];
        // Total number of such tickets. If sum==0, the ticket is deleted.
        int  sum;
        /*
         * The remaining value of tickets from date june 1st to june thirtieth.
         */
        Date D;
        Time arrive_time,start_time;
        int price_num;
        char price_name[5][10];
        double price[5];
        /*
         * The list of price value.
         */
        Ticketbought(){
            sum=0;
            price_num=0;
            for(int i=0;i<5;++i) remain[i]=0;
        }
        Ticketbought(const Ticketbought& rhs){
            strcpy(loc1,rhs.loc1);
            strcpy(loc2,rhs.loc2);
            strcpy(train_id,rhs.train_id);
            strcpy(catalog,rhs.catalog);
            sum=rhs.sum;
            D=rhs.D;
            arrive_time=rhs.arrive_time;
            start_time=rhs.start_time;
            price_num=rhs.price_num;
            for(int i=0;i<price_num;++i){
                strcpy(price_name[i],rhs.price_name[i]);
                price[i]=rhs.price[i];
                remain[i]=rhs.remain[i];
            }
        }


        Ticketbought& operator=(const Ticketbought &rhs){
            if(&rhs==this) return *this;
            strcpy(loc1,rhs.loc1);
            strcpy(loc2,rhs.loc2);
            strcpy(train_id,rhs.train_id);
            strcpy(catalog,rhs.catalog);
            sum=rhs.sum;
            D=rhs.D;
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
            price_num=T.price_num;
            strcpy(train_id,tid);
            strcpy(catalog,cat);
            strcpy(loc1,lc1);
            strcpy(loc2,lc2);
            for(int i=0;i<price_num;++i){
                strcpy(price_name[i],T.price_name[i]);
                price[i]=T.price[i];
            }
            arrive_time=T.arrive_time;
            start_time=T.start_time;
            sum=num;
            D=Da;
            for(int i=0;i<5;++i) {
                if(strcmp(price_name[i],ticket_kind)==0){
                    if(T.remain[i][D.pos]<num) return 0;
                    remain[i]=num;
                    T.remain[i][D.pos]-=num;
                }
                else
                    remain[i]=0;
            }
            return 1;
        }
    };



    /*
     * Print the information when a query for order occurs.
     */
    std::ostream& operator<<(std::ostream& os,const Ticketbought& T){
        os<<T.train_id<<" "<<T.loc1<<" "<<T.D<<" "<<T.start_time<<" "<<T.loc2<<" "<<T.D<<" "<<T.arrive_time<<" ";
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

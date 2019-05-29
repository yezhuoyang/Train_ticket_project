//
// Created by yezhuoyang on 2019-04-30.
//
#ifndef SJTU_TRAIN_HPP
#define SJTU_TRAIN_HPP
#include "TIME.hpp"
#include "link.hpp"
#include "link.hpp"
#include "constant.h"
namespace sjtu {
    double transform_to_double(const char* tmp){
        int pos=0,dig=-1;
        while(tmp[pos]>'9'||tmp[pos]<'0') ++pos;
        double num=0;
        while(tmp[pos]){
            if(tmp[pos]=='.')
                dig=pos;
            else
                num=10*num+(tmp[pos]-'0');
            ++pos;
        }
        num=num*pow(10,-(pos-dig-1));
        return num;
    }
    struct Station{
        char loc[LOCSIZE];
        Time arrive_time,start_time,stop_time;
        double price[PRICENUM];
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
            S.price[i]=transform_to_double(tmp);
        }
        return is;
    }
    struct Trainkey{
            char train_id[IDSIZE];
            Trainkey(const char*tid){
                strcpy(train_id,tid);
            }
            Trainkey(const Trainkey& rhs){
                strcpy(train_id,rhs.train_id);
            }
            Trainkey& operator=(const Trainkey& rhs){
                if(&rhs==this) return *this;
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
            char name[TRAINNAME];
            char catalog[CATSIZE];
            int station_num;
            //存放Station在文件中存放的位置
            block stblock;
            /*
             * Whether the train is already forsale or not
             */
            bool For_sale;
            int price_num;
            char price_name[PRICENUM][PRICESIZE];
            bool operator==(const Train&rhs){
                return (strcmp(name,rhs.name)==0);
            }
            Train& operator=(const Train& rhs){
                if(&rhs==this) return *this;
                strcpy(name,rhs.name);
                strcpy(catalog,rhs.catalog);
                station_num=rhs.station_num;
                For_sale=rhs.For_sale;
                price_num=rhs.price_num;
                stblock=rhs.stblock;
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
                stblock=rhs.stblock;
                for(int i=0;i<price_num;++i){
                    strcpy(price_name[i],rhs.price_name[i]);
                }
            }
            Train(){
                strcpy(name,"\0");
                station_num=0;
                For_sale=false;
                price_num=0;
            }
    };
    /*
     * The ticket structure to be stored in ticket bptree;
     * All train that passed by loc.
     */
    struct Ticketkey{
          char loc1[LOCSIZE];
          char loc2[LOCSIZE];
          char catalog[CATSIZE];
          char tid[IDSIZE];
          Ticketkey(const char*id,const char* lc,const char* lc2,const char*cat){
               strcpy(tid,id);
               strcpy(loc1,lc);
               strcpy(loc2,lc2);
               strcpy(catalog,cat);
          }
          Ticketkey& operator=(const Ticketkey& rhs){
              if(&rhs==this) return *this;
              strcpy(loc1,rhs.loc1);
              strcpy(loc2,rhs.loc2);
              strcpy(catalog,rhs.catalog);
              strcpy(tid,rhs.tid);
              return *this;
          }
          Ticketkey(const Ticketkey& rhs){
              strcpy(loc1,rhs.loc1);
              strcpy(loc2,rhs.loc2);
              strcpy(catalog,rhs.catalog);
              strcpy(tid,rhs.tid);
          }
          Ticketkey()= default;
    };
    class compare_ticket{
    public:
        bool operator () (const Ticketkey& T1,const Ticketkey& T2) const {
               if(strcmp(T1.loc1,"\0")!=0&&strcmp(T2.loc1,"\0")!=0){
                   if(strcmp(T1.loc1,T2.loc1)<0) return true;
                   if(strcmp(T1.loc1,T2.loc1)>0) return false;
               }
               if(strcmp(T1.loc2,"\0")!=0&&strcmp(T2.loc2,"\0")!=0){
                   if(strcmp(T1.loc2,T2.loc2)<0) return true;
                   if(strcmp(T1.loc2,T2.loc2)>0) return false;
               }
               if(strcmp(T1.catalog,"\0")!=0&&strcmp(T2.catalog,"\0")!=0){
                   if(strcmp(T1.catalog,T2.catalog)<0) return true;
                   if(strcmp(T1.catalog,T2.catalog)>0) return false;
               }
               if(strcmp(T1.tid,"\0")!=0&&strcmp(T2.tid,"\0")!=0){
                   if(strcmp(T1.tid,T2.tid)<0) return true;
               }
               return false;
        }
    };

    struct Ticket{
            /*
             * The remaining value of tickets from date june 1st to june thirtieth.
             * The kth column denotes the number of tickets of the kth price.
             */
            //char catalog[CATSIZE];
            //char loc1[LOCSIZE];
            //char loc2[LOCSIZE];
            int remain[PRICENUM][DAYNUM];
            Time arrive_time,start_time;
            int price_num;
            char price_name[PRICENUM][PRICESIZE];
            double price[PRICENUM];
            bool operator==(const Ticket& rhs){
                return price_num==rhs.price_num;
            }
            Ticket(){
                price_num=0;
                for(int i=0;i<PRICENUM;++i)
                    for(int j=0;j<DAYNUM;++j)
                         remain[i][j]=2000;
            }
            Ticket& operator=(const Ticket& rhs){
                if(&rhs==this) return *this;
                arrive_time=rhs.arrive_time;
                start_time=rhs.start_time;
                price_num=rhs.price_num;
                for(int i=0;i<price_num;++i){
                    strcpy(price_name[i],rhs.price_name[i]);
                    price[i]=rhs.price[i];
                    for(int j=0;j<DAYNUM;++j){
                        remain[i][j]=rhs.remain[i][j];
                    }
                }
                return *this;
            }
            Ticket(const Ticket& rhs){
                arrive_time=rhs.arrive_time;
                start_time=rhs.start_time;
                price_num=rhs.price_num;
                for(int i=0;i<price_num;++i){
                    strcpy(price_name[i],rhs.price_name[i]);
                    price[i]=rhs.price[i];
                    for(int j=0;j<DAYNUM;++j){
                        remain[i][j]=rhs.remain[i][j];
                    }
                }
            }
            /*
             * When a train is ready for sale, copy all trivial information.
             */
            void copy(const Train& T){
                    price_num=T.price_num;
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
            char catalog[CATSIZE];
            char train_id[IDSIZE];
            Orderkey(const int&id,const Date& d,const char* cat,const char*tid){
                Uid=id;D=d;
                strcpy(catalog,cat);
                strcpy(train_id,tid);
            }
            Orderkey()= default;
            Orderkey& operator=(const Orderkey& rhs){
                if(&rhs==this) return *this;
                Uid=rhs.Uid;
                D=rhs.D;
                strcpy(catalog,rhs.catalog);
                strcpy(train_id,rhs.train_id);
                return *this;
            }
            Orderkey(const Orderkey& rhs){
                Uid=rhs.Uid;
                D=rhs.D;
                strcpy(catalog,rhs.catalog);
                strcpy(train_id,rhs.train_id);
            }
    };
    class compare_order{
    public:
        bool operator () (const Orderkey& T1,const Orderkey& T2) const {
            {
                if(T1.Uid<T2.Uid) return true;
                if(T1.Uid>T2.Uid) return false;
                if(T1.D<T2.D) return true;
                if(T1.D>T2.D) return false;
                if(strcmp(T1.catalog,"\0")!=0&&strcmp(T2.catalog,"\0")!=0){
                    if(strcmp(T1.catalog,T2.catalog)<0) return true;
                    if(strcmp(T1.catalog,T2.catalog)>0) return false;
                }
                if(strcmp(T1.train_id,"\0")!=0&&strcmp(T2.train_id,"\0")!=0){
                    if(strcmp(T1.train_id,T2.train_id)<0) return true;
                }
                return false;
            }
        }
    };
    struct Order{
        char loc1[LOCSIZE],loc2[LOCSIZE];
        // The kth column denotes the number of tickets of the kth price.
        int  remain[PRICENUM];
        // Total number of such tickets. If sum==0, the ticket is deleted.
        int  sum;
        /*
         * The remaining value of tickets from date june 1st to june thirtieth.
         */
        Time arrive_time,start_time;
        int price_num;
        char price_name[PRICENUM][PRICESIZE];
        double price[PRICENUM];
        /*
         * The list of price value.
         */
        Order(){
            sum=0;
            price_num=0;
            for(int i=0;i<PRICENUM;++i) remain[i]=0;
        }
        Order(const Order& rhs){
            strcpy(loc1,rhs.loc1);
            strcpy(loc2,rhs.loc2);
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
        void copy(const Ticket& T){
            price_num=T.price_num;
            start_time=T.start_time;
            arrive_time=T.arrive_time;
            for(int i=0;i<price_num;++i){
                strcpy(price_name[i],T.price_name[i]);
                price[i]=T.price[i];
            }
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
        os<<" "<<T.loc1<<" "<<" "<<T.start_time<<" "<<T.loc2<<" "<<" "<<T.arrive_time<<" ";
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

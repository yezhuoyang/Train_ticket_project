//
// Created by yezhuoyang on 2019-04-30.
//

#ifndef SJTU_TRAIN_HPP
#define SJTU_TRAIN_HPP
#include "back_end/TIME.hpp"
#include<iostream>



namespace sjtu {
    struct Station{
        char loc[20];
        Time arrive_time,start_time,stop_time;
        char price_list[5][10];
        int type_num;
        Station& operator=(const Station& rhs){
            arrive_time=rhs.arrive_time;
            start_time=rhs.start_time;
            stop_time=rhs.stop_time;
            type_num=rhs.type_num;
            for(int i=0;i<type_num;++i){
                strcpy(price_list[i],rhs.price_list[i]);
            }
            strcpy(loc,rhs.loc);
        }
    };
    /*
     * The class of a Train.
     * The order of Train stored in Disk is based on train_id.
     */
    class Train {
        friend void add_train(Train &T);
        friend void query_train(Train &T);
        friend void put_on_sale(Train& T);
        private:
            char train_id[10];
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
        public:
            bool operator>(const Train& T){
                return strcmp(train_id,T.train_id)>0;
            }
            bool operator<(const Train& T){
                return strcmp(train_id,T.train_id)<0;
            }
            Train& operator=(const Train& rhs){
                strcpy(train_id,rhs.train_id);
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

            /*
             * TODO
             */
            std::ostream &operator<<(std::ostream &os){}


            Train(){station_num=0;For_sale=false;price_num=0;}
    };
    /*
     * Ticket fo sale.
     */
    class Ticket_forsale{
           friend class Ticket_bought;
        private:
            int remain;
            const char loc1[20],loc2[20];
            Time arrive_time,start_time,stop_time;
            const int price_num;
            const char price_name[5][10];
            const char price_list[5][10];
            const Date date;
            const char train_id[10];
            const char catalog[10];
        public:
    };



    /*
     * Ticket that bought by a user.
     */
    class Ticket_bought{
        friend class Ticket_forsale;
        friend class User;
        private:
            char loc1[20],loc2[20];
            char price_type[5];
            char price[5];
            Date date;
            char train_id[10];
            char catalog[10];
        public:


            /*
             * When a user decides to by a for-sale ticket T of price type *name, set the corresponding value.
             */
            void set(Ticket_forsale& T,const char* name){
                strcpy(loc1,T.loc1);
                strcpy(loc2,T.loc2);
                strcpy(train_id,T.train_id);
                strcpy(catalog,T.catalog);
                date=T.date;
                for(int i=0;i<T.price_num;++i){
                     if(strcmp(T.price_name[i],name)==0){
                         strcpy(price,T.price_list[i]);
                         break;
                     }
                }
            }
    };




    /*
     * The function print the information of a single station.
     */
    std::ostream &operator<<(std::ostream &os, Station &S){
        os<<S.loc<<" "<<S.arrive_time<<" "<<S.start_time<<" "<<S.stop_time<<" ";
        for(int i=0;i<S.type_num;++i){
            os<<S.price_list[i]<<" ";
        }
        return os;
    }




    /*
     * The function read the input from istream and
     * add the information to a train T.
     */
    void add_train(Train& T){
        std::cin>>T.train_id>>T.name>>T.catalog>>T.station_num>>T.price_num;
        for(int i=0;i<T.price_num;++i){
            std::cin>>T.price_name[i];
        }
        for(int i=0;i<T.station_num;++i){
            std::cin>>T.station[i].loc>>T.station[i].arrive_time>>T.station[i].start_time>>T.station[i].stop_time;
            T.station[i].type_num=T.price_num;
            for(int j=0;j<T.price_num;++j){
                std::cin>>T.station[i].price_list[j];
            }
        }
    };




    /*
     *  The function print all information of a train T.
    */
    void query_train(Train& T){
        std::cout<<T.train_id<<" "<<T.name<<" "<<T.catalog<<" "<<T.station_num<<" "<<T.price_num;
        for(int i=0;i<T.price_num;++i){
            std::cout<<T.price_name[i]<<" ";
        }
        std::cout<<std::endl;
        for(int i=0;i<T.station_num;++i){
            std::cout<<T.station[i]<<std::endl;
        }
    }



}





#endif //SJTU_TRAIN_HPP

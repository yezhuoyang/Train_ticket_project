//
// Created by yezhuoyang on 2019-05-14.
//

#ifndef SJTU_PROCESSOR_HPP
#define SJTU_PROCESSOR_HPP
#include "TRAIN.hpp"
#include "TIME.hpp"




namespace sjtu{
    extern Bpp_tree<User> User_Bpp;
    extern Bpp_tree<Train> Train_Bpp;
    extern Bpp_tree<Ticket_forsale> Ticket_Bpp;
    extern current_id;
    //TODO
    int Register(const char* name,const char * pass, const char *email,const char* phone){

    }



    /*
     * TODO Add the ticket to the bpp tree.
     */
    void add_ticket(ticket_forsale& t){
            Ticket_Bpp.insert(t);
    }


    extern void put_on_sale(Train& T);




    /*TODO
     * Put the Train T on sale
     * For each
     *
     */
    int put_on_sale(Train& T){
        if(T.For_sale) return 0;
        T.For_sale=true;
        Ticket_forsale tmp;
        for(int i=0;i<T.station_num;++i){


        }



    }



    /*
     * TODO
     */
    void query_transfer(const char*loc1,const char*loc2,const Date& date,const char *catalog){

    }



    /*
     * TODO
     */
    void query_order(const char* id,const Date& date,const char* catelog){

    }


    /*
     * TODO
     */
    void buy_ticket(const char*id,const char* num,const char* train_id,const char* loc1,const char* loc2,const Date& D,const char* ticket_kind){

    }



    /*
     * TODO
     */
    void refund_ticket(const char* id,const char* num,const char* train_id,const char* loc1,const char* loc2,const Date& D,const char *ticket_kind){

    }


    /*
     * TODO
     */
    void modify_train(){

    }


    /*
     * TODO
     */
    void delete_train(){


    }



    /*
     * TODO
     */
    void log_in(const int& id,const char* pass){



    }




}





#endif

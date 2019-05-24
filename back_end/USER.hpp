//
// Created by yezhuoyang on 2019-04-30.
//
#ifndef SJTU_USER_HPP
#define SJTU_USER_HPP
#include <iostream>
#include "TRAIN.hpp"
namespace sjtu {
    struct User {
        /*
         * The privilege of the user.
         * 0 unregistered
         * 1 registered user
         * 2 Administrator
         */
        int privilege;
        char name[40];
        char password[20];
        char phone[20];
        char email[20];
        Ticketbought ticket[10];
        int num_ticket;
        //The number of different kinds of tickets.
        //The number of each kind of ticket.
        User(){num_ticket=0;privilege=0;}
        User(const char* N){strcpy(name,N);}
        User&operator=(const User &U){
            if(&U==this) return *this;
            num_ticket=U.num_ticket;
            privilege=U.privilege;
            strcpy(name,U.name);
            strcpy(password,U.password);
            strcpy(email,U.email);
            strcpy(phone,U.phone);
            for(int i=0;i<num_ticket;++i){
                ticket[i]=U.ticket[i];
            }
            return *this;
        }
        int add_ticket(Ticket& t,const char* train_id,const char* cat,const char* loc1,const char* loc2,const Date& D,const char* ticket_kind,const int& num);
        int modify(const char* Na,const char* pass,const char* em,const char* pho);
        int refund_ticket(Ticket& t,const int& n,const char* train_id,const char* loc1,const char* loc2,const Date& D,const char *price_type);
    };

    /*
     * User buy num tickets. Return 0 if the tickets are not enough.
     * First search the ticket from all ticket that he has already bought.
     * If he hasn't bought such kind of ticket, add it to the ticket list.
     */
    int User::add_ticket(Ticket& t,const char* train_id,const char* cat,const char* loc1,const char* loc2,const Date& D,const char* ticket_kind,const int& num){
        /*
         * Find whether the ticket is already in the user's ticket list.
         */
        bool found=false;
        for(int i=0;i<num_ticket;++i){
            if(strcmp(train_id,ticket[i].train_id)==0&&strcmp(loc1,ticket[i].loc1)==0&&strcmp(loc2,ticket[i].loc2)==0){
                for(int j=0;j<ticket[i].price_num;++j){
                    if(strcmp(ticket_kind,ticket[i].price_name[j])==0){
                        if(t.remain[j][D.pos]<num) return 0;
                        t.remain[j][D.pos]-=num;
                        ticket[i].remain[j]+=num;
                        ticket[i].sum+=num;
                        found=true;
                        break;
                    }
                }
                break;
            }
        }
        if(!found){
            if(!ticket[num_ticket++].buy(t,train_id,cat,D,ticket_kind,num,loc1,loc2)) return 0;
        }
        return 1;
    }

    /*
     * User refund ticket.
     */
    int User::refund_ticket(Ticket& t,const int& n,const char* train_id,const char* loc1,const char* loc2,const Date& D,const char *price_type){
        bool success=false;
        for(int i=0;i<num_ticket;++i){

            std::cout<<num_ticket<<" "<<ticket[i].train_id<<" "<<train_id;

            std::cout<<ticket[i].loc1<<" "<<loc1<<" "<<std::endl;

            if(strcmp(ticket[i].train_id,train_id)==0&&strcmp(ticket[i].loc1,loc1)==0&&strcmp(ticket[i].loc2,loc2)==0&&D==ticket[i].D)
            {
                for(int j=0;j<t.price_num;++j){
                    std::cout<<t.price_name[j]<<" "<<price_type<<std::endl;
                    if(strcmp(t.price_name[j],price_type)==0){
                        if(ticket[i].remain[j]<n) return 0;
                        ticket[i].remain[j]-=n;
                        ticket[i].sum-=n;
                        t.remain[j][D.pos]+=n;
                        std::cout<<ticket[i].remain[j]<<" "<< t.remain[j][D.pos]<<std::endl;
                        success=true;
                        break;
                    }
                }
                if(!ticket[i].sum){
                    for(int j=i;j<num_ticket-1;++j){
                        ticket[j]=ticket[j+1];
                    }
                    --num_ticket;
                }
                break;
            }
        }
        if(!success) return 0;
        return 1;
    }


    int User::modify(const char* Na,const char* pass,const char* em,const char* pho){
            strcpy(name,Na);
            strcpy(password,pass);
            strcpy(email,em);
            strcpy(phone,pho);
            return 1;
    }

    int modify_privilege(const User &U1,User &U2,int privilege){
         if(U1.privilege<2) return 0;
         if(U2.privilege==2&&privilege<2) return 0;
         U2.privilege=privilege;
         return 1;
    }

    std::ostream &operator<<(std::ostream &os,const User& U){
        os<<" "<<U.name<<" "<<U.password<<" "<<U.email<<" "<<U.phone;
        return os;
    }



}


#endif //SJTU_USER_HPP

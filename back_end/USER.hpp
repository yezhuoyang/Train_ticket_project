//
// Created by yezhuoyang on 2019-04-30.
//

#ifndef SJTU_USER_HPP
#define SJTU_USER_HPP
#include <iostream>
#include "back_end/TRAIN.hpp"


namespace sjtu {
    class User {
        friend int modify_privilege(const User &U1,User &U2,int privilege);
    private:
        /*
         * The privilege of the user.
         * 0 unregistered
         * 1 registered user
         * 2 Administrator
         */
        int privilege;
        int id;
        char name[40];
        char password[20];
        char phone[20];
        char email[20];
        Ticket_bought ticket[10];
        int num_ticket;
    public:
        User(){num_ticket=0;privilege=0;}
        bool operator>(const User &U){
            return id>U.id;
        }
        bool operator<(const User &U){
            return id<U.id;
        }
        std::ostream &operator<<(std::ostream &os){
            os<<U.id<<" "<<U.name<<" "<<U.password<<" "<<U.email<<" "<<U.phone;
            return os;
        }
        int add_ticket(User &U,Ticket_forsale& t);
        int modify_profile(const int& ID,const char* Na,const char* pass,const char* em,const char* pho);
        int refund_ticket(const char* train_id,const char* loc1,const char* loc2, const Date& D,const char *ticket_kind);
    };





    int User::add_ticket(Ticket_forsale& t,const char* ticket_name){
        ticket[num_ticket].set(t,ticket_name);
        ++num_ticket;
        return 1;
    }





    int User::refund_ticket(const char* train_id,const char* loc1,const char* loc2, const Date& D,const char *ticket_kind){
        bool success=false;
        for(int i=0;i<num_ticket;++i){
            if(strcmp(ticket[i].train_id,train_id)==0&&strcmp(ticket[i].loc1,loc1)==0&&strcmp(ticket[i].loc2,loc2)==0&&
            ticket[i].date==D&&strcmp(ticket[i].catalog,ticket_kind)==0)
            {
                success=true;
                for(int j=i;j<num_ticket-1;++j){
                    ticket[j]=ticket[j+1];
                }
                --num_ticket;
                break;
            }
        }
        if(!success) return 0;
        return 1;
    }



    int User::modify_profile(const int &ID,const char* Na,const char* pass,const char* em,const char* pho){
            id=ID;
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







}


#endif //SJTU_USER_HPP

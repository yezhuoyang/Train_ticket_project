//
// This is the class that determines the input and output of terminal
//

#ifndef TRAIN_TICKET_PROJECT_TERMINAL_HPP
#define TRAIN_TICKET_PROJECT_TERMINAL_HPP
//#include "processor.hpp"
#include "iostream"
//#include "processor.hpp"
#include "TIME.hpp"
#include "bplus.hpp"
#include "string.h"
#include <stdio.h>
namespace sjtu{
    class terminal{
        bptree<int,User>  User_Bpp;
        int current_id;
        char input[20];
        int  id;
        int  num;
        int id1,id2;
        int  privilege;
        char name[40];
        char password[20];
        char phone[20];
        char email[20];
        Date date;
        Time time;
        char loc1[20],loc2[20];
        char ticket_kind[10];
        char train_id[10];
        char catalog[10];
        int result;
        int nums,nump;
        char MaxC[10];
        char  MinC[2];
        char* Userfile;
         /*
          * Enumerate type for different operation
          * Reg:      register          *name* *password* *email* *phone*
          * Log:      login             *id* *password*
          * Qp:       query_profile     *id*
          * Mpro:     modify_profile    *id* *name* *password* *email* *phone*
          * Mpri:     modify_privilege  *id1* *id2* *privilege*
          * Qti:      query_ticket      *loc1* *loc2* *date* *catalog*
          * Qtrs:     query_transfer    *loc1* *loc2* *date* *catalog*
          * Bt:       buy_ticket        *id* *num* *train_id* *loc1* *loc2* *date* *ticket_kind*
          * Qo:       query_order       *id* *date* *catalog*
          * Rt:       refund_ticket     *id* *num* *train_id* *loc1* *loc2* *date* *ticket_kind*
          * At:       add_train         *train_id* *name* *catalog* *num(station)* *num(price)* *(name(price) ) xnum(price)*
                                            *[name time(arriv) time(start) time(stopover) (price) x num(price) ] x num(station)*
          * St:       sale_train        *train_id*
          * Qtra:     query_train       *train_id*
          * Dt:       delete_train      *train_id*
          * Mt:       modify_train      *train_id* *name* *catalog* *num(station)* *num(price)* *(name(price) ) xnum(price)*
                                            *[name time(arriv) time(start) time(stopover) (price) x num(price) ] x num(station)*
          * Cl:       clean
          * Ex:       exit
          */
          enum type{
              Reg,Log,Qp,Mpro,Mpri,Qti,Qtrs,Bt,Qo,Rt,At,St,Qtra,Dt,Mt,Cl,Ex
          };
          type currenttype;
         int Register(const char *name, const char *pass, const char *email, const char *phone);

         int log_in(const int& id,const char* password);

         int query_profile(const int& id);

         int modify_profile(const int&id,const char* name,const char* password,const char* email,const char* phone);

         int modify_privilege(const int& id1,const int& id2,const int&privilege);

         int query_ticket(const char* loc1,const char*loc2,const Date& date,const char* catalog);

         int query_transfer(const char* loc1,const char* loc2,const Date& date,const char* catalog);

         int buy_ticket(const int&id,const int& num,const char*train_id,const char*loc1,const char*loc2,const Date& date,const char*ticket_kind);

         int query_order(const int& id,const Date& date,const char*catalog);

         int refund_ticket(const int& id,const int& num,const char* train_id,const char* loc1,const char* loc2,const Date& date,const char* ticket_kind);

         int add_train(const char* ticket_id,const char* name,const char* catalog,const int& nums,const int& nump);

         int put_on_sale(const char *tid);

         int query_train(const char *tid);

         int delete_train(const char *tid);

         int modify_train(const char*train_id,const char* name,const char* catalog,const int&nums,const int& nump);
    public:
          terminal(const char* userfile):User_Bpp(userfile){
              MinC[0]=0;
              MaxC[0]=255;
              MaxC[1]=0;
              strcpy(Userfile,userfile);
          }
          ~terminal(){

          }
          void clear(){
              current_id=2018;
              User_Bpp.init();
          }
          int execute(){
              strcpy(input,"\0");
              std::cin>>input;
              if(strcmp(input,"register")==0){
                  currenttype=Reg;
                  std::cin>>name>>password>>email>>phone;
                  result=Register(name,password,email,phone);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"login")==0){
                  currenttype=Log;
                  std::cin>>id>>password;
                  result=log_in(id,password);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"query_profile")==0){
                  currenttype=Qp;
                  std::cin>>id;
                  result=query_profile(id);
                  if(!result) std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"modify_profile")==0){
                  currenttype=Mpro;
                  std::cin>>id>>name>>password>>email>>phone;
                  result=modify_profile(id,name,password,email,phone);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"modify_privilege")==0){
                  currenttype=Mpri;
                  std::cin>>id1>>id2>>privilege;
                  result=modify_privilege(id1,id2,privilege);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"query_ticket")==0){
                  currenttype=Qti;
                  std::cin>>loc1>>loc2>>date>>catalog;
                  result=query_ticket(loc1,loc2,date,catalog);
                  if(!result) std::cout<<-1<<std::endl;
              }
              else if(strcmp(input,"query_transfer")==0){
                  currenttype=Qtrs;
                  std::cin>>loc1>>loc2>>date>>catalog;
                  result=query_transfer(loc1,loc2,date,catalog);
                  if(!result) std::cout<<-1<<std::endl;
              }
              else if(strcmp(input,"buy_ticket")==0){
                  currenttype=Bt;
                  std::cin>>id>>num>>train_id>>loc1>>loc2>>date>>ticket_kind;
                  result=buy_ticket(id,num,train_id,loc1,loc2,date,ticket_kind);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"query_order")==0){
                  currenttype=Qo;
                  std::cin>>id>>date>>catalog;
                  result=query_order(id,date,catalog);
                  if(result==-1)std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"refund_ticket")==0) {
                  currenttype=Rt;
                  std::cin>>id>>num>>train_id>>loc1>>loc2>>date>>ticket_kind;
                  result=refund_ticket(id,num,train_id,loc1,loc2,date,ticket_kind);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"add_train")==0){
                  currenttype=At;
                  std::cin>>train_id>>name>>catalog>>nums>>nump;
                  result=add_train(train_id,name,catalog,nums,nump);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"sale_train")==0){
                  currenttype=St;
                  std::cin>>train_id;
                  result=put_on_sale(train_id);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"query_train")==0){
                  currenttype=Qtra;
                  std::cin>>train_id;
                  result=query_train(train_id);
                  if(!result) std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"delete_train")==0){
                  currenttype=Dt;
                  std::cin>>train_id;
                  result=delete_train(train_id);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"modify_train")==0){
                  currenttype=Mt;
                  std::cin>>train_id>>name>>catalog>>nums>>nump;
                  result=modify_train(train_id,name,catalog,nums,nump);
                  std::cout<<result<<std::endl;
              }
              else if(strcmp(input,"clean")==0){
                  currenttype=Cl;
                  clear();
                  std::cout<<1<<std::endl;
              }
              else if(strcmp(input,"exit")==0){
                  currenttype=Ex;
                  std::cout<<"BYE"<<std::endl;
              }
              if(currenttype==Ex) return 0;
              else return 1;
          }
    };


    /*
     * TODO 用户已经存在的时候要有特殊返回值
     */
    int terminal::Register(const char *name, const char *pass, const char *email, const char *phone){
        User U;
        U.modify(name, pass, email, phone);
        if(current_id==2018) U.privilege=2;
        //读取失败！
        if(!User_Bpp.insert(current_id,U)) return 0;
        return (current_id++);
    }



    int terminal::put_on_sale(const char *tid){
        return 1;
    }

/*
 * TODO
 */
    int terminal::query_transfer(const char *loc1, const char *loc2, const Date &D, const char *cat){
        return 1;
    }




    int terminal::query_profile(const int &id) {
        User U;
        U=User_Bpp.find(id);
        if(U==User()) return 0;
        std::cout << U.name << " " << U.email<<" "<<U.phone<< " " << U.privilege << std::endl;
        return 1;
    }


/*
 *   Query the ticket that User id has bought
 */
    int terminal::query_order(const int &id, const Date &date, const char *catalog){
        return 1;
    }


    int terminal::buy_ticket(const int&id,const int& num,const char*train_id,const char*loc1,const char*loc2,const Date& date,const char*ticket_kind){
        return 1;
    }


    int terminal::refund_ticket(const int& id,const int& num,const char* train_id,const char* loc1,const char* loc2,const Date& date,const char* ticket_kind){
        return 1;
    }



    int terminal::add_train(const char *train_id, const char *name, const char *catalog, const int &nums, const int &nump) {
        return 1;
    }



    int terminal::modify_train(const char *train_id, const char *name, const char *catalog, const int &nums, const int &nump) {
        return 1;
    }


    int terminal::delete_train(const char *tid){
        return 1;
    }


    int terminal::log_in(const int &id, const char *pass){
        User U;
        U=User_Bpp.find(id);
        if (U==User()) return 0;
        if (strcmp(U.password, pass) != 0) return 0;
        return 1;
    }

    int terminal::modify_profile(const int &id, const char *name, const char *password, const char *email, const char *phone){
        User U;
        U=User_Bpp.find(id);
        if (U==User()) return 0;
        U.modify(name, password, email, phone);
        User_Bpp.insert(id,U);
        return 1;
    }



    int terminal::modify_privilege(const int &id1, const int &id2, const int &privilege) {
        User U1, U2;
        U1=User_Bpp.find(id1);
        if (U1==User()) return 0;
        U2= User_Bpp.find(id2);
        if (U2==User()) return 0;
        if (U1.privilege < 2) return 0;
        if (U2.privilege == 2&&privilege<2) return 0;
        U2.privilege = privilege;
        User_Bpp.insert(id2,U2);
        return 1;
    }

    int terminal::query_train(const char *tid){
        return 1;
    }









    int terminal::query_ticket(const char *lc1, const char *lc2, const Date &D, const char *cat){
        return 1;
    }






}











#endif //TRAIN_TICKET_PROJECT_TERMINAL_HPP

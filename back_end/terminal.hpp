//
// This is the class that determines the input and output of terminal
//
#ifndef TRAIN_TICKET_PROJECT_TERMINAL_HPP
#define TRAIN_TICKET_PROJECT_TERMINAL_HPP
#include "TIME.hpp"
#include "TRAIN.hpp"
#include "bplus.hpp"
#include "string.h"
#include "constant.h"
#include "USER.hpp"
namespace sjtu{
    void print(const Trainkey& Tkey,const Train& T){
         std::cout<<Tkey.train_id<<" "<<T.name<<std::endl;
    }
    void print1(const Ticketkey& Tkey,const Ticket& T){
        std::cout<<Tkey.tid<<" "<<Tkey.loc1<<" "<<Tkey.loc2<<" "<<" "<<T.catalog<<std::endl;
        std::cout<<T.loc1<<" "<<T.loc2<<std::endl;
    }
    class terminal{
        list<User>  User_list;
        bptree<Trainkey,Train,4096,compare_train> Train_bpp;
        bptree<Orderkey,Order,4096,compare_order> Order_bpp;
        bptree<Ticketkey,Ticket,4096,compare_ticket> Ticket_bpp;
        link<Station> Station_link;
        FILE* infoF;//用来管理用户信息的文件
        int current_id;
        char input[20];
        int  id;
        int  num;
        int id1,id2;
        int  privilege;
        char name[NAMESIZE];
        char password[PASSSIZE];
        char phone[PHONESIZE];
        char email[EMAILSIZE];
        Date date;
        Time time;
        char loc1[LOCSIZE],loc2[LOCSIZE];
        char ticket_kind[PRICESIZE];
        char train_id[IDSIZE];
        char catalog[CATSIZE];
        int result;
        int nums,nump;
        char  Trainfile[FILENAME];char Trainidfile[FILENAME];
        char  Orderfile[FILENAME];char Orderidfile[FILENAME];
        char  Ticketfile[FILENAME];char Ticketidfile[FILENAME];
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
          terminal(const char* uf,const char* Tf,const char* Tfid,const char* of,const char* ofid,const char* tf,const char* tfid,
                  const char* stationfile):User_list(uf),Train_bpp(Tf,Tfid),Order_bpp(of,ofid),Ticket_bpp(tf,tfid),
                  Station_link(stationfile){
              strcpy(Trainfile,Tf);
              strcpy(Trainidfile,Tfid);
              strcpy(Orderfile,of);
              strcpy(Orderidfile,ofid);
              strcpy(Ticketfile,tf);
              strcpy(Ticketidfile,tfid);
              current_id=User_list.Size()+FIRSTID;
          }
          ~terminal(){

          }
          void clear(){
              current_id=FIRSTID;
              User_list.clear();
              Train_bpp.init();
              Order_bpp.init();
              Ticket_bpp.init();
              Station_link.clear();
          }
          int execute(){
              strcpy(input,"\0");
              std::cin>>input;
              if(strcmp(input,"register")==0){
                  currenttype=Reg;
                  std::cin>>name>>password>>email>>phone;
                  result=Register(name,password,email,phone);
                  if(!result)
                      std::cout<<result<<std::endl;
                  else
                      std::cout<<current_id-1<<std::endl;
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
                  if(!result) std::cout<<0<<std::endl;
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
                  if(result==0)std::cout<<result<<std::endl;
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
              else if(strcmp(input,"sale_train")==0) {
                  currenttype = St;
                  std::cin >> train_id;
                  result = put_on_sale(train_id);
                  std::cout << result << std::endl;
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
              else if(strcmp(input,"traverse_Train")==0){
                  Train_bpp.traverse(print);
              }
              else if(strcmp(input,"traverse_Ticket")==0){
                  Ticket_bpp.traverse(print1);
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
        U.privilege=1;
        if(current_id==FIRSTID) U.privilege=2;
        //读取失败！
        User_list.push_back(U);
        ++current_id;
        return 1;
    }

    int terminal::put_on_sale(const char *tid){
        //std::cout<<"Selling"<<" "<<tid<<std::endl;
        Trainkey K(tid);
        if(!Train_bpp.count(K)) {
            return 0;
        }
        Train T=Train_bpp.find(K);
        if(T.For_sale){
            return 0;}
        T.For_sale=true;
        sjtu::vector<Station> V;
        Station_link.read_block(T.stblock,V);
        Ticket tmp;
        tmp.copy(T);
        for(int i=0;i<V.size();++i){
            for(int k=0;k<T.price_num;++k){
                tmp.price[k]=0;
            }
            tmp.start_time=V[i].start_time;
            for(int j=i+1;j<V.size();++j){
                for(int k=0;k<T.price_num;++k){
                    tmp.price[k]+=V[j].price[k];
                }
                tmp.arrive_time=V[j].arrive_time;
                strcpy(tmp.loc1,V[i].loc);
                strcpy(tmp.loc2,V[j].loc);
                Ticket_bpp.insert(Ticketkey(tid,V[i].loc,V[j].loc),tmp);
                Ticket_bpp.insert(Ticketkey(tid,V[j].loc,V[i].loc),tmp);
            }
        }
        Train_bpp.set(K,T);
        return 1;
    }

/*
     add_train A  Atrain  CD 5 1 商务座
     北京 xx:xx 08:00 00:00 ¥0.0
     夏威夷 08:01 08:02 00:00 ¥1.5
     南京 08:03 08:04 00:00 ¥1.5
     东京 08:05 08:08 00:00 ¥1.5
     日本 08:10 08:12 00:00 ¥1.5
 *
 */
/*
     add_train B  Btrain CD 4 1 商务座
     天津 xx:xx 08:00 00:00 ¥0.0
     南京 08:02 08:04 00:00 ¥1.5
     东京 08:06 08:08 00:00 ¥1.5
     海南 08:10 xx:xx 00:00 ¥1.5
 */

/*
    add_train E  Etrain CD 3 1 商务座
    日本 xx:xx 08:20 00:00 ¥0.0
    南京 08:25 08:30 00:00 ¥1.5
    海南 08:40 xx:xx 00:00 ¥1.5
*/
    int terminal::query_transfer(const char *loc1, const char *loc2, const Date &D, const char *cat){
        Ticketkey key1("\0",loc1,"\0");
        Ticketkey key2("\0",loc2,"\0");
        vector<pair<Ticketkey,Ticket>> V1;
        vector<pair<Ticketkey,Ticket>> V2;
        vector<pair<Ticketkey,Ticket>> V10;
        vector<pair<Ticketkey,Ticket>> V20;
        Ticket_bpp.search(V10,key1,compare_ticket());
        Ticket_bpp.search(V20,key2,compare_ticket());
        for(int i=0;i<V10.size();++i){
            if(strstr(cat,V10[i].second.catalog)!=NULL&&strcmp(V10[i].second.loc1,loc1)==0)
                V1.push_back(V10[i]);
        }
        for(int i=0;i<V20.size();++i){
            if(strstr(cat,V20[i].second.catalog)!=NULL&&strcmp(V20[i].second.loc2,loc2)==0){
                //std::cout<<V20[i].second.loc1<<" "<<V20[i].second.loc2<<" "<<loc2<<std::endl;
                V2.push_back(V20[i]);
            }
        }
        if(V1.empty()||V2.empty()) return 0;
        int mintime=1e9;
        int x=-1,y=-1;
        for(int i=0;i<V1.size();++i){
            for(int j=0;j<V2.size();++j){
                if(strcmp(V1[i].second.loc2,V2[j].second.loc1)==0&&V1[i].second.arrive_time<V2[j].second.start_time){
                     if(V2[j].second.arrive_time-V1[i].second.start_time<mintime){
                         mintime=V2[j].second.arrive_time-V1[i].second.start_time;
                         x=i;y=j;
                     }
                }
            }
        }
        if(x==-1&&y==-1) return 0;
        std::cout<<V1[x].first.tid<<" "<<V1[x].second.loc1<<" "<<D<<" "<<V1[x].second.start_time<<" ";
        std::cout<<V1[x].second.loc2<<" "<<D<<" "<<V1[x].second.arrive_time<<" ";
        for(int i=0;i<V1[x].second.price_num;++i){
            std::cout<<V1[x].second.price_name[i]<<" "<<V1[x].second.remain[i][D.pos]<<" "<<V1[x].second.price[i]<<" ";
        }
        std::cout<<std::endl;
        std::cout<<V2[y].first.tid<<" "<<V2[y].second.loc1<<" "<<D<<" "<<V2[y].second.start_time<<" ";
        std::cout<<V2[y].second.loc2<<" "<<D<<" "<<V2[y].second.arrive_time<<" ";
        for(int i=0;i<V2[y].second.price_num;++i){
            std::cout<<V2[y].second.price_name[i]<<" "<<V2[y].second.remain[i][D.pos]<<" "<<V2[y].second.price[i]<<" ";
        }
        std::cout<<std::endl;
        return 1;
    }

    int terminal::query_profile(const int &id){
        User U;
        if (!User_list.find(id-FIRSTID,U)) return 0;
        std::cout << U.name << " " << U.email<<" "<<U.phone<< " " << U.privilege << std::endl;
        return 1;
    }
    /*
    *   Query the ticket that User id has bought
    */
    int terminal::query_order(const int &id, const Date &date, const char *catalog){
        Orderkey ok(id,date,"\0");
        vector<pair<Orderkey,Order>> V;
        vector<pair<Orderkey,Order>> Vfound;
        Order_bpp.search(V,ok,search_order());
        for(int i=0;i<V.size();++i){
            if(strstr(catalog,V[i].second.catalog)!=NULL){
               Vfound.push_back(V[i]);
            }
        }
        if(!Vfound.size()) return 0;
        std::cout<<Vfound.size()<<std::endl;
        for(int i=0;i<Vfound.size();++i){
            std::cout<<Vfound[i].first.train_id<<" "<<Vfound[i].second.loc1<<" "<<date<<" "<<Vfound[i].second.start_time<<" ";
            std::cout<<Vfound[i].second.loc2<<" "<<date<<" "<<Vfound[i].second.arrive_time<<" ";
            for(int j=0;j<Vfound[i].second.price_num;++j){
                std::cout<<Vfound[i].second.price_name[j]<<" "<<Vfound[i].second.remain[j]<<" "<<Vfound[i].second.price[j]<<" ";
            }
            std::cout<<std::endl;
        }
        return 1;
    }
//    buy_ticket 2018 1 abc123456 北京 夏威夷  2018-06-28 商务座
//    1
//    query_order 2018 2018-06-28 G
    //buy_ticket 6666 1 C101 北京 夏威夷 2018-03-28 一等座
    int terminal::buy_ticket(const int&id,const int& num,const char*train_id,const char*loc1,const char*loc2,const Date& date,const char*ticket_kind){
        if((id-FIRSTID)>User_list.Size()){
            return 0;
        }
        Ticketkey K(train_id,loc1,loc2);
        vector<pair<Ticketkey,Ticket>> V;
        Ticket_bpp.search(V,K,compare_ticket());
        if(V.empty()) {
            return 0;
        }
        //确定改票种的位置
        int k=0;
        while(k<V[0].second.price_num&&strcmp(V[0].second.price_name[k],ticket_kind)!=0)++k;
        if(k==V[0].second.price_num) {
            return 0;
        }
        if(V[0].second.remain[k][date.pos]<num) {
            return 0;
        }
        V[0].second.remain[k][date.pos]-=num;
        Ticket_bpp.set(V[0].first,V[0].second);
        Ticketkey K2(train_id,loc2,loc1);
        Ticket_bpp.set(K2,V[0].second);
        Orderkey ok(id,date,V[0].first.tid);
        Order O;
        if(Order_bpp.count(ok)){
            O=Order_bpp.find(ok);
            O.remain[k]+=num;
            O.sum+=num;
            Order_bpp.set(ok,O);
        }
        else{
            O.copy(V[0].second);
            O.remain[k]=num;
            O.sum=num;
            strcpy(O.loc1,loc1);
            strcpy(O.loc2,loc2);
            Order_bpp.insert(ok,O);
        }
        return 1;
    }


    int terminal::refund_ticket(const int& id,const int& num,const char* train_id,const char* loc1,const char* loc2,const Date& date,const char* ticket_kind){
        Orderkey okey(id,date,train_id);
        vector<pair<Orderkey,Order>> V;
        Order_bpp.search(V,okey,compare_order());
        if(!V.size()) return 0;
        int p=0;
        while(p<V.size()&&(strcmp(V[p].second.loc1,loc1)!=0||strcmp(V[p].second.loc2,loc2)!=0))++p;
        if(p==V.size()) return 0;
        int k=0;
        while(k<V[p].second.price_num&&strcmp(ticket_kind,V[p].second.price_name[k])!=0)++k;
        if(k==V[p].second.price_num) return 0;
        if(V[p].second.remain[k]<num) return 0;
        if(V[p].second.sum==num){
            Order_bpp.remove(V[p].first);
        }
        else{
            V[p].second.sum-=num;
            V[p].second.remain[k]-=num;
            Order_bpp.set(V[p].first,V[p].second);
        }
        Ticketkey tkey(train_id,loc1,loc2);
        Ticketkey tkey2(train_id,loc2,loc1);
        Ticket T=Ticket_bpp.find(tkey);
        T.remain[k][date.pos]+=num;
        Ticket_bpp.set(tkey,T);
        Ticket_bpp.set(tkey2,T);
        return 1;
    }

    int terminal::add_train(const char *train_id, const char *name, const char *catalog, const int &nums, const int &nump){
        Trainkey K(train_id);
        if(Train_bpp.count(K)) return 0;
        Train T;
        strcpy(T.name, name);
        strcpy(T.catalog, catalog);
        T.station_num = nums;
        T.price_num = nump;
        Station tmp;
        vector<Station> V;
        for (int i = 0; i < nump; ++i){
            std::cin >> T.price_name[i];
        }
        for (int i = 0; i < nums; ++i) {
            tmp.type_num=nump;
            std::cin >>tmp;
            V.push_back(tmp);
        }
        T.stblock=Station_link.push_back(V);
        Train_bpp.insert(K,T);
        return 1;
    }


    int terminal::modify_train(const char *train_id, const char *name, const char *catalog, const int &nums, const int &nump){
        Trainkey K(train_id);
        if(!Train_bpp.count(K)) return 0;
        Train T=Train_bpp.find(K);
        if(T.For_sale) return 0;
        strcpy(T.name, name);
        strcpy(T.catalog, catalog);
        T.station_num = nums;
        T.price_num = nump;
        Station tmp;
        vector<Station> V;
        for (int i = 0; i < nump; ++i){
            std::cin >> T.price_name[i];
        }
        for (int i = 0; i < nums; ++i) {
            tmp.type_num=nump;
            std::cin >>tmp;
            V.push_back(tmp);
        }
        T.stblock=Station_link.push_back(V);
        Train_bpp.set(K,T);
        return 1;
    }

    //删掉车次，Station_link里的东西就不管了
    int terminal::delete_train(const char *tid){
        Trainkey K(tid);
        if(!Train_bpp.count(K)) return 0;
        Train_bpp.remove(K);
        return 1;
    }


    int terminal::log_in(const int &id, const char *pass){
        User U;
        if (!User_list.find(id-FIRSTID,U)) return 0;
        if (strcmp(U.password, pass) != 0) return 0;
        return 1;
    }



    int terminal::modify_profile(const int &id, const char *name, const char *password, const char *email, const char *phone){
        User U;
        if(!User_list.find(id-FIRSTID,U)) return 0;
        U.modify(name, password, email, phone);
        User_list.modify(id-FIRSTID,U);
        return 1;
    }




    int terminal::modify_privilege(const int &id1, const int &id2, const int &privilege){
        User U1, U2;
        if (!User_list.find(id1-FIRSTID,U1)) return 0;
        if(!User_list.find(id2-FIRSTID,U2)) return 0;
        if (U1.privilege < 2) return 0;
        if (U2.privilege == 2&&privilege<2) return 0;
        U2.privilege = privilege;
        User_list.modify(id2-FIRSTID,U2);
        return 1;
    }



    int terminal::query_train(const char *tid){
        Trainkey K(tid);
        if(!Train_bpp.count(K)) return 0;
        Train T=Train_bpp.find(K);
        std::cout << tid << " " << T.name << " " << T.catalog << " " << T.station_num << " " << T.price_num << " ";
        for (int i = 0; i < T.price_num; ++i) {
            std::cout << T.price_name[i] << " ";
        }
        std::cout<<std::endl;
        sjtu::vector<Station> V;
        Station_link.read_block(T.stblock,V);
        for (int i = 0; i <V.size(); ++i) {
            std::cout<<V[i] << std::endl;
        }
        std::cout << std::endl;
        return 1;
    }




    int terminal::query_ticket(const char *lc1, const char *lc2, const Date &D, const char *cat){
        Ticketkey K("\0",lc1,lc2);
        vector<pair<Ticketkey,Ticket>> V;
        vector<pair<Ticketkey,Ticket>> Vfound;
        Ticket_bpp.search(V,K,search_ticket());
        for(int i=0;i<V.size();++i){
            if(strstr(cat,V[i].second.catalog)!=NULL&&strcmp(lc1,V[i].second.loc1)==0)
                Vfound.push_back(V[i]);
        }
        if(!Vfound.size()) return 0;
        std::cout<<Vfound.size()<<std::endl;
        for(int k=0;k<Vfound.size();++k){
            if(strstr(cat,Vfound[k].second.catalog)!=NULL){
                std::cout<<Vfound[k].first.tid<<" "<<Vfound[k].second.loc1<<" "<<D<<" "<<Vfound[k].second.start_time<<" ";
                std::cout<<Vfound[k].second.loc2<<" "<<D<<" "<<Vfound[k].second.arrive_time<<" ";
                for(int i=0;i<Vfound[k].second.price_num;++i){
                    std::cout<<Vfound[k].second.price_name[i]<<" "<<Vfound[k].second.remain[i][D.pos]<<" "<<Vfound[k].second.price[i]<<" ";
                }
                std::cout<<std::endl;
            }
        }
        return 1;
    }




}











#endif //TRAIN_TICKET_PROJECT_TERMINAL_HPP

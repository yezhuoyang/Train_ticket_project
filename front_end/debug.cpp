//
// Created by yezhuoyang on 2019-05-15.
//


#include "../back_end/terminal.hpp"


using namespace sjtu;






int main(){




    terminal T("User.txt","train.txt","trainid.txt","order.txt","orderid.txt","ticket.txt","ticketid.txt","station.txt");
    int c=1;
    while(c){
        c=T.execute();
    }



}



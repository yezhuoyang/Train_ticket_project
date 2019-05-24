//
// Created by yezhuoyang on 2019-04-30.
//

#ifndef SJTU_TIME_HPP
#define SJTU_TIME_HPP
#include <iostream>
#include "stdio.h"



namespace  sjtu {
    /*
     * The function convert hour ab and minute cd to string "ab:cd"
     */
    void convert_to_char(char list[],int hour,int minute){
        list[1]='0'+hour-10*(hour/10);
        list[0]='0'+hour/10;
        list[2]=':';
        list[4]='0'+minute-10*(minute/10);
        list[3]='0'+minute/10;
        list[5]='\0';
    }
    /*
     * The function convert the string of date "XXXX:XX:ab" to an integer  ab-1
     */
    int date_to_int(const char list[]){
        return 10*(list[8]-'0')+(list[9]-'1');
    }
    struct Date {
        char date[13];
        /*
         * Store a date as an integer.
         * 0 denotes 2019-06-01
         */
        int  pos;
        Date(){strcpy(date,"XXXX—XX—XX");}
        Date(const char*D){
            strcpy(date,D);
            pos=date_to_int(date);
        }
        Date& operator=(const Date& rhs){
            if(&rhs==this) return *this;
            strcpy(date,rhs.date);
            pos=rhs.pos;
            return *this;
        }
        bool operator>(const Date &rhs){
            return strcmp(date,rhs.date)>0;
        }
        bool operator<(const Date &rhs){
            return strcmp(date,rhs.date)<0;
        }
        bool operator==(const Date& rhs) const{
            return strcmp(date,rhs.date)==0;
        }
    };
//时间类
    struct Time {
        int hour;
        int minute;
        char time[6];
        Time(const char* T){
            strcpy(time,T);
            hour=10*time[0]+time[1];
            minute=10*time[3]+time[4];
        }
        Time(){
            strcpy(time,"XX:XX");
            hour=minute=0;
        }
        Time& operator=(Time& rhs){
            if(&rhs==this) return *this;
            strcpy(time,rhs.time);
            hour=rhs.hour;
            minute=rhs.minute;
            return *this;
        }
        bool operator>(const Time &T){
            return strcmp(time,T.time)>0;
        }
        bool operator<(const Time& T){
            return strcmp(time,T.time)<0;
        }
        bool operator==(const Time &T)const{
            return strcmp(time,T.time)==0;
        }
        Time& operator=(const Time& rhs){
            if(&rhs==this) return *this;
            strcpy(time,rhs.time);
            hour=rhs.hour;
            minute=rhs.minute;
            return *this;
        }
        int operator-(const Time& rhs){
            return (hour-rhs.hour)*60+minute-rhs.minute;
        }


    };
    std::ostream &operator<<(std::ostream& os,const Date& D){
         os<<D.date;
         return os;
    }

    std::istream &operator>>(std::istream& is,Date& D){
        is>>D.date;
        D.pos=date_to_int(D.date);
        return is;
    }
    std::ostream& operator<<(std::ostream &os,const Time& T){
        os<<T.time;
        return os;
    }
    std::istream& operator>>(std::istream &is,Time& T){
        is>>T.time;
        T.hour=10*(T.time[0]-'0')+(T.time[1]-'0');
        T.minute=10*(T.time[3]-'0')+(T.time[4]-'0');
        return is;
    }


}

#endif 

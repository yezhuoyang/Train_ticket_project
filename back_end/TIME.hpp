//
// Created by yezhuoyang on 2019-04-30.
//

#ifndef SJTU_TIME_HPP
#define SJTU_TIME_HPP
#include <iostream>
#include "stdio.h"
namespace  sjtu {
    class Date {
    public:
        Date(){strcpy(date,"XXXX:XX:XX");}
        std::ostream &operator<<(std::ostream &os){
                    os<<date;
                    return os;
        }
        std::istream &operator>>(std::istream &is){
                is>>date;
                return is;
        }
        Date& operator=(Date& rhs){
            strcpy(date,rhs.date);
            return *this;
        }
        bool operator>(const Date &rhs){
            return strcmp(date,rhs.date)>0;
        }
        bool operator<(const Date &rhs){
            return strcmp(date,rhs.date)<0;
        }
        bool operator==(const Date& rhs){
            return strcmp(date,rhs.date)==0;
        }
        private:
            char date[13];
    };
//时间类
    class Time {
        friend int gap(const Time &T1, const Time &T2);
    private:
        int hour;
        int second;
        char time[6];
    public:
        Time(const char* T){
            strcpy(time,T);
            hour=10*time[0]+time[1];
            second=10*time[3]+time[4];
        }
        Time(){
            strcpy(time,"XX:XX");
            hour=second=0;
        }
        Time& operator=(Time& rhs){
            strcpy(time,rhs.time);
            hour=rhs.hour;
            second=rhs.second;
            return *this;
        }
        std::ostream &operator<<(std::ostream &os){
            os<<time;
            return os;
        }
        std::istream &operator>>(std::istream &is){
            is>>time;
            hour=10*(time[0]-'0')+(time[1]-'0');
            second=10*(time[3]-'0')+(time[4]-'0');
            return is;
        }
        bool operator>(const Time &T){
            return strcmp(time,T.time)>0;
        }
        bool operator<(const Time& T){
            return strcmp(time,T.time)<0;
        }
        bool operator==(const Time &T){
            return strcmp(time,T.time)==0;
        }
    };




}

#endif 

//
// Created by yezhuoyang on 2019-05-15.
//

#include <iostream>

#include "../map/map.hpp"

#include "../back_end/TIME.hpp"
#include "../back_end/TRAIN.hpp"
#include "../back_end/USER.hpp"
#include "../map/map.hpp"
#include "../map/vector.hpp"
#include "../back_end/terminal.hpp"



using namespace sjtu;
using namespace std;
using namespace sjtu;




int main() {
     terminal T("Userfile.txt");
     int C=1;
     while(C){
          C=T.execute();
     }

}
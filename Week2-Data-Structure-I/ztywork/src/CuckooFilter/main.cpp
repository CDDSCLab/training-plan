#include "cuckoofilter.h"

cuckoofilter cuck;

int main()
{
    std::string key;
    std::cout<<"please try insert:"<<std::endl;
    while(std::cin>>key){
        if(key=="quit")break;
        if(cuck.inserts(key)){
            std::cout<<"add."<<std::endl;
        }else{
            std::cout<<"fail."<<std::endl;
        }
    }
    std::cout<<"please try delete:"<<std::endl;
    while(std::cin>>key){
        if(key=="quit")break;
        if(cuck.deletes(key)){
            std::cout<<"deletes."<<std::endl;
        }else{
            std::cout<<"fail."<<std::endl;
        }
    }

    return 0;
}
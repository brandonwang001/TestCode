#include <iostream>
#include <vector>

using namespace std;


int main(){
  
  uint64_t tablet_ids[] = {1,2,3,4,5,100,1000,10000,100000};

  vector<uint64_t> tablet_id_vec(tablet_ids, tablet_ids + (sizeof(tablet_ids)/sizeof(uint64_t)-1));

  vector<uint64_t> * ptr = &tablet_id_vec;
  
  for(auto &itr : *ptr) {
    cout << itr << endl;  
  }

  return 0;  
}



//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef GENERICLIST_H_
#define GENERICLIST_H_

#include <vector>
using namespace std;

template <class A_TYPE>
class genericList: public vector<A_TYPE> {
public:
    genericList(){}
    virtual ~genericList(){}

    void addItem(A_TYPE item){
        if (containsItem(item))
            return;

        this->push_back(item);
    }
    void removeItem(A_TYPE item){
        typename genericList<A_TYPE>::iterator it;

        for (it = this->begin(); it != this->end(); it++){
            if ( (*it) == item ) {

                erase(it);

                return;
            }
        }
    }
    void removeItem(A_TYPE* item, bool deleteItem = false){
        typename genericList<A_TYPE>::iterator it;

        for (it = this->begin(); it != this->end(); it++){
            if ( (*it) == item ) {

                if (deleteItem)
                    delete (*it);
                erase(it);

                return;
            }
        }
    }
    bool containsItem(A_TYPE item){
        typename genericList<A_TYPE>::iterator it;

        for (it = this->begin(); it != this->end(); it++){
            if ( (*it) == item ) {
                return true;
            }
        }

        return false;
    }
};

#endif /* GENERICLIST_H_ */
/*
void MTreeBoneStripe::removeChild(IPvXAddress addr){
    std::vector<IPvXAddress>::iterator it;

    for (it = m_Children.begin(); it != m_Children.end(); it++){
        if ( it->equals(addr) ){

            m_Children.erase(it);

            return;
        }
    }
}

*/

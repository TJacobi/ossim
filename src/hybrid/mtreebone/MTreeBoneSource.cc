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

#include "MTreeBoneSource.h"

Define_Module(MTreeBoneSource);

MTreeBoneSource::MTreeBoneSource(){

}

MTreeBoneSource::~MTreeBoneSource() {

}

void MTreeBoneSource::initialize(int stage){
    if (stage != 3)
        return;
    initBase();

    // create messages


    // schedule timers

}

void MTreeBoneSource::handleTimerMessage(cMessage *msg){

    if (msg == NULL)
    {
    }
    else
    {
        MTreeBoneBase::handleTimerMessage(msg);
    }
}

bool MTreeBoneSource::isBoneNodeForStripe(int stripe){
    return true;
}

int MTreeBoneSource::getMyDistance(){
    return 0;
}

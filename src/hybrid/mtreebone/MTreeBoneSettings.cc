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

#include "MTreeBoneSettings.h"

MTreeBoneSettings* MTreeBoneSettings::theSettings = NULL;

Define_Module(MTreeBoneSettings);

MTreeBoneSettings::MTreeBoneSettings() {
    // TODO Auto-generated constructor stub

}

MTreeBoneSettings::~MTreeBoneSettings() {
    // TODO Auto-generated destructor stub
}


void MTreeBoneSettings::initialize(int stage){
    if (stage != 1)
        return;

    MTreeBoneSettings::theSettings = this;

    // Get the number of stripes
    m_Stripes                 = par("NumberOfStripes");

    m_ChunkScheduleIntervall  = par("ChunkScheduleInterval");
    m_ChunkRequestTimeout     = par("ChunkRequestTimeout");

    m_BufferMapIntervall      = par("BufferMapIntervall");

    // partnership
    m_MaxNumberOfPartners     = par("NumberOfPartnersMaximum");
    m_DesiredNumberOfPartners = par("NumberOfPartnersDesired");
    if (m_MaxNumberOfPartners < 1) m_MaxNumberOfPartners = 4;
    if (m_DesiredNumberOfPartners < 1) m_DesiredNumberOfPartners = (m_MaxNumberOfPartners / 2) < 1 ? 1 : m_MaxNumberOfPartners / 2;


    /*
         *  k       x
            0.5     0.25
            0.6     0.264457737322019...
            0.8     0.291466380311610...
            1       0.3
            1.2     0.338932
            1.5     0.36981
            1.8     0.397388
            2       0.41421
         */
    int kMode = par("kValueMode");
    switch (kMode){
        case 0: // disable
                m_KResult = 0.0;
                break;
        case 1: // k = 0.5
                m_KResult = 0.25;
                break;
        case 2: // k = 0.6
                m_KResult = 0.264457737322019;
                break;
        case 3: // k = 0.8
                m_KResult = 0.291466380311610;
                break;
        default:
        case 4: // k = 1.0
                m_KResult = 0.3;
                break;
        case 5: // k = 1.2
                m_KResult = 0.338932;
                break;
        case 6: // k = 1.5
                m_KResult = 0.36981;
                break;
        case 7: // k = 1.8
                m_KResult = 0.397388;
                break;
        case 8: // k = 2.0
                m_KResult = 0.41421;
                break;
    }


}

void MTreeBoneSettings::finish(){

}

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

#ifndef MTREEBONESETTINGS_H_
#define MTREEBONESETTINGS_H_

#include "csimplemodule.h"

class MTreeBoneSettings : public cSimpleModule {
public:
    MTreeBoneSettings();
    virtual ~MTreeBoneSettings();

    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void finish();

    static MTreeBoneSettings* theSettings; // global pointer for easier acces

    int getNumberOfStripes(){return m_Stripes;}
    double getKResult(){return m_KResult;}
protected:
    int m_Stripes;
    double m_ChunkScheduleIntervall, m_ChunkRequestTimeout;
    int m_MaxNumberOfPartners, m_DesiredNumberOfPartners;
    double m_BufferMapIntervall;

    double m_KResult;
};

#endif /* MTREEBONESETTINGS_H_ */

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

#ifndef PLAYERLISTENER_H_
#define PLAYERLISTENER_H_

#include "AppCommon.h"

class PlayerListener {
public:
    PlayerListener();
    virtual ~PlayerListener();

    // @brief gets called when the player starts playback
    virtual void onPlayerStarted();

    virtual void onChunksSkipped(SEQUENCE_NUMBER_T oldposition, SEQUENCE_NUMBER_T newposition){};

    virtual void onChunkHit(SEQUENCE_NUMBER_T hit){};

    virtual void onChunkMiss(SEQUENCE_NUMBER_T miss){};
};

#endif /* PLAYERLISTENER_H_ */

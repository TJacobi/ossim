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

// @author Thorsten Jacobi
// @brief interface to add listeners to the videobuffer

#ifndef VIDEOBUFFERLISTENER_H_
#define VIDEOBUFFERLISTENER_H_

class VideoBufferListener {
public:
    VideoBufferListener();
    virtual ~VideoBufferListener();

	// @brief gets called whenever the video Buffer receives a new chunk
    virtual void onNewChunk(int sequenceNumber);
};

#endif /* VIDEOBUFFERLISTENER_H_ */

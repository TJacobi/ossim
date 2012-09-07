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

#include "VideoBuffer.h"
//#include "simutil.h"
#include "AppSettingDonet.h"

#define _VERSION_1 1
#define _VERSION_2 2
#define _VERSION_3 3
#define BUFFER_IMPL_VERSION _VERSION_3

using namespace std;

Define_Module(VideoBuffer);

VideoBuffer::VideoBuffer() {}

VideoBuffer::~VideoBuffer() {}

void VideoBuffer::initialize(int stage)
{
    if (stage == 0)
    {
        signal_seqNum_receivedChunk = registerSignal("SeqNum_RecevedChunk");
    }

    if (stage != 3)
    {
        return;
    }

    cModule *temp = simulation.getModuleByPath("appSetting");
    AppSettingDonet *m_appSetting = check_and_cast<AppSettingDonet *>(temp);

    m_bufferSize_chunk  = m_appSetting->getBufferMapSizeChunk();
    m_chunkInterval     = m_appSetting->getIntervalNewChunk();

    // -- Adjust the size of the videoBuffer to the suitable value
    m_streamBuffer.resize(m_bufferSize_chunk);

    // -- Initialize every element of the array to NULL
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    for(iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++ iter)
    {
        STREAM_BUFFER_ELEMENT_T &elem = *iter;
        elem.m_chunk = NULL;
    }

    m_bufferStart_seqNum = m_bufferEnd_seqNum = m_head_received_seqNum = 0;

    WATCH(m_bufferSize_chunk);
    WATCH(m_chunkInterval);
}

void VideoBuffer::handleMessage(cMessage *)
{
    throw cException("VideoBuffer doesn't process messages!");
}

void VideoBuffer::insertPacket(VideoChunkPacket *packet)
{
    Enter_Method("insertPacket()");

    EV << "Insert a packet into video buffer:" << endl;

    long seq_num = packet->getSeqNumber();
    EV << "\tSequence number of chunk: " << seq_num << " --> element: " << seq_num % m_bufferSize_chunk << endl;

    // Stats
    emit(signal_seqNum_receivedChunk, seq_num);

    STREAM_BUFFER_ELEMENT_T & elem = m_streamBuffer[seq_num % m_bufferSize_chunk];

    if (elem.m_chunk != NULL)
    {
        EV << "\tExisting chunk needs to be deleted: " << seq_num % m_bufferSize_chunk << endl;
        delete elem.m_chunk;
        elem.m_chunk = NULL;

        if (__VIDEOBUFFER_CROSSCHECK__) --m_nActiveElement;
    }

    EV << "\tNew chunk was attached to the Buffer" << endl;
    elem.m_chunk = packet;

    if (__VIDEOBUFFER_CROSSCHECK__) ++m_nActiveElement;

#if (BUFFER_IMPL_VERSION == _VERSION_1)
    if (seq_num > m_id_bufferEnd)
    {
        // because of modification in class implementation
        m_id_bufferEnd = seq_num;
        m_id_bufferStart = std::max(0L, m_id_bufferEnd - m_bufferSize + 1);
    }
#elif (BUFFER_IMPL_VERSION == _VERSION_2)
    if (seq_num > m_bufferEnd_seqNum)
    {
        m_bufferEnd_seqNum = seq_num;
        m_bufferStart_seqNum = std::max(0L, m_bufferEnd_seqNum - m_bufferSize + 1);
    }
#elif (BUFFER_IMPL_VERSION == _VERSION_3)
    if (seq_num > m_head_received_seqNum)
    {
        EV << "\tUpdate the range of the Video Buffer:" << endl;

        m_head_received_seqNum = seq_num;
        m_bufferStart_seqNum = std::max(0L, m_head_received_seqNum - m_bufferSize_chunk + 1);
        m_bufferEnd_seqNum = m_bufferStart_seqNum + m_bufferSize_chunk - 1;
        EV << "\t-- head: " << m_head_received_seqNum << endl;
        EV << "\t-- start: " << m_bufferStart_seqNum << endl;
        EV << "\t-- end: " << m_bufferEnd_seqNum << endl;
    }
#endif


}


// TODO: verification!!!
bool VideoBuffer::isInBuffer(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("isInBuffer()");

    // Browse through the queue?
    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_streamBuffer.size()];
    /*
     * Return TRUE only if the elem points to a packet AND the seq_num of the packet is correct
     */
    return (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num);
}

bool VideoBuffer::inBuffer(SEQUENCE_NUMBER_T seq_num)
{
    // Browse through the queue?
    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_streamBuffer.size()];
    /*
     * Return TRUE only if the elem points to a packet AND the seq_num of the packet is correct
     */
    if (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num)
        return true;

    return false;
}

// TODO: verification!!!
SIM_TIME_T VideoBuffer::getReceivedTime(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("getReceivedTime()");

    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_bufferSize_chunk];
    if (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num)
    {
        return elem.m_recved_time;
    }
    return (SIM_TIME_T)-1;
}

// TODO: verification!!!
/*
MeshVideoChunkPacket * VideoBuffer::getChunk(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("getChunk()");

    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_bufferSize_chunk];
    if (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num)
    {
        return elem.m_chunk;
    }
    return NULL;
}
*/

VideoChunkPacket * VideoBuffer::getChunk(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("getChunk()");

    STREAM_BUFFER_ELEMENT_T &elem = m_streamBuffer[seq_num % m_bufferSize_chunk];
    if (elem.m_chunk != NULL && elem.m_chunk->getSeqNumber() == seq_num)
    {
        return elem.m_chunk;
    }
    return NULL;
}

// TODO: verification!!!
STREAM_BUFFER_ELEMENT_T & VideoBuffer::getBufferElement(SEQUENCE_NUMBER_T seq_num)
{
    Enter_Method("getBufferElement()");

    return m_streamBuffer[seq_num % m_bufferSize_chunk];

}

void VideoBuffer::captureVideoBuffer(BufferMap *bm)
{
    Enter_Method("captureVideoBuffer()");

    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    for (iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++iter)
    {
        // if the element stores no chunk
        if (iter->m_chunk == NULL)
            continue;

        // if the id of the packet at that element is too "old"
        // if (iter->m_chunk->getSeqNumber() < m_id_bufferStart)
        if (iter->m_chunk->getSeqNumber() < m_bufferStart_seqNum)
            continue;

        // int idx = iter->m_chunk->getSeqNumber() - m_id_bufferStart;
        int idx = iter->m_chunk->getSeqNumber() - m_bufferStart_seqNum;
        bm->setElementByOffset(idx, true); // TODO: find out the problem of this line
    }
}

void VideoBuffer::printStatus()
{
    Enter_Method("printStatus()");

/*
    // Version 1
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    EV << "[" << m_bufferStart_seqNum << ", " << m_bufferEnd_seqNum << "] --- ";
    for(iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++ iter)
    {
        STREAM_BUFFER_ELEMENT_T &elem = *iter;
        short bit = -1;
        bit = (elem.m_chunk != NULL)?1:0;
        EV << bit << " ";
    }
    EV << std::endl;
*/

    // Version 2
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    EV << "[" << m_bufferStart_seqNum << ", " << m_bufferEnd_seqNum << "] --- ";
    for(iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++ iter)
    {
        STREAM_BUFFER_ELEMENT_T &elem = *iter;
        short bit = -1;
        bit = (elem.m_chunk != NULL)?1:0;
        EV << bit << " ";
    }
    EV << std::endl;

}

void VideoBuffer::fillBufferMapPacket(MeshBufferMapPacket *bmPkt)
{
    Enter_Method("fillBufferMapPacket()");
    EV << "Mapping from Buffer Map into BM packet: " << endl;

    // bmPkt->setBufferMapArraySize(m_bufferSize_chunk);
    // bmPkt->setIdStart(m_id_bufferStart);
    // bmPkt->setIdEnd(m_id_bufferEnd);
    bmPkt->setBmStartSeqNum(m_bufferStart_seqNum);
    bmPkt->setBmEndSeqNum(m_bufferEnd_seqNum);
    bmPkt->setHeadSeqNum(m_head_received_seqNum);

    EV << "\tBuffer Map info: " << endl;
    EV  << "\t-- m_bufferSize_chunk = "   << m_bufferSize_chunk       << endl \
        << "\t-- m_bufferStart_seqNum = " << m_bufferStart_seqNum     << endl \
        << "\t-- m_bufferEnd_seqNum = "   << m_bufferEnd_seqNum       << endl \
        << "\t-- m_head_received_seqNum ="  << m_head_received_seqNum   << endl;

    // -- Initialize all of the element of the BM to false
    for (int i = 0; i < m_bufferSize_chunk; i++)
    {
        bmPkt->setBufferMap(i, false);
    }

    // this block has problem?
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;
    for (iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++iter)
    {
        // if the element stores no chunk
        if (iter->m_chunk == NULL)
            continue;

        // if the id of the packet at that element is too "old"
        // if (iter->m_chunk->getSeqNumber() < m_id_bufferStart)
        if (iter->m_chunk->getSeqNumber() < m_bufferStart_seqNum)
            continue;

        // int idx = iter->m_chunk->getSeqNumber() - m_id_bufferStart;
        int idx = iter->m_chunk->getSeqNumber() - m_bufferStart_seqNum;
        #if (__VIDEO_BUFFER_DEBUG__)
            EV << "idx = " << idx << endl;
        #endif
        bmPkt->setBufferMap(idx, true);
//        r_index.record(iter->m_chunk->getSeqNumber());
//        r_index.record(m_id_bufferStart);
//        r_index.record(1);

//        int idx = iter->m_chunk->getSeqNumber() - m_id_bufferStart;
//        bm->setElement(idx, true); // TODO: find out the problem of this line
    }

}

void VideoBuffer::finish(void)
{
    std::vector<STREAM_BUFFER_ELEMENT_T>::iterator iter;

    for(iter = m_streamBuffer.begin(); iter != m_streamBuffer.end(); ++ iter)
    {
        STREAM_BUFFER_ELEMENT_T &elem = *iter;
        if (elem.m_chunk != NULL)
        {
            delete elem.m_chunk;
            elem.m_chunk = NULL;

            if (__VIDEOBUFFER_CROSSCHECK__) --m_nActiveElement;

            // EV << "one element deleted! --- " << endl;
        }
    }

    #if(__VIDEO_BUFFER_DEBUG__)
        EV << "VideoBuffer@finish() :: Number of undeleted objects: " << m_nActiveElement << endl;
    #endif
}

int VideoBuffer::getNumberActiveElement(void)
{
    return m_nActiveElement;
}

int VideoBuffer::getNumberFilledChunk()
{
    int count = 0;
    for (SEQUENCE_NUMBER_T i = m_bufferStart_seqNum; i <= m_bufferEnd_seqNum; ++i)
    {
        if (inBuffer(i))
            ++count;
    }
    EV << "Number of chunks in the video buffer: " << count << endl;
    return count;
}

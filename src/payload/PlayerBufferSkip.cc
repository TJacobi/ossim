//  
// =============================================================================
// OSSIM : A Generic Simulation Framework for Overlay Streaming
// =============================================================================
//
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Project Info: http://www.p2p.tu-darmstadt.de/research/ossim
//
// OSSIM is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// OSSIM is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with 
// this program. If not, see <http://www.gnu.org/licenses/>.

// -----------------------------------------------------------------------------
// Player.cc
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Giang;
// Code Reviewers: -;
// -----------------------------------------------------------------------------
//

#include "PlayerBufferSkip.h"

Define_Module(PlayerBufferSkip)

PlayerBufferSkip::PlayerBufferSkip() {
    // TODO Auto-generated constructor stub
}

PlayerBufferSkip::~PlayerBufferSkip()
{
    if (timer_nextChunk != NULL) { delete cancelEvent(timer_nextChunk); timer_nextChunk = NULL; }
    if (timer_playerStart) cancelAndDelete(timer_playerStart);
}

void PlayerBufferSkip::initialize(int stage)
{
    if (stage == 0)
    {
        sig_chunkHit            = registerSignal("Signal_ChunkHit");
        sig_chunkMiss           = registerSignal("Signal_ChunkMiss");
        sig_chunkSeek           = registerSignal("Signal_ChunkSeek");
        sig_rebuffering_local   = registerSignal("rebuffering_Local");
        sig_rebuffering         = registerSignal("Signal_Rebuffering");
        sig_stall               = registerSignal("Signal_Stall");
        return;
    }

    if (stage != 3)
        return;

    m_CurrentlyStalledChunks = 0;

    // -- pointing to the Video Buffer
    cModule *temp = getParentModule()->getModuleByRelativePath("videoBuffer");
    m_videoBuffer = check_and_cast<VideoBuffer *>(temp);
    //    m_videoBuffer = dynamic_cast<VideoBuffer *>(temp);
//    if (!m_videoBuffer) throw cException("Null pointer for the VideoBuffer module");

    temp = simulation.getModuleByPath("appSetting");
    m_appSetting = check_and_cast<AppSettingDonet *>(temp);
    //    m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
//    if (!m_videoBuffer) throw cException("Null pointer for the AppSetting module");

    temp = simulation.getModuleByPath("globalStatistic");
    m_stat = check_and_cast<StreamingStatistic *>(temp);
    //    m_stat = dynamic_cast<GlobalStatistic *>(temp);
//    if (!m_stat) throw cException("Null pointer for the GlobalStatistic module");

    timer_nextChunk     = new cMessage("PLAYER_TIMER_NEXT_CHUNK");
    timer_playerStart   = new cMessage("PLAYER_TIMER_START");

    // -- Reading parameters from module itself
    param_interval_recheckVideoBuffer = par("interval_recheckVideoBuffer");

    // parameters
    param_BufferSeconds = par("bufferSecondsBeforeStart").doubleValue();
    param_MinimumChunks = par("minimumOfChunksReceivedBeforeStart");
    if (param_MinimumChunks < param_BufferSeconds / m_videoBuffer->getChunkInterval())
        param_MinimumChunks = param_BufferSeconds / m_videoBuffer->getChunkInterval();
    param_MaxStalledChunks = par("max_stalled_chunks");

    // -- for the FSM
    param_max_skipped_chunk = (int) par("max_skipped_chunk").doubleValue();
    param_interval_probe_playerStart = par("interval_probe_playerStart").doubleValue();
    m_state = PLAYER_STATE_IDLE;
    m_skip = 0;

    m_interval_newChunk = m_appSetting->getIntervalNewChunk();
    m_chunkSize  = m_appSetting->getChunkSize();

    // -- State variables
    m_playerStarted = false;
    m_id_nextChunk = -1L;

    // -- Continuity Index

    m_chunks_hit     = 0;
    m_chunks_stalled = 0;
    m_chunks_skipped = 0;
    m_chunks_missed  = 0;

    // -------------------------------------------------------------------------
    // Signals
    // -------------------------------------------------------------------------
    sig_timePlayerStart = registerSignal("Signal_timePlayerStart");

    // -- Schedule the first event for the first chunk
//    scheduleAt(simTime() + par("videoStartTime").doubleValue(), timer_newChunk);

    WATCH(m_videoBuffer);
    WATCH(m_appSetting);
    WATCH(m_chunkSize);
    WATCH(m_interval_newChunk);
}

void PlayerBufferSkip::activate(void)
{
    Enter_Method("activate()");

    EV << "Player activated" << endl;
    if (m_state != PLAYER_STATE_IDLE)
        return;

        //throw cException("Wrong Player state %d while expecting %d", m_state, PLAYER_STATE_IDLE);

    m_state = PLAYER_STATE_BUFFERING;
    scheduleAt(simTime() + param_interval_probe_playerStart, timer_playerStart);
}

void PlayerBufferSkip::finish()
{

}

void PlayerBufferSkip::handleMessage(cMessage *msg)
{
    Enter_Method("handleMessage");

    if (!msg->isSelfMessage())
    {
        throw cException("This module does NOT process external messages!");
        return;
    }

    handleTimerMessage(msg);
}

SEQUENCE_NUMBER_T PlayerBufferSkip::findNextPlayablePosition(){
    double requiredChunks  = param_BufferSeconds / m_videoBuffer->getChunkInterval();

    for (SEQUENCE_NUMBER_T start = max(m_videoBuffer->getBufferStartSeqNum(), m_id_nextChunk); start < m_videoBuffer->getBufferEndSeqNum(); start++){
        bool validposition = true;
        for (SEQUENCE_NUMBER_T offset = start; offset < start + requiredChunks; offset++){
            if (!m_videoBuffer->inBuffer(offset)){
                validposition = false;
                break;
            }
        }

        if (validposition){
            return start;
        }
    }

    return -1;
}

void PlayerBufferSkip::handleTimerMessage(cMessage *msg)
{
    Enter_Method("handleTimerMessage");

    if (msg == timer_playerStart)
    {
        switch (m_state)
        {
        case PLAYER_STATE_BUFFERING:
        {
            if (m_videoBuffer->getNumberOfChunkFill() < param_MinimumChunks)
            {
               EV << "*********************************************************" << endl;
               EV << "Buffer filled not enough! Should not start the player now!" << endl;

                // Probe the status of the buffer again
                scheduleAt(simTime() + param_interval_probe_playerStart, timer_playerStart);
            }
            else
            {
                int nextPosition = findNextPlayablePosition();

                if (nextPosition >= 0){
                    EV << "*********************************************************" << endl;
                    EV << "Player starts now" << endl;
                    EV << "*********************************************************" << endl;
                    // Signal
                    emit(sig_timePlayerStart, simTime().dbl());

                    m_id_nextChunk = nextPosition;
                    if (m_id_nextChunk <= m_videoBuffer->getBufferStartSeqNum())
                        m_id_nextChunk = m_videoBuffer->getBufferStartSeqNum();
                    else if (m_id_nextChunk > m_videoBuffer->getBufferEndSeqNum())
                        throw cException("Expected sequence number %ld is out of range [%ld, %ld]",
                                         m_id_nextChunk,
                                         m_videoBuffer->getBufferStartSeqNum(),
                                         m_videoBuffer->getBufferEndSeqNum());

                    // -- Change state to PLAYING
                    m_state = PLAYER_STATE_PLAYING;

                    // listening support ->
                        std::vector<PlayerListener*>::iterator it;
                        for(it = mListeners.begin(); it != mListeners.end(); it++){
                            (*it)->onPlayerStarted();
                        }
                    // <- listening support

                    scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);
                }else{ // wait until we find a valid starting position ...
                    scheduleAt(simTime() + param_interval_probe_playerStart/10, timer_playerStart); // schedule at a faster rate, because we now have a higher probability that we can start next time
                }
            }
            break;
        }
        default:
        {
            throw cException("Wrong state %d while expecting %d", m_state, PLAYER_STATE_BUFFERING);
        }
        }
    }
    else if (msg == timer_nextChunk)
    {
        if (m_videoBuffer->inBuffer(m_id_nextChunk)){
            m_chunks_hit++;
        }else{
            m_chunks_missed++;
        }

        m_id_nextChunk++;
        scheduleAt(simTime() + m_videoBuffer->getChunkInterval(), timer_nextChunk);
    }
}

void PlayerBufferSkip::startPlayer()
{
    Enter_Method("startPlayer");
    scheduleAt(simTime(), timer_nextChunk);

    EV << "Player starts with chunk " << m_id_nextChunk << endl;

    m_id_nextChunk = m_videoBuffer->getBufferStartSeqNum();

    m_playerStarted = true;

}

SEQUENCE_NUMBER_T PlayerBufferSkip::getCurrentPlaybackPoint(void)
{
    return m_id_nextChunk;
}

SEQUENCE_NUMBER_T PlayerBufferSkip::getPrefferedNextChunk(){
    /*SEQUENCE_NUMBER_T target = m_videoBuffer->getBufferStartSeqNum() + (m_videoBuffer->getSize() / 4);
    if (m_id_nextChunk < target) // always target a position which is atleast around 25% fill
        return target;*/

    if (m_id_nextChunk < m_videoBuffer->getBufferStartSeqNum())
        return m_videoBuffer->getBufferStartSeqNum();

    return m_id_nextChunk;
}

bool PlayerBufferSkip::playerStarted(void)
{
    //return m_playerStarted;
    return (m_state == PLAYER_STATE_PLAYING);
}

bool PlayerBufferSkip::shouldResumePlaying(SEQUENCE_NUMBER_T seq_num)
{
    // !!! Asuming that the seq_num is a valid value within the range [id_start, id_end]

    if (m_videoBuffer->getPercentFillAhead(seq_num) >= 0.5)
        return true;

    return false;
}


void PlayerBufferSkip::addListener(PlayerListener* listener){
    mListeners.push_back(listener);
}
void PlayerBufferSkip::removeListener(PlayerListener* listener){
    std::vector<PlayerListener*>::iterator it;
    for(it = mListeners.begin(); it != mListeners.end(); it++){
        if ( (*it) == listener )
            mListeners.erase(it);
    }
}

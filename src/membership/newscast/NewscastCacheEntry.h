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
// NewscastCacheEntry.h
// -----------------------------------------------------------------------------
// (C) Copyright 2012-2013, by Giang Nguyen (P2P, TU Darmstadt) and Contributors
//
// Contributors: Thorsten Jacobi;
// Code Reviewers: Giang;
// -----------------------------------------------------------------------------
//

// @author Thorsten Jacobi
// @brief a object to be stored in the cache contains: address, agent identifier, timestamp and value of the agent
// @ingroup membership
// @ingroup newscast

#ifndef NEWSCASTCACHEENTRY_H_
#define NEWSCASTCACHEENTRY_H_

#include "IPvXAddress.h"
#include "simtime_t.h"
#include "GossipUserData.h"

//#define NewscastCacheEntry_DoCounting

class NewscastCacheEntry
{
public:
    NewscastCacheEntry();
    virtual ~NewscastCacheEntry();

    // Getter
    IPvXAddress     getAddress()        { return m_address;}
    simtime_t       getTimestamp()      { return m_timestamp;}
    std::string     getAgent()          { return m_agent;}
    /*
     * returns a COPY of the current value or NULL if empty
     */
    GossipUserData* getValue()          { return (m_value != NULL) ? m_value->dup() : NULL;}
    /*
     * returns the pointer to the current value
     */
    GossipUserData* getValuePointer()   { return m_value;}
    bool            hasValue()          { return (m_value != NULL); }
#ifdef NewscastCacheEntry_DoCounting
    static int count;
#endif
    // Setter
    void setAddress(IPvXAddress addr)       { m_address = addr;}
    void setTimestamp(simtime_t timestamp)  { m_timestamp = timestamp;}
    void setAgent(std::string agent)        { m_agent = agent;}
    void setValue(GossipUserData* value)    {
        if (m_value) delete m_value;
        m_value = (value == NULL) ? NULL : value->dup();}

    // estimate
    /*
     * returns a estimated size in bytes of this entry
     */
    long getEstimatedSizeInBits();

    // copy constructor
    NewscastCacheEntry(const NewscastCacheEntry &cSource)
    {
        m_address = cSource.m_address;
        m_timestamp = cSource.m_timestamp;
        m_agent = cSource.m_agent;
        setValue(cSource.m_value);
#ifdef NewscastCacheEntry_DoCounting
        NewscastCacheEntry::count++;
#endif
    }
    // assignment operator
    NewscastCacheEntry& operator= (const NewscastCacheEntry &cSource){
        m_address = cSource.m_address;
        m_timestamp = cSource.m_timestamp;
        m_agent = cSource.m_agent;
        setValue(cSource.m_value);

        return *this;
    }
protected:
    IPvXAddress         m_address;
    simtime_t           m_timestamp;
    std::string         m_agent;
    GossipUserData*     m_value;
};

#endif /* NEWSCASTCACHEENTRY_H_ */

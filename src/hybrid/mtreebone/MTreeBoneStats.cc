#include "MTreeBoneStats.h"

MTreeBoneStats* MTreeBoneStats::theStats = NULL;

Define_Module(MTreeBoneStats);

MTreeBoneStats::MTreeBoneStats() {
    // TODO Auto-generated constructor stub
    m_Src = NULL;
}

MTreeBoneStats::~MTreeBoneStats() {
    if (m_Timer_Report != NULL) cancelAndDelete(m_Timer_Report); m_Timer_Report = NULL;
}

void MTreeBoneStats::initialize(int stage){
    if (stage !=2)
        return;

    MTreeBoneStats::theStats = this;

    // Get the number of stripes
    cModule* temp = simulation.getModuleByPath("appSetting");
    AppSettingDonet* m_appSetting = dynamic_cast<AppSettingDonet *>(temp);
    if (m_appSetting == NULL) throw cException("m_appSetting == NULL is invalid");
    m_Stripes = m_appSetting->getNumberOfStripes();

    // create directory
    std::string command = "mkdir \"stats\"";
    system(command.c_str());

    m_Timer_Report = new cMessage("MTREEBONESTATS_TIMER");

    scheduleAt(simTime() + 1, m_Timer_Report);
}

void MTreeBoneStats::handleMessage(cMessage *msg){
    if (!msg->isSelfMessage())
        return;

    if (msg == m_Timer_Report){

        std::string command = "mkdir \"stats/" + simTime().str() + "\"";
        system(command.c_str());

        for (int stripe = 0; stripe < m_Stripes; stripe++)
            doReportForStripe(stripe);

        scheduleAt(simTime() + 1, m_Timer_Report);
    }
}

void MTreeBoneStats::doReportForStripe(int stripe){
    std::stringstream filename;
    std::ofstream m_Writer;
    // source report
        filename << "stats/" << simTime().str() << "/stripe." << stripe << ".sources";
        m_Writer.open(filename.str().c_str());
        if (m_Src != NULL)
            m_Writer << m_Src->getAddress().str() << " " << endl;
        m_Writer.flush(); m_Writer.close();
    //peers
        filename.str("");
        filename << "stats/" << simTime().str() << "/stripe." << stripe << ".parents";
        m_Writer.open(filename.str().c_str());

        genericList<MTreeBonePeer*>::iterator it;
        for (it = m_Peers.begin(); it != m_Peers.end(); it++)
            if (! (*it)->getParent(stripe).isUnspecified())
                m_Writer << (*it)->getAddress() << " " << (*it)->getParent(stripe).str() << " " << endl;

        m_Writer.flush(); m_Writer.close();
}

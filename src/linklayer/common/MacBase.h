//
// Copyright (C) 2013 OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
// author: Zoltan Bojthe (zbojthe@omnetpp.org)
//


#ifndef __INET_MACBASE_H
#define __INET_MACBASE_H

#include "INETDefs.h"


// Forward declarations:
class IPassiveQueue;

/**
 * Base class for MAC protocols.
 */
class INET_API MacBase : public cSimpleModule
{
  protected:
    /** gate id*/
    //@{
    int upperLayerIn;
    int upperLayerOut;
    int lowerLayerIn;
    int lowerLayerOut;
    //@}

    IPassiveQueue *externalQueue;
    cQueue innerQueue;   //XXX miert nem cPacketQueue?
    int queueLimit;               // max length of innerQueue
    bool hasOutStandingRequest;   //XXX biztos hogy kell?

    //TODO rename variable, TRANSLATE
    // azt jelzi, hogy a handleMessage vagy processRequestPackets fuggvenyekben belul vagyunk-e:
    //  - ha igen, akkor innerqueue eseten eleeg beallitani a flag-et, hogy kerunk csomagot.
    //  - ha nem, akkor meg kell hivni utana a processRequestPackets fuggvenyt is.
    bool inside;     //XXX ilyenre nem szabad hogy szukseg legyen!

    static simsignal_t packetSentToLowerSignal;         // at start of transmit
    static simsignal_t packetReceivedFromLowerSignal;
    static simsignal_t packetSentToUpperSignal;         // at start of transmit
    static simsignal_t packetReceivedFromUpperSignal;

  private:
    void processRequestedPackets();   //XXX valszeg nem kell
    void pushMsgToInnerQueue(cMessage *msg);
    void requestNextMsgFromUpperQueue();   //XXX rename: requestNextMessage

  public:
    MacBase() { externalQueue = NULL; hasOutStandingRequest = false; }

  protected:
    /**
     * Initialization of the module and some variables.
     *
     * ennek leszarmazottjaban kell az innerQueue compareFunc-ot beallitani, ha szukseges. (ETHERNET!!!)
     */
    virtual void initialize(int);

    virtual void initializeQueueModule();

    virtual void handleMessage(cMessage *msg);

    /**
     * Returns a packet from the queue, or NULL if none is available
     */
    virtual cPacket *getPacketFromQueue();

    /** @name Functions to redefine in subclasses */
    //@{
    /** Handle self messages such as timers */
    virtual void handleSelfMsg(cMessage *msg) = 0;

    /** Handle messages from upper layer queue */
    virtual void handleMsgFromUpperQueue(cMessage *msg) = 0;

    /** Handle packets from lower layer */
    virtual void handleMsgFromLL(cMessage *msg) = 0;  //XXX rename: ...FromLowerLayer
    //@}

    /** @name Convenience Functions*/
    //@{
    /** Sends a message to the lower layer */
    virtual void sendDown(cMessage *msg);

    /** Sends a message to the upper layer */
    virtual void sendUp(cMessage *msg);
    //@}
};

#endif


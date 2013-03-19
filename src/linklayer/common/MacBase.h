/* -*- mode:c++ -*- ********************************************************
 * file:        MacBase.h
 *
 * derived by Andras Varga using decremental programming from BasicMacLayer.h,
 * which had the following copyright:
 *
 * author:      Daniel Willkomm
 *
 * copyright:   (C) 2004 Telecommunication Networks Group (TKN) at
 *              Technische Universitaet Berlin, Germany.
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU Lesser General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 **************************************************************************/


#ifndef __INET_MACBASE_H
#define __INET_MACBASE_H

#include "INETDefs.h"


// Forward declarations:
class IPassiveQueue;

/**
 * Base class for MAC protocols.
 *
 * @author Zoltan Bojthe
 */
class INET_API MacBase : public cSimpleModule
{
  protected:
    /** @brief gate id*/
    //@{
    int upperLayerIn;
    int upperLayerOut;
    int lowerLayerIn;
    int lowerLayerOut;
    //@}

    IPassiveQueue *externalQueue;
    cQueue innerQueue;
    int queueLimit;               // max length of innerQueue
    bool hasOutStandingRequest;

    //TODO rename variable, TRANSLATE
    // azt jelzi, hogy a handleMessage vagy processRequestPackets fuggvenyekben belul vagyunk-e:
    //  - ha igen, akkor innerqueue eseten eleeg beallitani a flag-et, hogy kerunk csomagot.
    //  - ha nem, akkor meg kell hivni utana a processRequestPackets fuggvenyt is.
    bool inside;

    static simsignal_t packetSentToLowerSignal;         // at start of transmit
    static simsignal_t packetReceivedFromLowerSignal;
    static simsignal_t packetSentToUpperSignal;         // at start of transmit
    static simsignal_t packetReceivedFromUpperSignal;

  public:
    MacBase() { externalQueue = NULL; hasOutStandingRequest = false; }
  protected:
    /**
     * Initialization of the module and some variables.
     *
     * ennek leszarmazottjaban kell az innerQueue compareFunc-ot beallitani, ha szukseges. (ETHERNET!!!)
     */
    virtual void initialize(int);

    /** @brief Called every time a message arrives*/
    virtual void handleMessage(cMessage *msg);

  private:
    void processRequestedPackets();
    void pushMsgToInnerQueue(cMessage *msg);

  protected:
    virtual void initializeQueueModule();

    /**
     * @name Handle Messages
     * @brief Functions to redefine by the programmer
     */
    //@{
    /** @brief Handle self messages such as timers */
    virtual void handleSelfMsg(cMessage *msg) = 0;

    /** @brief Handle messages from upper layer queue */
    virtual void handleMsgFromUpperQueue(cMessage *msg) = 0;

    /** Handle packets from lower layer */
    virtual void handleMsgFromLL(cMessage *msg) = 0;
    //@}

    /** @brief Request a packet from upper layer queue */
    virtual void requestNextMsgFromUpperQueue();

    /** @name Convenience Functions*/
    //@{
    /** @brief Sends a message to the lower layer */
    virtual void sendDown(cMessage *msg);

    /** @brief Sends a message to the upper layer */
    virtual void sendUp(cMessage *msg);
    //@}
};

#endif


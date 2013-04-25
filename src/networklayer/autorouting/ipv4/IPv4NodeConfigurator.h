//
// Copyright (C) 2012 Opensim Ltd
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//
//
// Authors: Levente Meszaros
//

#ifndef __INET_IPV4NODECONFIGURATOR_H
#define __INET_IPV4NODECONFIGURATOR_H

#include "INETDefs.h"
#include "ILifecycle.h"
#include "IInterfaceTable.h"
#include "IRoutingTable.h"
#include "IPv4NetworkConfigurator.h"
#include "NodeStatus.h"

/**
 * This class is responsible for the static configuration of a particular network node.
 */
class IPv4NodeConfigurator : public cSimpleModule, public ILifecycle {
    protected:
        cModule *node;
        NodeStatus *nodeStatus;
        IInterfaceTable *interfaceTable;
        IRoutingTable *routingTable;
        IPv4NetworkConfigurator *networkConfigurator;

    public:
        virtual bool handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback);

    protected:
        virtual int numInitStages() const { return 4; }
        virtual void handleMessage(cMessage *msg) { throw cRuntimeError("this module doesn't handle messages, it runs only in initialize()"); }
        virtual void initialize(int stage);

        virtual void configureNode();
        virtual bool isNodeUp();
};

#endif

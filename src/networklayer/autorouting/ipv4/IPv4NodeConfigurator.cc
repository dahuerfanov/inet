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

#include "IPv4NodeConfigurator.h"
#include "ModuleAccess.h"
#include "InterfaceTableAccess.h"
#include "RoutingTableAccess.h"
#include "NodeOperations.h"

Define_Module(IPv4NodeConfigurator);

void IPv4NodeConfigurator::initialize(int stage)
{
    if (stage == 0) {
        node = findContainingNode(this);
        interfaceTable = InterfaceTableAccess().get();
        routingTable = RoutingTableAccess().get();
        nodeStatus = dynamic_cast<NodeStatus *>(findContainingNode(this)->getSubmodule("status"));
        networkConfigurator = dynamic_cast<IPv4NetworkConfigurator *>(findModuleWherever("configurator", simulation.getSystemModule()));
    }
    else if (stage == 3)
    {
        if (isNodeUp())
            configureNode();
    }
}

bool IPv4NodeConfigurator::handleOperationStage(LifecycleOperation *operation, int stage, IDoneCallback *doneCallback)
{
    Enter_Method_Silent();
    if (dynamic_cast<NodeStartOperation *>(operation)) {
        if (stage == NodeStartOperation::STAGE_NETWORK_LAYER + 1)
            configureNode();
    }
    else if (dynamic_cast<NodeShutdownOperation *>(operation)) ;
    else if (dynamic_cast<NodeCrashOperation *>(operation)) ;
    else throw cRuntimeError("Unsupported lifecycle operation '%s'", operation->getClassName());
    return true;
}

void IPv4NodeConfigurator::configureNode()
{
    for (int i = 0; i < interfaceTable->getNumInterfaces(); i++)
        networkConfigurator->assignAddress(interfaceTable->getInterface(i));
    networkConfigurator->addStaticRoutes(routingTable);
}

bool IPv4NodeConfigurator::isNodeUp()
{
    return !nodeStatus || nodeStatus->getState() == NodeStatus::UP;
}

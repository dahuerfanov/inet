// the original file was included in the ~UDP sockets example

package inet.transport.rtp;

import inet.applications.rtpapp.RTPApplication;
import inet.base.NotificationBoard;
import inet.linklayer.IWiredNic;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;
import inet.nodes.inet.NodeBase;
import inet.status.NodeStatus;
import inet.transport.udp.UDP;
import ned.IdealChannel;


module RTPHost extends NodeBase
{
    parameters:
        string profileName;
        string destinationAddress;
        int portNumber;
        double bandwidth;
        string fileName;
        int payloadType;
        // network information
        @display("i=device/pc2");

    submodules:
        application: RTPApplication {
            parameters:
                profileName = profileName;
                destinationAddress = destinationAddress;
                portNumber = portNumber;
                bandwidth = bandwidth;
                fileName = fileName;
                payloadType = payloadType;
                @display("p=286,43");
        }
        rtp: RTP {
            @display("p=288,111");
        }
        rtcp: RTCP {
            @display("p=368,111");
        }
        udp: UDP {
            parameters:
                @display("p=327,180");
            gates:
                appIn[2];
                appOut[2];
        }

    connections allowunconnected:
        // transport connections
        application.rtpOut --> rtp.appIn;
        application.rtpIn <-- rtp.appOut;

        rtp.udpOut --> udp.appIn[0];
        rtp.udpIn <-- udp.appOut[0];

        rtcp.udpOut --> udp.appIn[1];
        rtcp.udpIn <-- udp.appOut[1];

        networkLayer.udpOut --> udp.ipIn;
        networkLayer.udpIn <-- udp.ipOut;

        rtp.rtcpOut --> IdealChannel --> rtcp.rtpIn;
        rtcp.rtpOut --> IdealChannel --> rtp.rtcpIn;
}


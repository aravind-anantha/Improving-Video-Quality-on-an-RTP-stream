# Improving-Video-Quality-on-an-RTP-stream

MPEG video frames contain 4 types of pictures, I, P, B and D. Loss of I and P pictures affect all other pictures within the same frame, causing more severe consequences than losing B or D picture packets. However, these priority levels of video picture packets are not known to the network forwarding engines. If congestion happen at the routers where there are video packets being forwarded, they will not honor the importance of I picture packets and drop them with the same rule as with P and B picture packets.

The aim of this project is to improve video quality by prioritizing I picture packets. The importance of I picture packets is made known to edge routers, so the edge routers will drop such packets with lower probability than other video packets when edge routers get congested. The rationale to enable this feature on edge routers but not core routers is that the core routers are busy forwarding packets all the time, so they can not do any extra work other than forwarding.

The most significant part is tagging I picture packets in IP headers so that routers can read this information. This function will be realized by a Linux kernel module using Netfilter hooks. This kernel module needs to first examine whether the packets are tagged or not. If already tagged, the module does nothing on it. If not tagged, the module will read the Picture-Type (3 bits) field in the MPEG header where frame structure is available as RTP payload as defined in RFC 2250. The module will then tag the I packets by setting the DSCP field to a value. The folder marking module has the kernel code that identifies a video packet from an RTP stream from all incoming IP traffic.

The entire project was done on linux machines; the topology used can be found in topology.png and all the configurations done on the machines can be found in the folder config scripts. 

The video servers generate video streams and the client receives them. Edge router E1 has the marking module which tags the I packets and gives them a higher priority when the network is congested.

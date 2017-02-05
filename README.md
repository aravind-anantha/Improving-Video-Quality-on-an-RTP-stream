# Improving-Video-Quality-on-an-RTP-stream

MPEG video frames contain 4 types of pictures, I, P, B and D. Loss of I and P pictures affect all other pictures within the same frame, causing more severe consequences than losing B or D picture packets. However, these priority levels of video picture packets are not known to the network forwarding engines. If congestion happen at the routers where there are video packets being forwarded, they will not honor the importance of I picture packets and drop them with the same rule as with P and B picture packets.

The aim of this project is to improve video quality by prioritizing I picture packets. The importance of I picture packets is made known to edge routers, so the edge routers will drop such packets with lower probability than other video packets when edge routers get congested. The rationale to enable this feature on edge routers but not core routers is that the core routers are busy forwarding packets all the time, so they can not do any extra work other than forwarding.


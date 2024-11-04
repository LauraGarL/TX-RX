#ifndef TX_RX_H_
#define TX_RX_H_

using namespace std;

#include <iostream>
#include "systemc.h"
#include "tx_moore.h"
#include "rx_moore.h"

#define DATA_WIDTH (8)
#define nPACKETS_WIDTH (32)
#define noFlitPacket (4)

SC_MODULE(TXRX){
	sc_in_clk 						i_clock;				//Input: System Clock
	sc_in<bool>						i_reset;				//Input: System Reset
	sc_in<bool>						i_packetReady;			//Input: Indicates that there is a packet to send
	sc_in<bool>						i_ready_to_receive;		//Input: Indicates readiness to receive a packet
	sc_in<bool>						i_fifo_empty;			//Input: Indicates that the FIFO is empty
	sc_in<bool>						i_fifo_full;			//Input: Indicates that the FIFO is full
	sc_in<sc_uint<DATA_WIDTH>>		i_fifo_dataOut;			//Input: Data_out from FIFO
	sc_out<bool> 					o_fifo_pop;				//Output: Remove an element from  FIFO
	sc_out<bool> 					o_fifo_push;			//Output: Push an element from  FIFO
	sc_out<bool> 					o_packet_sent;			//Output: Indicates that a packet has been sent by TX
	sc_out<bool> 					o_packet_received;		//Output: Indicates that a packet has been received by RX
	sc_out<sc_uint<nPACKETS_WIDTH>> o_nPackets_sent;		//Output: Indicates the packet number that has been sent by TX
	sc_out<sc_uint<nPACKETS_WIDTH>> o_nPackets_received;	//Output: Indicates the packet number that has been received by RX
	sc_out<bool>					o_OnOff;			//Output: Indicates availability to receive a flit
	sc_out<bool> 					o_Req;				//Output: Indicates whether the request was granted
	sc_out<bool> 					o_SoP;				//Output: Indicates the start of a packet

	TX *Transmitter;
	RX *Receiver;

	sc_signal<bool> Req_fromTX_toRX;
	sc_signal<bool> SoP_fromTX_toRX;
	sc_signal<bool> OnOff_fromRX_toTX;


	 void concurrent(){
		 o_OnOff.write(OnOff_fromRX_toTX);
		 o_SoP.write(SoP_fromTX_toRX);
		 o_Req.write(Req_fromTX_toRX);
	 }


			    SC_CTOR(TXRX): //	Labeling ports
			    	i_clock("i_clock"),							//Input: System Clock
			    	i_reset("i_reset"),							//Input: System Reset
			    	i_packetReady("i_packetReady"),				//Input: Indicates that there is a pack to send
					i_ready_to_receive("i_ready_to_receive"),	//Input: Indicates readiness to receive a packet
					i_fifo_empty("i_fifo_empty"),				//Input: Indicates that the FIFO is empty
					i_fifo_full("i_fifo_full"),					//Input: Indicates that the FIFO is full
					i_fifo_dataOut("i_fifo_dataOut"),			//Input: Data_out from FIFO
					o_fifo_pop("o_fifo_pop"),					//Output: Remove an element from  FIFO
					o_fifo_push("o_fifo_push"),					//Output: Push an element from  FIFO
					o_packet_sent("o_packet_sent"),				//Output: Indicates that a packet has been sent by TX
					o_packet_received("o_packet_received"),		//Output: Indicates that a packet has been received by RX
					o_nPackets_sent("o_nPackets_sent"),			//Output: Indicates the packet number that has been sent by TX
					o_nPackets_received("o_nPackets_received"),	//Output: Indicates the packet number that has been received by RX
					o_OnOff("o_OnOff"),
					o_Req("o_Req"),
					o_SoP("o_SoP")
			    {

			    	Transmitter = new TX("u1");
			    		Transmitter->i_clock(i_clock);
						Transmitter->i_reset(i_reset);		//Input: System Reset
						Transmitter->i_packetReady(i_packetReady);	//Input: Indicates that there is a pack to send
						Transmitter->i_OnOff(OnOff_fromRX_toTX);		//Input: Indicates availability to receive a flit
						Transmitter->i_fifo_empty(i_fifo_empty);	//Input: Indicates that the FIFO is empty
						Transmitter->i_fifo_dataOut(i_fifo_dataOut);	//Input: Data_out from FIFO
						Transmitter->o_Req(Req_fromTX_toRX);			//Output: Indicates whether the request was granted
						Transmitter->o_SoP(SoP_fromTX_toRX);			//Output: Indicates the start of a packet
						Transmitter->o_fifo_pop(o_fifo_pop);
						Transmitter->o_nPackets_sent(o_nPackets_sent);
						Transmitter->o_packet_sent(o_packet_sent);

					Receiver = new RX("u2");
					    Receiver->i_clock(i_clock);
					    Receiver->i_reset(i_reset);
					    Receiver->i_Req(Req_fromTX_toRX);
					    Receiver->i_SoP(SoP_fromTX_toRX);
					    Receiver->i_fifo_full(i_fifo_full);
					    Receiver->i_fifo_dataOut(i_fifo_dataOut);
					    Receiver->i_ready_to_receive(i_ready_to_receive);
					    Receiver->o_OnOff(OnOff_fromRX_toTX);
					    Receiver->o_fifo_push(o_fifo_push);
					    Receiver->o_nPackets_received(o_nPackets_received);
					    Receiver->o_packet_received(o_packet_received);
			    }
			};
#endif /* TX_RX_H_ */

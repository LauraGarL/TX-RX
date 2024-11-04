//============================================================================
// Name        : handshakeTX.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <iostream>
#include "systemc.h"

#include "tx_rx.h"
using namespace std;

int sc_main(int argc, char* argv[]) {
    sc_signal<bool> i_clock;
    sc_signal<bool> i_reset;
    sc_signal<bool> i_packetReady;
	sc_signal<bool>						i_ready_to_receive;		//Input: Indicates readiness to receive a packet
	sc_signal<bool>						i_fifo_empty;			//Input: Indicates that the FIFO is empty
	sc_signal<bool>						i_fifo_full;			//Input: Indicates that the FIFO is full
	sc_signal<sc_uint<DATA_WIDTH>>		i_fifo_dataOut;			//Input: Data_out from FIFO
	sc_signal<bool> 					o_fifo_pop;				//Output: Remove an element from  FIFO
	sc_signal<bool> 					o_fifo_push;			//Output: Push an element from  FIFO
	sc_signal<bool> 					o_packet_sent;			//Output: Indicates that a packet has been sent by TX
	sc_signal<bool> 					o_packet_received;		//Output: Indicates that a packet has been received by RX
	sc_signal<sc_uint<nPACKETS_WIDTH>> o_nPackets_sent;		//Output: Indicates the packet number that has been sent by TX
	sc_signal<sc_uint<nPACKETS_WIDTH>> o_nPackets_received;	//Output: Indicates the packet number that has been received by RX
    sc_signal<bool> o_Req;
    sc_signal<bool> o_SoP;
    sc_signal<bool> o_OnOff;

    TXRX transmission("transmission");
    transmission.i_clock(i_clock);
    transmission.i_reset(i_reset);
    transmission.i_packetReady(i_packetReady);
    transmission.i_ready_to_receive(i_ready_to_receive);
    transmission.i_fifo_dataOut(i_fifo_dataOut);
    transmission.i_fifo_empty(i_fifo_empty);
    transmission.i_fifo_full(i_fifo_full);
    transmission.o_fifo_pop(o_fifo_pop);
    transmission.o_fifo_push(o_fifo_push);
    transmission.o_nPackets_received(o_nPackets_received);
    transmission.o_nPackets_sent(o_nPackets_sent);
    transmission.o_packet_received(o_packet_received);
    transmission.o_packet_sent(o_packet_sent);
    transmission.o_Req(o_Req);
    transmission.o_OnOff(o_OnOff);
    transmission.o_SoP(o_SoP);

    sc_start(10,SC_NS);	//El entero nos dice cuántas unidades de tiempo se va a ejecutar. Es un método necesario para comenzar una simulación.

    		sc_trace_file *wf = sc_create_vcd_trace_file("vcd_myTX-RX");

    		sc_trace(wf, i_clock, "i_clock");
    		sc_trace(wf, i_reset, "i_reset");
    		sc_trace(wf, i_packetReady, "i_packetReady");
    		sc_trace(wf, i_ready_to_receive, "i_ready_to_receive");
    		sc_trace(wf, i_fifo_dataOut,"i_fifo_dataOut");
    		sc_trace(wf, i_fifo_empty,"i_fifo_empty");
    		sc_trace(wf, i_fifo_full,"i_fifo_full");
    		sc_trace(wf, o_fifo_pop, "o_fifo_pop");
    		sc_trace(wf, o_fifo_push,"o_fifo_push");
    		sc_trace(wf, o_nPackets_received,"o_nPackets_received");
    		sc_trace(wf, o_packet_received,"o_packet_received");
    		sc_trace(wf, o_nPackets_received,"o_nPackets_sent");
    		sc_trace(wf, o_packet_received,"o_packet_sent");
    		sc_trace(wf, transmission.Transmitter->state, "TX State");
    		sc_trace(wf, transmission.Receiver->state, "RX State");
       		sc_trace(wf, transmission.Transmitter->count, "TX Count");
        	sc_trace(wf, transmission.Receiver->count, "RX Count");
        	sc_trace(wf, transmission.OnOff_fromRX_toTX, "OnOff_fromRX_toTX");
        	sc_trace(wf, transmission.Req_fromTX_toRX, "Req_fromTX_toRX");
        	sc_trace(wf, transmission.SoP_fromTX_toRX, "SoP_fromTX_toRX");

    		i_reset = 0;
    		i_packetReady = 0;
    		i_ready_to_receive = 0;
    		i_fifo_empty = 0;
    		i_fifo_full = 0;
    		i_fifo_dataOut = 0;

    		for(int i=0; i<2; i++){
    			i_clock = 0; sc_start(10,SC_NS);
    			i_clock = 1; sc_start(10,SC_NS);}

			i_reset = 1; //Assert the reset
			i_fifo_empty = 1;

			cout << "@" << sc_time_stamp() <<"\t Asserting reset ...TX\n" << endl;

    		for(int i=0; i<2; i++){
    			i_clock = 0; sc_start(10,SC_NS);
    			i_clock = 1; sc_start(10,SC_NS);}

			i_reset = 0; //De-Assert the reset

			cout << "@" << sc_time_stamp() <<"\t De-Asserting reset\n" << endl;

			cout << "@" << sc_time_stamp() <<"\t All configurations:\n" << endl; //Quiero poner todas las combinaciones de las solicitudes.
			i_packetReady = 0;
			i_fifo_empty = 0;
			//i_fifo_dataOut = 0x0000010E; //270 - 0001 0000 1110
			i_fifo_dataOut = 0x00000004; //4 - 0000 0000 0100 //n=4 flits a enviara

    		for(int i=0; i<2; i++){
    			i_clock = 0; sc_start(10,SC_NS);
    			i_clock = 1; sc_start(10,SC_NS);}

			//Paquete listo y ya en la entrada i_fifo_dataOut
    		i_packetReady = 1;
    		i_fifo_empty = 0;

    		for(int i=0; i<2; i++){
    			i_clock = 0; sc_start(10,SC_NS);
    			i_clock = 1; sc_start(10,SC_NS);}
    		i_packetReady = 0;
    		i_fifo_empty = 1;

//    		for(int i=0; i<40 ; i++){
				for(int i=0; i<10; i++){
					i_clock = 0; sc_start(10,SC_NS);
					i_clock = 1; sc_start(10,SC_NS);}

				i_ready_to_receive = 1;
				i_fifo_empty = 0;

				for(int i=0; i<2; i++){
					i_clock = 0; sc_start(10,SC_NS);
					i_clock = 1; sc_start(10,SC_NS);}

				i_ready_to_receive = 0;
				i_fifo_empty = 0;
//    		}

				for(int i=0; i<40; i++){
					i_clock = 0; sc_start(10,SC_NS);
					i_clock = 1; sc_start(10,SC_NS);}

cout << "@" << sc_time_stamp() << "Terminating simulation" << endl;
sc_close_vcd_trace_file(wf);
    return 0;
}
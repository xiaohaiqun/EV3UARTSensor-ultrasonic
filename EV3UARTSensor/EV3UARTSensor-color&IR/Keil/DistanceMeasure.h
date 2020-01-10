#include "MS51_16K.H"
#ifndef DISTANCEMEASURE_H
	#define DISTANCEMEASURE_H
	
	#define NOP() {_nop_();_nop_();_nop_();_nop_();}
	sbit Trig=P0^5;
	
	short int DistanceMeasure();
#endif
/*
 * F2837xD_CMPSS_defines.h
 *
 *  Created on: 18 Feb 2020
 *      Author: Dinh Ngoc
 */

#ifndef F2837xD_CMPSS_DEFINES_H_
#define F2837xD_CMPSS_DEFINES_H_

//
// Defines
//
//definitions for selecting DACH reference
#define REFERENCE_VDDA_CMPSS     0
#define REFERENCE_VDAC_CMPSS     1

//definitions for COMPH input selection
#define NEGIN_DAC          0
#define NEGIN_PIN          1

//definitions for CTRIPH/CTRIPOUTH output selection
#define CTRIP_ASYNCH       0
#define CTRIP_SYNCH        1
#define CTRIP_FILTER       2
#define CTRIP_LATCH        3





#endif /* 4_DEVICE_F2837XD_COMMON_INCLUDE_F2837XD_CMPSS_DEFINES_H_ */

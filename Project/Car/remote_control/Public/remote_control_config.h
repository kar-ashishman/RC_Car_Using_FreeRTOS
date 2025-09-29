/*
 * remote_control_config.h
 *
 *  Created on: Sep 29, 2025
 *      Author: kar-a
 */

#ifndef REMOTE_CONTROL_PUBLIC_REMOTE_CONTROL_CONFIG_H_
#define REMOTE_CONTROL_PUBLIC_REMOTE_CONTROL_CONFIG_H_

/* function proto */
	void remote_control(void);

/* Total number of remote signals queued by kernel to be serviced later in FIFO */
	#define REMOTE_QUEUE_LENGTH 									10
	#define REMOTE_SIGNAL_BYTE_SIZE

#endif /* REMOTE_CONTROL_PUBLIC_REMOTE_CONTROL_CONFIG_H_ */

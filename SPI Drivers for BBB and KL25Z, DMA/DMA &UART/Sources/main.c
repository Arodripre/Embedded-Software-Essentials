/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MKL25Z4.h"
#include "uart.h"
#include"circbuf.h"
#include"data.h"
#include"log.h"
#include"memory.h"
#include"main.h"
#include"dma.h"
#include"profile.h"
#include"message.h"
#include<malloc.h>
#define sz 5000
static int i = 0;

int main(void)
{
	uart_init(57600);								// Initialize UART
	pit_init();										// Initialize the PIT timer for profiling

	uint8_t source[sz]="ABCDEFGHI",dest[sz];

	//LOG0("\n\r100 Bytes memmove_dma 8-bit \n\r");
	//LOG0("\n\r5000 Bytes memzero_dma \n\r");

	memmove_dma(source,dest,sz);					// Memmove using DMA
	memzero_dma(source, sz);						// Memzero using DMA
	msg_t m;										// Define variable of message structure
	uint32_t flag=1,flag1=1,i=0;
/*Commands: Command ID 00:LED ON 
					   02:LED OFF
					   03: Change color and brightness 
	Command Structure: Command ID, Message Length, Either color ID and Brightness, Checksum 	*/
	while(1)			// Infinite loop for continuous operation
		{
			state ec=buff_empty(&rx_buf);					// Checking  Rx buffer for empty

			if(ec==buf_not_empty && (flag1==1))				// If buffer is not empty and lenth is not read then
			{												// read command ID first and then read message length
				uint8_t con=read_data(&rx_buf);
				if(flag==1 && con>0 && con<4)
				{
					m.command = con;
					flag=0;
				}
				else if(flag1==1 && flag==0)
				{
					m.length = con;
					flag1=0;
				}

		    }
			// After command command ID and Length is recieved then Rea data into mesage structure
			else if(ec==buf_not_empty && i<m.length && m.length<MAX_DATA_SIZE)
			{
				uint8_t con=read_data(&rx_buf);
				m.data[i]=con;
				i++;
			}
			else if(ec==buf_not_empty && i==m.length)
			{
				uint8_t con=read_data(&rx_buf);
				m.checksum = con;
				flag=flag1=1;
				i=0;
				decode_msg(&m);

			}

		}



}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////

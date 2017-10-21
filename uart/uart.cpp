
//========================================================================================
//
// File Name    : tb_uart.h
// Description  : Testbench
// Release Date : 02/12/2014
// Author       : PolyU DARCLab
//                Nandeesh Veeranna
//
// Revision History
//---------------------------------------------------------------------------------------
// Date         Version    Author     		 Description
//---------------------------------------------------------------------------------------
//02/12/2014      1.0      PolyU DARClab    UART main description
//=======================================================================================


#include "uart.h"
#include "define.h"
void uart::uart_main(void){

    //local variables
    sc_uint<8> r_data_buff,t_data_buff;//LCOV_EXCL_BR_LINE
    bool rx_buff,tx_buff;
    bool trans_flag,recv_flag;
    bool rx_flag,tx_flag,active;
    int i,j,cnt=0;
    bool t_ack_tmp;
    trans_flag = 0;
    recv_flag = 0;
    tx_flag = 0;
    rx_flag = 0;
    r_data_buff = "00000000";//LCOV_EXCL_BR_LINE
    t_data_buff = "11111111";//LCOV_EXCL_BR_LINE
    active = 0;

    //wait for reset

    wait();//LCOV_EXCL_BR_LINE

    while(true)
    {
        while(!active)  //wait_until(active == 1)
        {
            if(cnt==10416) //9600 baudrate
            //if(cnt==100)
            {
                active = 1;
                cnt=0;
            }

            else
            {
                active= 0;
                cnt+=1;
            }
            t_ack_tmp = t_ack.read();//LCOV_EXCL_BR_LINE
            if(!tx_flag && t_ack_tmp == 1)  //get transmit signal
            {
                tx_flag = 1;
                t_data_buff = t_data.read();//LCOV_EXCL_BR_LINE
                cout<<endl<<t_data_buff<<endl;//LCOV_EXCL_BR_LINE
            }

            if(rx_flag == 1) //clear receiving signal
            {
                rx_flag = 0;
                r_ack.write(rx_flag);//LCOV_EXCL_BR_LINE
            }
        }

        /************************************************************/
        /***************** Receiving Operation ********************/
        /************************************************************/

        rx_buff = rx.read();//LCOV_EXCL_BR_LINE
        if (rx_buff == STARTBIT && !recv_flag) {
            recv_flag = 1;
            i = 0;
        } // Wait for recieved start-bit

        else if (recv_flag==1 && i != WORDLEN) {
            r_data_buff[i] = rx_buff;//LCOV_EXCL_BR_LINE
            i++;
        } // store recived data into buffer once start-bit is dectected

        else if (recv_flag && i == WORDLEN) {
            if (rx_buff == STOPBIT) {
                r_data.write(r_data_buff);//LCOV_EXCL_BR_LINE
                rx_flag = 1;
                r_ack.write(rx_flag);//LCOV_EXCL_BR_LINE
            }
            recv_flag = 0;
        } // data is output when stop-bit is recieved, otherwise abandon it

        else {
            recv_flag = 0;
            i = 0;
        }
        /************************************************************/
        /**************** Transmitting Operation ******************/
        /************************************************************/
        if (!tx_flag && (t_ack.read()==1)) {
            tx_flag = 1;
            // get transmitting signal
            t_data_buff =t_data.read(); //LCOV_EXCL_BR_LINE
        }

        if (tx_flag && !trans_flag) {  // transmitting starts and sending start-bit
            j = 0;
            trans_flag = 1;
            tx_buff = STARTBIT;
            tx.write(tx_buff);//LCOV_EXCL_BR_LINE
        }

        else if (trans_flag && j != WORDLEN) { // sending transmitted data
            tx_buff = t_data_buff[j];//LCOV_EXCL_BR_LINE
            tx.write(tx_buff);//LCOV_EXCL_BR_LINE
            j++;
        }

        else if (trans_flag && j == WORDLEN) { // transmitting ends and sending stop-bit
            trans_flag = 0;
            tx_flag = 0;
            j = 0;
            tx_buff = STOPBIT;
            tx.write(tx_buff);//LCOV_EXCL_BR_LINE
            //active=0;
            //wait();
        }

        else {
            trans_flag = 0;
            tx_flag = 0;
            j = 0;
        }

        wait();//LCOV_EXCL_BR_LINE
    }
}//LCOV_EXCL_BR_LINE

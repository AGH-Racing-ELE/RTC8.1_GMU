#include "CAN_LIB_HW.h"

uint8_t retransmision_counter;

void CAN_HW_Init(){

	static CAN_FilterTypeDef sFilterConfig;
	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig.FilterIdHigh = 0x0000;
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;
	sFilterConfig.SlaveStartFilterBank = 14;
	if (HAL_CAN_ConfigFilter(&CAN_HANDLE, &sFilterConfig) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}

	if (HAL_CAN_Start(&CAN_HANDLE) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

	if (HAL_CAN_ActivateNotification(&CAN_HANDLE, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_RX_FIFO0_OVERRUN
			| CAN_IT_RX_FIFO0_FULL | CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_ERROR) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}

}

inline uint32_t CAN_HW_GetTickMs(){
	return HAL_GetTick();
}

uint8_t CAN_HW_SendFrame(struct can_frame* pFrame){

	/* Check if mailbox is free */
    if ((CAN_HANDLE.Instance->TSR & CAN_TSR_TME0) == 0U) return 0;

    /* Set up the Id */
    if(pFrame->core.isExtId){
    	CAN_HANDLE.Instance->sTxMailBox[0].TIR = ((pFrame->core.id << CAN_TI0R_EXID_Pos) | CAN_ID_EXT | CAN_RTR_DATA);
    }
    else{
        CAN_HANDLE.Instance->sTxMailBox[0].TIR = ((pFrame->core.id << CAN_TI0R_STID_Pos) | CAN_RTR_DATA);
    }

    /* Set up the DLC */
    CAN_HANDLE.Instance->sTxMailBox[0].TDTR = pFrame->core.dlc;
    /* Set up the data field */
    WRITE_REG(CAN_HANDLE.Instance->sTxMailBox[0].TDHR,
              ((uint32_t)pFrame->core.data[7] << CAN_TDH0R_DATA7_Pos) |
              ((uint32_t)pFrame->core.data[6] << CAN_TDH0R_DATA6_Pos) |
              ((uint32_t)pFrame->core.data[5] << CAN_TDH0R_DATA5_Pos) |
              ((uint32_t)pFrame->core.data[4] << CAN_TDH0R_DATA4_Pos));
    WRITE_REG(CAN_HANDLE.Instance->sTxMailBox[0].TDLR,
              ((uint32_t)pFrame->core.data[3] << CAN_TDL0R_DATA3_Pos) |
              ((uint32_t)pFrame->core.data[2] << CAN_TDL0R_DATA2_Pos) |
              ((uint32_t)pFrame->core.data[1] << CAN_TDL0R_DATA1_Pos) |
              ((uint32_t)pFrame->core.data[0] << CAN_TDL0R_DATA0_Pos));

    /* Request transmission */
    SET_BIT(CAN_HANDLE.Instance->sTxMailBox[0].TIR, CAN_TI0R_TXRQ);

    return 1;
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan){
	CAN_TxCompleteCallback();
	retransmision_counter = 0;
}

void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan){
	CAN_TxCompleteCallback();
	retransmision_counter = 0;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){

	static struct can_msg msg;
	static CAN_RxHeaderTypeDef RxHeader;
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, msg.data);
	msg.dlc = RxHeader.DLC;
	msg.id = RxHeader.StdId;
	CAN_ReceiveCallback(&msg);
}

void HAL_CAN_RxFifo0FullCallback(CAN_HandleTypeDef *hcan){

}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan){

	static uint32_t errorcode;
	errorcode = HAL_CAN_GetError(hcan);
	HAL_CAN_ResetError(hcan);

	/* arbitration lost, retransmit */
	if(errorcode & HAL_CAN_ERROR_TX_ALST0){

		if(retransmision_counter < CAN_ARB_LOST_RETR){
		    SET_BIT(CAN_HANDLE->Instance->sTxMailBox[0].TIR, CAN_TI0R_TXRQ);
			retransmision_counter++;
		}
	}

}




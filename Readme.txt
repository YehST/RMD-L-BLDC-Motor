使用說明：

本函式庫基於Myactuator官方提供之通訊datasheet，適用韌體版本為2023/02之RMD-L系列無刷馬達，且通訊協定為RS-485總線。
本函式庫基於STM32 HAL庫開發，如有標準庫或其他硬體需求請自行更改。

使用本函式庫所需外設及腳位：
	- 用於RS-485通訊之UART *1
	- 用於RS-485通訊模式切換腳位 *1
	- 用於RS-485通訊Delay之Timer *1
	並配合MAX485晶片協助資料傳輸

基於以上設定，於韌體中main檔需先調用初始化函數，其中引數則為uart的handle、模式切換腳位以及timer 的handle，其中要將timer設定為每us計數一次才能確保通訊延遲足夠且正確，詳細資訊請參考所使用之STM32控制板或另行查詢設定資料：
	RS_Init_Handle(&huart2, GPIOA, GPIO_PIN_8, &htim8);

開始使用：
韌體中main檔需先建立馬達結構：
	RMD_Motordef Motor = {
    	.ID = 1
	};
如使用多顆馬達：
	RMD_Motordef Motor[2] = {
            {.ID = 1},
            {.ID = 2}
    	};
建立完畢後即可使用函式庫中所有的讀寫控制命令，並傳入相對應的馬達以及資料作為引數：
	RS_ABSangleControl(&Motor[0], angleCMD, 1800);
回傳值會直接存入結構，需要時再調用。

 (#) 注意：
	1. 使用多顆馬達時，各馬達間需要延時或透過RTOS做時序控制。
	2. 若傳值出現問題，檢查馬達以及版端設定之Baudrate是否相同，若相同，可以調高RS485_delay_time_us之數值。
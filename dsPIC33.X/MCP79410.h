
#define MCP79410_address            0x6F

#define RTCSEC     0x00
#define RTCMIN     0x01
#define RTCHOUR    0x02
#define RTCWKDAY   0x03
#define RTCDATE    0x04
#define RTCMTH     0x05
#define RTCYEAR    0x06
#define CONTROL    0x07
#define OSCTRIM    0x08
#define EEUNLOCK   0x09
#define ALM0SEC    0x0a
#define ALM0MIN    0x0b
#define ALM0HOUR   0x0c
#define ALM0WKDAY  0x0d
#define ALM0DATE   0x0e
#define ALM0MTH    0x0f

#define ST          1 << 7
#define WKDAY       2 << 0
#define PWRFAIL     0 << 4
#define VBATEN      0 << 3
#define CRSTRIM     0 << 2
#define EXTOSC      0 << 3
#define ALM0EN      1 << 4
#define ALM1EN      0 << 5
#define ALMMSK      2 << 4
#define ALM0IF      0 << 3

typedef union{
    struct{
        uint8_t Initinal        :1;
        uint8_t Interrupt       :1;
        uint8_t Issue           :1;
        uint8_t First           :1;
        uint8_t reserved        :4;
    };
    uint8_t flag;
}MCP79410_STATUS_u;
extern MCP79410_STATUS_u rtcc_status;

extern uint16_t data_temp_ten;
extern uint16_t data_temp_one;
extern uint16_t dummy;
extern uint16_t flag_MCP79410;

extern const uint8_t MCP79410_init[15][2];
extern const uint8_t MCP79410_oscrun[2][2];

void MCP79410_Init(void);
void MCP79410_Start(void);
void MCP79410_Reset_Hour(void);
void MCP79410_Print(void);
void MCP79410_process(void);
//void MCP79410_Read(void);



///***** Copyright © 2019 TeleDNA Comm. Pvt. Ltd. All rights reserved. *****
// *
// * Project     : messager-II
// * Module      : proc
// * Author      : sumala
// * Created     : 2019-12-23 12:37:42
// * Modified    : 2020-01-28 17:55:46
// * FileType    : C/C++ File
// * FileName    : SmppInfo.hh
// * Description : SmppGw, Smpp Partner and Smpp Account Infomation
// *
// ************************************************************************/
//#ifndef __SmppInfo__
//#define __SmppInfo__    1
//
//#include <vector>
//#include <algorithm>
//#include <fstream>
//
//
//#define  PATTERN "[%D %T.%e] [%L:%@] %v"
//#define MAX_SMPPGWs             20   /* Max Supported SmppGw Modules */
//#define MAX_PARTNERs            5000 /* Max Supported Smpp Partners */
//#define MAX_ACCOUNTs_PER_PRTNR  100  /* Max Supported Smpp Accounts per Partners */
//#define MAX_ACCOUNTs            (MAX_PARTNERs * MAX_ACCOUNTs_PER_PRTNR)
//#define MAX_SESSIONs            100  /* Max Supported Sessions per Smpp Account */
//#define MAX_DEST_MSCs           100  /* Max Supported Dest MSCs per Smpp Account  */
//#define MAX_DEST_IMSIs          100  /* Max Supported Dest IMSIs per Smpp Account */
//#define MAX_DEST_LRNs           100  /* Max Supported Dest LRNs per Smpp Account */
//#define MIN_IMSI_LEN            6    /* Min IMSI Len for IMSI Validation */
//#define MAX_IMSI_LEN            15   /* Max IMSI Len for IMSI Validation */
//#define MIN_MSC_LEN             4    /* Min MSC Len for MSC Validation */
//#define MAX_MSC_LEN             20   /* Max MSC Len for MSC Validation */
//#define MIN_LRN_LEN             4    /* Min LRN Len for LRN Validation */
//#define MAX_LRN_LEN             10   /* Max LRN Len for LRN Validation */
//#define MIN_MSISDN_LEN          4    /* Min MSISDN Len for MSISDN Validation */
//#define MAX_MSISDN_LEN          20   /* Max MSISDN Len for MSISDN Validation */
//
//#define MSC_LEN_INDEX           (MAX_MSC_LEN - MIN_MSC_LEN)
//#define LRN_LEN_INDEX           (MAX_LRN_LEN - MIN_LRN_LEN)
//#define IMSI_LEN_INDEX          (MAX_IMSI_LEN - MIN_IMSI_LEN)
//
//#define SMPP_PARTNER_NAME_LEN   20
//#define SMPP_ACCOUNT_NAME_LEN   20
//#define EMAIL_ID_LEN            256
//#define GLOBAL_TITLE_LEN        15
//
//using namespace std;
//
///* Smpp Partner Infomation */
//typedef struct SmppPrtnrInfo
//{
//public:
//    SmppPrtnrInfo();
//    ~SmppPrtnrInfo();
//
//    void fnReset();
//    bool fnFill(shared_ptr<spdlog::logger> lgr, const vector<string>& rkvPrtnrInfo);
//    //        bool fnUpdate(shared_ptr<spdlog::logger> lgr, const vector<string> &rkvPrtnrInfo);
//
//            /* uiId is commented as SmppPrtnrInfo array (Index + 1) will provide the Partner Id */
//            //uint32_t  uiId;                        /* Smpp Partner Id : Reference for SmppAccountInfo */
//    bool      bFilled;                       /* To indicate Smpp Partner Info is filled */
//    char      acName[SMPP_PARTNER_NAME_LEN]; /* Smpp Partner Name */
//    char      acEmailId[EMAIL_ID_LEN];       /* Smpp Partner Email Id for email notifications/alarms */
//    char      acMobile[GLOBAL_TITLE_LEN];    /* Smpp Partner Mobile Number for Sms notifications/alarms */
//    /* TODO: Should we have Smpp Acc Id Mapping here? Is so ow to mainatin? Action on Sumala */
//
//}SmppPrtnrInfo;
//
///* Smpp Account Infomation */
//typedef struct SmppAccInfo
//{
//public:
//    SmppAccInfo();
//    ~SmppAccInfo();
//
//    void fnReset();
//    void fnFill(shared_ptr<spdlog::logger> lgr, const vector<string>& kvAccInfo);
//    void fnFillLists(shared_ptr<spdlog::logger> lgr, string sData, char* acLst, int iMaxLen, int* iStartIndex,
//        int* iEndIndex, int* iLstSize);
//    void fnUpdate(shared_ptr<spdlog::logger> lgr, const vector<string>& kvAccInfo);
//    void fnUpdateAccStatus(shared_ptr<spdlog::logger> lgr, const ACCOUNT_STATUS& keAccStatus);
//    void fnResetSessions(shared_ptr<spdlog::logger> lgr);
//    void fnUpdateSessionStatus(shared_ptr<spdlog::logger> lgr, const vector<string>& kvSessionInfo);
//    //void fnFillImsiLst(shared_ptr<spdlog::logger> lgr);
//    //void fnFillLrnLst(shared_ptr<spdlog::logger> lgr);
//    //void fnFillMscLst(shared_ptr<spdlog::logger> lgr);
//    bool fnIsImsiAllowed(shared_ptr<spdlog::logger> lgr, string sRcvdImsi);
//    bool fnIsMscAllowed(shared_ptr<spdlog::logger> lgr, string sRcvdMsc);
//    bool fnIsLrnAllowed(shared_ptr<spdlog::logger> lgr, string sLrn);
//    bool fnIsSessionActive(shared_ptr<spdlog::logger> lgr, const uint8_t& rkuiSessionId,
//        ACCOUNT_STATUS& reAccStatus, const BIND_TYPE eBindType);
//    int8_t fnGetActiveSmppGwId(shared_ptr<spdlog::logger> lgr, const BIND_TYPE eBindType,
//        ACCOUNT_STATUS& reAccStatus);
//
//    /* uiId is commented as SmppAccInfo array (Index + 1) will provide the Smpp Account Id */
//    //uint32_t              uiId;                                  /* Smpp Account Id */
//    bool                  bFilled;                                 /* To indicate Smpp Account Info is filled */
//    char                  acName[SMPP_ACCOUNT_NAME_LEN];           /* Smpp Account Name */
//    ACCOUNT_STATUS        eAccStatus;                              /* Smpp Account Status */
//    uint32_t              uiPrtnrId;                               /* Smpp Partner Id */
//    uint8_t               auiTxSession[MAX_SESSIONs];              /* SmppGwId for each Active Tx session */
//    uint8_t               auiRxSession[MAX_SESSIONs];              /* SmppGwId for each Active Rx session */
//    uint8_t               uiTxIndex;                               /* Tx Session Index for Load Balancing */
//    uint8_t               uiRxIndex;                               /* Rx Session Index for Load Balancing */
//
//    /* TODO: Which columns need to refer for uiMobDestRetPolicy, uiAppDestRetPolicy and for LRN details ? Check
//     * with UI Team */
//    uint16_t              uiMobDestRetPolicy;                      /* Mobile Destined Retry Policy Id */
//    uint16_t              uiAppDestRetPolicy;                      /* App Destined Retry Policy Id */
//    char                  acVirtualGT[GLOBAL_TITLE_LEN];           /* Virtual SMSC GT Address*/
//    TRAFFIC_CNTRL_ACTION  eImsiCntrlAction;                        /* IMSI Control Action */
//    TRAFFIC_CNTRL_ACTION  eLrnCntrlAction;                         /* LRN Control Action */
//    TRAFFIC_CNTRL_ACTION  eMscCntrlAction;                         /* MSC Control Action */
//    char                  acMscLst[MAX_DEST_MSCs][MAX_MSC_LEN];    /* MSC Validation List */
//    char                  acImsiLst[MAX_DEST_IMSIs][MAX_IMSI_LEN]; /* IMSI Validation List */
//    char                  acLrnLst[MAX_DEST_LRNs][MAX_LRN_LEN];    /* LRN Validation List */
//    int8_t                iImsiStartIndex[IMSI_LEN_INDEX];         /* Configured IMSI Length Start Index */
//    int8_t                iImsiEndIndex[IMSI_LEN_INDEX];           /* Configured IMSI Length End Index */
//    int8_t                iLrnStartIndex[LRN_LEN_INDEX];           /* Configured LRN Length Start Index */
//    int8_t                iLrnEndIndex[LRN_LEN_INDEX];             /* Configured LRN Length End Index */
//    int8_t                iMscStartIndex[MSC_LEN_INDEX];           /* Configured MSC Length Start Index */
//    int8_t                iMscEndIndex[MSC_LEN_INDEX];             /* Configured MSC Length End Index */
//    uint8_t               uiLrnLstSize;                            /* Number of LRNs configured */
//    uint8_t               uiMscLstSize;                            /* Number of MSCs configured */
//    uint8_t               uiImsiLstSize;                           /* Number of IMSIs configured */
//
//} SmppAccInfo;
//
//struct SmppInfo
//{
//public:
//    SmppInfo();
//    ~SmppInfo();
//
//    void fnReset();
//    void fnPrintSmppInfo();
//    void fnPrintSmppMacros();
//    void fnPrintSmppEnums();
//    void fnPrintSmppAccInfo();
//    void fnPrintSmppPrtnrInfo();
//    void fnPrintSmppPrtnrInfo(const uint16_t& rkuiPrtnrId);
//    void fnPrintSmppAccInfo(const uint32_t& rkuiAccId);
//    void fnPrintSessionInfo(const uint32_t& rkuiAccId);
//    void fnFillSmppPrtnrInfo(const vector<vector<string>>& rkvvsPrtnrInfo);
//    void fnFillSmppAccInfo(const vector<vector<string>>& rkvvsAccInfo);
//
//    //        ModuleInfo       stSmppGw[MAX_SMPPGWs];         /* SmppGw Module Infomation */
//    SmppPrtnrInfo  stSmppPartners[MAX_PARTNERs];  /* Provisioned Smpp Partners Infomation */
//    SmppAccInfo  stSmppAccounts[MAX_ACCOUNTs];  /* Provisioned Smpp Accounts Infomation */
//    uint8_t          uiSmppModuleSize;              /* Number of Smpp Modules configured */
//    uint16_t         uiSmppPrtnrSize;               /* Number of Smpp Partners configured */
//    uint16_t         uiSmppAccSize;                 /* Number of Smpp Accounts configured */
//};
//
//
//#endif //__SmppInfo__

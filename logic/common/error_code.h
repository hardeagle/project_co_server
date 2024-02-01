#ifndef __LOGIC_COMMON_ERROR_CODE_H__
#define __LOGIC_COMMON_ERROR_CODE_H__

enum EC_LOGIN {
    SUCCESS                 = 0,
    NO_ROLE                 = 101,
    GENERATE_ID_FAIL        = 102,
    GAME_ID_ILLEGAL         = 103,
    HTTP_TT_STATUS_ERROR    = 104,
    HTTP_TT_RSP_FAIL        = 105,
    HTTP_TT_GET_FAIL        = 106,
    HTTP_TT_RSP_PARSE_FAIL  = 107,
    PARSE_FROM_STRING_FAIL  = 108,
};

#endif
#pragma once

#include "def.h"

#ifndef ERR_PORT
#error Please define ERR_PORT(__FILE__ __LINE__)
#endif
#ifndef ERR_PORT_BIT
#error Please define ERR_PORT_NUM(__FILE__ __LINE__)
#endif

/**
 * エラーが発生した際に特定のポートをHIGHにします。
 * エラー表示に使用するポートとして、
 * あらかじめ、ERR_PORT ERR_PORT_BITをdefineしておく必要があります。
 */
void Exception(){
    sbi(ERR_PORT,ERR_PORT_BIT);
}

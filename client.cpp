#include "client.h"
#include "ace/Log_Msg.h"
#include "ace/SOCK_Connector.h"
#include "ace/SString.h"

Client::Client(void) : initialized_(0), error_(0) { }


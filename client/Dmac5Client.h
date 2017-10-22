#pragma once

namespace dmac5
{

int initialize_dmac5_proxy_connection(SOCKET& ConnectSocket);

int deinitialize_dmac5_proxy_connection(SOCKET ConnectSocket);

}
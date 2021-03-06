#ifndef NET_H
#define NET_H

#include <sys/fd_set.h>

/** @defgroup Network

	Network functions
	@{
*/

#define OSL_NET_ERROR_NET		-1
#define OSL_NET_ERROR_INET		-2
#define OSL_NET_ERROR_RESOLVER	-3
#define OSL_NET_ERROR_APCTL		-4
#define OSL_NET_ERROR_SSL		-5
#define OSL_NET_ERROR_HTTP		-6
#define OSL_NET_ERROR_HTTPS		-7
#define OSL_NET_ERROR_CERT		-8
#define OSL_NET_ERROR_COOKIE	-9

#define OSL_ERR_APCTL_GETINFO   -10
#define OSL_ERR_APCTL_CONNECT	-11
#define OSL_ERR_APCTL_TIMEOUT	-12
#define OSL_ERR_APCTL_GETSTATE	-13

#define OSL_ERR_RESOLVER_CREATE		-14
#define OSL_ERR_RESOLVER_RESOLVING	-15

#define OSL_ERR_WLAN_OFF	        -16
#define OSL_USER_ABORTED            -17

#define OSL_ERR_HTTP_INIT           -18
#define OSL_ERR_HTTP_TEMPLATE       -19
#define OSL_ERR_HTTP_TIMEOUT        -20
#define OSL_ERR_HTTP_CONNECT        -21
#define OSL_ERR_HTTP_REQUEST        -22
#define OSL_ERR_HTTP_GENERIC        -23


#define OSL_MAX_NET_CONFIGS		20

/**Structure for network configuration*/
struct oslNetConfig{
    char name[128];
    char IP[128];
};

#define OSL_RESOLVER_RETRY		4

/**Checks if the WLAN switch is powered on (returns 1, else 0).
*/
int oslIsWlanPowerOn();

/**Checks if the WLAN is connected to an Access Point (returns 1, else 0).
*/
int oslIsWlanConnected();

/**Gets the network configs.
\code
struct oslNetConfig configs[OSL_MAX_NET_CONFIGS];
int numconfigs = oslGetNetConfigs(configs);
\endcode
*/
int oslGetNetConfigs(struct oslNetConfig *result);

/**Inits network
*/
int oslNetInit();

/**Terminates network
*/
int oslNetTerm();

/**Gets the current IP address (when connected to an Access Point)
*/
int oslGetIPaddress(char *IPaddress);

/**Tries to connect to an Access Point using the given configuration
	\param int config
        Index of the configuration to use
	\param int timeout
        Timeout (in seconds) for the connection
	\param int (*apctlCallback)(int state)
        Pointer to a callback function. It will be called with the current state
*/
int oslConnectToAP(int config, int timeout, int (*apctlCallback)(int state));

/**Disconnectes from the Access Point
*/
int oslDisconnectFromAP();

/**Gets the current Access Point state
*/
int oslGetAPState();

/**Resolves an address to its IP address
*/
int oslResolveAddress(char *address, char *resolvedIP);

/**Get a file from the web and save it
*/
int oslNetGetFile(const char *url, const char *filepath);

/**Posts a web form
*/
int oslNetPostForm(const char *url, char *data, char *response, unsigned int responsesize);


int oslNetSocketCreate(void);
int oslNetSocketAccept(int socket);
int oslNetSocketBind(int socket, unsigned short port);
int oslNetSocketListen(int socket, unsigned int maxconn);
int oslNetSocketConnect(int socket, char *ip, unsigned short port);
int oslNetSocketSend(int socket, const void *data, int length);
int oslNetSocketReceive(int socket, void *data, int length);
void oslNetSocketClose(int socket);
void oslNetSocketSetClear(fd_set *set);
void oslNetSocketSetAdd(int socket, fd_set *set);
void oslNetSocketSetRemove(int socket, fd_set *set);
int oslNetSocketSetIsMember(int socket, fd_set *set);
int oslNetSocketSetSelect(unsigned int maxsockets, fd_set *set);

/** @} */ // end of net
#endif

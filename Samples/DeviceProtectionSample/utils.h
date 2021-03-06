/*   
Copyright 2006 - 2011 Intel Corporation

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef __MeshUtils__
#define __MeshUtils__

#include "ILibParsers.h"
#include "ILibWebClient.h"
#include "ILibWebServer.h"
#include <openssl/pem.h>
//#include <openssl/err.h>
#include <openssl/pkcs7.h>
#include <openssl/pkcs12.h>
#include <openssl/conf.h>
#include <openssl/x509v3.h>
#include <openssl/engine.h>

#if defined(WIN32)
#define snprintf _snprintf
#endif


// Debugging features
#if defined(_DEBUG)
    char spareDebugMemory[4000];
    int  spareDebugLen;

	// Display & log
    //#define MSG(x) printf("%s",x);//mdb_addevent(x, (int)strlen(x));
	//#define MSG2(t,x) spareDebugLen = snprintf(spareDebugMemory,4000,t,x);printf("%s",spareDebugMemory);//mdb_addevent(spareDebugMemory, spareDebugLen);
    //#define MSG3(t,x,y) spareDebugLen = snprintf(spareDebugMemory,4000,t,x,y);printf("%s",spareDebugMemory);//mdb_addevent(spareDebugMemory, spareDebugLen);
    //#define MSG4(t,x,y,z) spareDebugLen = snprintf(spareDebugMemory,4000,t,x,y,z);printf("%s",spareDebugMemory);//mdb_addevent(spareDebugMemory, spareDebugLen);
    //#define MSG5(t,x,y,z,a) spareDebugLen = snprintf(spareDebugMemory,4000,t,x,y,z,a);printf("%s",spareDebugMemory);//mdb_addevent(spareDebugMemory, spareDebugLen);

	// Display only
	#define MSG(x) printf(x);fflush(NULL);
    #define MSG2(t,x) printf(t,x);fflush(NULL);
    #define MSG3(t,x,y) printf(t,x,y);fflush(NULL);
    #define MSG4(t,x,y,z) printf(t,x,y,z);fflush(NULL);
    #define MSG5(t,x,y,z,a) printf(t,x,y,z,a);fflush(NULL);

	#define DEBUGSTATEMENT(x) x
	#if defined(_POSIX)
		//#include <mcheck.h>
	#endif
#else
    #define MSG(x)
    #define MSG2(t,x)
    #define MSG3(t,x,y)
    #define MSG4(t,x,y,z)
	#define MSG5(t,x,y,z,a)
    #define DEBUGSTATEMENT(x)
#endif

#define UTIL_HASHSIZE     32
#define MAX_TOKEN_SIZE    1024
#define SMALL_TOKEN_SIZE  256
#define NONCE_SIZE        32
#define HALF_NONCE_SIZE   16

enum CERTIFICATE_TYPES
{
	CERTIFICATE_ROOT = 1,
	CERTIFICATE_TLS_SERVER = 2,
	CERTIFICATE_TLS_CLIENT = 3,
};

// Certificate structure
struct util_cert
{
	X509 *x509;
	EVP_PKEY *pkey;
};

// General methods
void  util_openssl_init();
void  util_openssl_uninit();
void  util_free(char* ptr);
void  util_tohex(char* data, int len, char* out);
int   util_hexToint(char *hexString, int hexStringLength);
void  util_startChronometer();
long  util_readChronometer();
unsigned long  util_gettime();

// File and data methods
//int    util_compress(char* inbuf, unsigned int inbuflen, char** outbuf, unsigned int headersize);
//int    util_decompress(char* inbuf, unsigned int inbuflen, char** outbuf, unsigned int headersize);
size_t util_writefile(char* filename, char* data, int datalen);
size_t util_readfile(char* filename, char** data);
int    util_deletefile(char* filename);
#ifdef _POSIX
int util_readfile2(char* filename, char** data);
#endif

// Certificate & crypto methods
void  util_freecert(struct util_cert* cert);
int   util_to_p12(struct util_cert cert, char *password, char** data);
int   util_from_p12(char* data, int datalen, char* password, struct util_cert* cert);
int   util_to_cer(struct util_cert cert, char** data);
int   util_from_cer(char* data, int datalen, struct util_cert* cert);
int   util_mkCert(struct util_cert *rootcert, struct util_cert* cert, int bits, int days, char* name, enum CERTIFICATE_TYPES certtype);
void  util_printcert(struct util_cert cert);
void  util_printcert_pk(struct util_cert cert);
void  util_sendcert(struct ILibWebServer_Session *sender, struct util_cert cert);
void  util_md5(char* data, int datalen, char* result);
void  util_md5hex(char* data, int datalen, char *out);
void  util_sha256(char* data, int datalen, char* result);
int   util_sha256file(char* filename, char* result);
int   util_keyhash(struct util_cert cert, char* result);
int   util_keyhash2(X509* cert, char* result);
int   util_sign(struct util_cert cert, char* data, int datalen, char** signature);
int   util_verify(char* signature, int signlen, struct util_cert* cert, char** data);
int   util_encrypt(struct util_cert cert, char* data, int datalen, char** encdata);
int   util_encrypt2(STACK_OF(X509) *certs, char* data, int datalen, char** encdata);
int   util_decrypt(char* encdata, int encdatalen, struct util_cert cert, char** data);
void  util_random(int length, char* result);
void  util_randomtext(int length, char* result);

// Symetric Crypto methods
void  util_nodesessionkey(char* nodeid, char* key);
int   util_cipher(char* key, unsigned int iv, char* nodeid, char* data, int datalen, char** result, int sendresponsekey);
int   util_decipher(char* data, int datalen, char** result, char* nodeid);
void  util_genusagekey(char* inkey, char* outkey, char usage);

// Network security methods
int	  util_antiflood(int rate, int interval);
int   util_ExtractWwwAuthenticate(char *wwwAuthenticate, void *request);
void  util_GenerateAuthorizationHeader(void *request, char *requestType, char *uri, char *authorization, char* cnonce);
unsigned int util_getiv();
int   util_checkiv(unsigned int iv);

#endif


#ifndef __CORE_WS_SESSION_H__
#define	__CORE_WS_SESSION_H__

#include <assert.h>
#include <stdint.h> /* uint8_t */
#include <stdio.h> /* sscanf */
#include <ctype.h> /* isdigit */
#include <stddef.h> /* int */

// std c++
#include <vector> 
#include <string> 

#include "session.h"

using namespace std;

namespace Eayew {

enum WebSocketFrameType {
	ERROR_FRAME=0xFF00,
	INCOMPLETE_FRAME=0xFE00,

	OPENING_FRAME=0x3300,
	CLOSING_FRAME=0x3400,

	INCOMPLETE_TEXT_FRAME=0x01,
	INCOMPLETE_BINARY_FRAME=0x02,

	TEXT_FRAME=0x81,
	BINARY_FRAME=0x82,

	PING_FRAME=0x19,
	PONG_FRAME=0x1A
};

class WsSession : public Session {
public:

	string resource;
	string host;
	string origin;
	string protocol;
	string key;

	WsSession(uint32_t fd);

    virtual bool start(bool accept = true) override;
	virtual void run();

private:
public:
    void sync_read();
    void sync_write();

	bool sync_connect(const std::string& url);

	/**
	 * @param input_frame .in. pointer to input frame
	 * @param input_len .in. length of input frame
	 * @return [WS_INCOMPLETE_FRAME, WS_ERROR_FRAME, WS_OPENING_FRAME]
	 */
	WebSocketFrameType parseHandshake(unsigned char* input_frame, int input_len);
	string answerHandshake();

	int makeFrame(WebSocketFrameType frame_type, unsigned char* msg, int msg_len, unsigned char* buffer);
	WebSocketFrameType getFrame(unsigned char* in_buffer, int in_length, unsigned char* out_buffer, int out_size, int* out_length, int* out_pos);

	string trim(string str);
	vector<string> explode(string theString, string theDelimiter, bool theIncludeEmptyStrings = false );

private:

};

}

#endif
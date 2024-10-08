
#include "ws_session.h"

//#include "md5/md5.h"
#include "core/const.hpp"
#include "core/util/base64.h"
#include "core/util/sha1.h"

#include <iostream>
#include <string>
#include <vector>

#include <co/all.h>

using namespace std;

namespace Eayew {

WsSession::WsSession(uint32_t fd)
    : Session(fd) {

}

bool WsSession::start(bool accept) {
    if (accept) {
        static constexpr size_t HANDSHAKE_STREAMBUF_SIZE = 8192;
        char buffs[HANDSHAKE_STREAMBUF_SIZE] = {};
        auto rlen = read(m_fd, &buffs[0], HANDSHAKE_STREAMBUF_SIZE);
        LOG << "WsSession start rlen " << rlen;
		if (rlen == 0) {
			return false;
		}
        auto wsft = parseHandshake((unsigned char*)(&buffs[0]), HANDSHAKE_STREAMBUF_SIZE);
        LOG << "WsSession start wstf " << wsft;
		if (wsft != OPENING_FRAME) {
			return false;
		}
        auto resp = answerHandshake();
        LOG << "WsSession start resp " << resp;
        write(m_fd, resp.data(), resp.size());
    }
	return true;
}

void WsSession::run() {
    m_rSched->go([this, self = shared_from_this()] {
        sync_read();
    });

    m_wSched->go([this, self = shared_from_this()] {
        sync_write();
    });
}

void WsSession::sync_read() {
	uint32_t icount = 0;
	uint32_t count = 0;

	static const uint32_t MAX_SIZE = 64 * 1024;
	char buffs[MAX_SIZE];
	uint32_t index = 0;
	uint32_t rlen = 0;
	for (;;) {
		if (index < rlen) {
			WLOG << "memmove index " << index << " rlen " << rlen;
			memcpy(&buffs[0], &buffs[index], rlen - index);
			index = rlen - index;
		} else {
			index = 0;
		}
		rlen = read(m_fd, &buffs[index], MAX_SIZE - index);
		if (0 == rlen) {
			WLOG << "close";
			if (m_onCloseCB != nullptr) {
                m_onCloseCB(id());
            }
			close(m_fd);
			break;
		} else if (-1 == rlen) {
			if (errno == EINTR || errno==EAGAIN) {
				WLOG << "errno " << errno;
				continue;
			} else {
				ELOG << "errno " << errno;
				if (m_onCloseCB != nullptr) {
					m_onCloseCB(id());
				}
				close(m_fd);
				break;
			}
		}

		icount++;
		rlen += index;
		index = 0;
		while (index < rlen) {
			char datas[MAX_SIZE] = {};
			int len = 0;
			int pos = 0;
			auto wsft = getFrame((unsigned char*)&buffs[index], rlen - index, (unsigned char*)&datas[0], rlen - index, &len, &pos);
			if ((wsft != 129 && wsft != 130) || index + len + pos - 1 > rlen) {
				WLOG << "break, wsft " << wsft << " len " << len << " pos " << pos << " rlen " << rlen << " index " << index << " count " << count++ << " icount " << icount  << " datas " << datas;
				break;
			}

			auto size = len + pos;
            auto msg = co::make_shared<Message>(size - Message::HEAD_LEN);
            memcpy(msg->data(), &datas[0], size);
			//msg.write(&datas[0], size);
            if (m_onMessageCB != nullptr) {
                LOG << "msg " << msg->strInfo();
                m_onMessageCB(msg);
            }

			index = index + len + pos;
			// WLOG << "loop, wsft " << wsft << " len " << len << " pos " << pos << " rlen " << rlen << " index " << index << " count " << count++ << " icount " << icount << " datas " << datas;
		}
		// WLOG << "once read, len " << rlen << " index " << index;
	}
}

void WsSession::sync_write() {
	static const uint32_t MAX_SIZE = 64 * 1024;

    for (;;) {
        // if (m_wMsgs.size() == 0) {
        //     WLOG << "m_wMsgs empty";
        // }

        Message::ptr msg;
        m_wMsgs >> msg;
        LOG << "sync_write " << msg->strInfo();
		if (msg->msgId() == CloseMsgId::ECMI_WebsocketSession) {
			WLOG << "wssession exit";
			break;
		}

		char datas[MAX_SIZE] = {};
		auto len = this->makeFrame(Eayew::WebSocketFrameType::BINARY_FRAME, (unsigned char*)msg->data(), msg->length(),  (unsigned char*)&datas[0]);
        write(m_fd, datas, len);
    }
}

WebSocketFrameType WsSession::parseHandshake(unsigned char* input_frame, int input_len) {
	// 1. copy char*/len into string
	// 2. try to parse headers until \r\n occurs
	string headers((char*)input_frame, input_len); 
	int header_end = headers.find("\r\n\r\n");

	if(header_end == string::npos) { // end-of-headers not found - do not parse
		return INCOMPLETE_FRAME;
	}

	headers.resize(header_end); // trim off any data we don't need after the headers
	vector<string> headers_rows = explode(headers, string("\r\n"));
	for(int i=0; i<headers_rows.size(); i++) {
		string& header = headers_rows[i];
		if(header.find("GET") == 0) {
			vector<string> get_tokens = explode(header, string(" "));
			if(get_tokens.size() >= 2) {
				this->resource = get_tokens[1];
			}
		}
		else {
			int pos = header.find(":");
			if(pos != string::npos) {
				string header_key(header, 0, pos);
				string header_value(header, pos+1);
				header_value = trim(header_value);
				if(header_key == "Host") this->host = header_value;
				else if(header_key == "Origin") this->origin = header_value;
				else if(header_key == "Sec-WebSocket-Key") this->key = header_value;
				else if(header_key == "Sec-WebSocket-Protocol") this->protocol = header_value;
			}
		}
	}

	//this->key = "dGhlIHNhbXBsZSBub25jZQ==";
	//printf("PARSED_KEY:%s \n", this->key.data());

	//return FrameType::OPENING_FRAME;
	LOG << "HANDSHAKE-PARSED";
	return OPENING_FRAME;
}

string WsSession::trim(string str) {
	//printf("TRIM\n");
	const char* whitespace = " \t\r\n";
	string::size_type pos = str.find_last_not_of(whitespace);
	if(pos != string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(whitespace);
		if(pos != string::npos) str.erase(0, pos);
	}
	else {
		return string();
	}
	return str;
}

vector<string> WsSession::explode(string theString, string theDelimiter, bool theIncludeEmptyStrings) {
	//printf("EXPLODE\n");
	//UASSERT( theDelimiter.size(), >, 0 );
	
	vector<string> theStringVector;
	int  start = 0, end = 0, length = 0;

	while ( end != string::npos )
	{
		end = theString.find( theDelimiter, start );

		// If at end, use length=maxLength.  Else use length=end-start.
		length = (end == string::npos) ? string::npos : end - start;

		if (theIncludeEmptyStrings
			|| (   ( length > 0 ) /* At end, end == length == string::npos */
            && ( start  < theString.size() ) ) )
		theStringVector.push_back( theString.substr( start, length ) );

		// If at end, use start=maxSize.  Else use start=end+delimiter.
		start = (   ( end > (string::npos - theDelimiter.size()) )
              ?  string::npos  :  end + theDelimiter.size()     );
	}
	return theStringVector;
}

string WsSession::answerHandshake() {
    unsigned char digest[20]; // 160 bit sha1 digest

	string answer;
	answer += "HTTP/1.1 101 Switching Protocols\r\n";
	answer += "Upgrade: WebSocket\r\n";
	answer += "Connection: Upgrade\r\n";
	if(this->key.length() > 0) {
		string accept_key;
		accept_key += this->key;
		accept_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"; //RFC6544_MAGIC_KEY

		//printf("INTERMEDIATE_KEY:(%s)\n", accept_key.data());

		SHA1 sha;
		sha.Input(accept_key.data(), accept_key.size());
		sha.Result((unsigned*)digest);
		
		//printf("DIGEST:"); for(int i=0; i<20; i++) printf("%02x ",digest[i]); printf("\n");

		//little endian to big endian
		for(int i=0; i<20; i+=4) {
			unsigned char c;

			c = digest[i];
			digest[i] = digest[i+3];
			digest[i+3] = c;

			c = digest[i+1];
			digest[i+1] = digest[i+2];
			digest[i+2] = c;
		}

		//printf("DIGEST:"); for(int i=0; i<20; i++) printf("%02x ",digest[i]); printf("\n");

		accept_key = base64_encode((const unsigned char *)digest, 20); //160bit = 20 bytes/chars

		answer += "Sec-WebSocket-Accept: "+(accept_key)+"\r\n";
	}
	if(this->protocol.length() > 0) {
		answer += "Sec-WebSocket-Protocol: "+(this->protocol)+"\r\n";
	}
	answer += "\r\n";
	return answer;

	//return WS_OPENING_FRAME;
}

int WsSession::makeFrame(WebSocketFrameType frame_type, unsigned char* msg, int msg_length, unsigned char* buffer) {
	int pos = 0;
	int size = msg_length; 
	buffer[pos++] = (unsigned char)frame_type; // text frame

	if(size <= 125) {
		buffer[pos++] = size;
	}
	else if(size <= 65535) {
		buffer[pos++] = 126; //16 bit length follows
		
		buffer[pos++] = (size >> 8) & 0xFF; // leftmost first
		buffer[pos++] = size & 0xFF;
	}
	else { // >2^16-1 (65535)
		buffer[pos++] = 127; //64 bit length follows
		
		// write 8 bytes length (significant first)
		
		// since msg_length is int it can be no longer than 4 bytes = 2^32-1
		// padd zeroes for the first 4 bytes
		for(int i=3; i>=0; i--) {
			buffer[pos++] = 0;
		}
		// write the actual 32bit msg_length in the next 4 bytes
		for(int i=3; i>=0; i--) {
			buffer[pos++] = ((size >> 8*i) & 0xFF);
		}
	}
	memcpy((void*)(buffer+pos), msg, size);
	return (size+pos);
}

WebSocketFrameType WsSession::getFrame(unsigned char* in_buffer, int in_length, unsigned char* out_buffer, int out_size, int* out_length, int* out_pos) {
	//printf("getTextFrame()\n");
	if(in_length < 2) return INCOMPLETE_FRAME;

	unsigned char msg_opcode = in_buffer[0] & 0x0F;
	unsigned char msg_fin = (in_buffer[0] >> 7) & 0x01;
	unsigned char msg_masked = (in_buffer[1] >> 7) & 0x01;

	// *** message decoding 

	int payload_length = 0;
	int pos = 2;
	int length_field = in_buffer[1] & (~0x80);
	unsigned int mask = 0;

	//printf("IN:"); for(int i=0; i<20; i++) printf("%02x ",buffer[i]); printf("\n");

	if(length_field <= 125) {
		payload_length = length_field;
	}
	else if(length_field == 126) { //msglen is 16bit!
		//payload_length = in_buffer[2] + (in_buffer[3]<<8);
		payload_length = (
			(in_buffer[2] << 8) | 
			(in_buffer[3])
		);
		pos += 2;
	}
	else if(length_field == 127) { //msglen is 64bit!
		payload_length = (
			(in_buffer[2] << 56) | 
			(in_buffer[3] << 48) | 
			(in_buffer[4] << 40) | 
			(in_buffer[5] << 32) | 
			(in_buffer[6] << 24) | 
			(in_buffer[7] << 16) | 
			(in_buffer[8] << 8) | 
			(in_buffer[9])
		); 
		pos += 8;
	}
		
	//printf("PAYLOAD_LEN: %08x\n", payload_length);
	if(in_length < payload_length+pos) {
		return INCOMPLETE_FRAME;
	}

	if(msg_masked) {
		mask = *((unsigned int*)(in_buffer+pos));
		//printf("MASK: %08x\n", mask);
		pos += 4;

		// unmask data:
		unsigned char* c = in_buffer+pos;
		for(int i=0; i<payload_length; i++) {
			c[i] = c[i] ^ ((unsigned char*)(&mask))[i%4];
		}
	}
	
	if(payload_length > out_size) {
		//TODO: if output buffer is too small -- ERROR or resize(free and allocate bigger one) the buffer ?
	}

	memcpy((void*)out_buffer, (void*)(in_buffer+pos), payload_length);
	//out_buffer[payload_length] = 0;
	*out_length = payload_length;
    *out_pos = pos;

	//printf("TEXT: %s\n", out_buffer);

	if(msg_opcode == 0x0) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME; // continuation frame ?
	if(msg_opcode == 0x1) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME;
	if(msg_opcode == 0x2) return (msg_fin)?BINARY_FRAME:INCOMPLETE_BINARY_FRAME;
	if(msg_opcode == 0x9) return PING_FRAME;
	if(msg_opcode == 0xA) return PONG_FRAME;

	return ERROR_FRAME;
}

// test
constexpr int INVALID_SOCKET = -1;
constexpr int SOCKET_ERROR = -1;

bool parse_url(const std::string& url, std::string* host, std::string* path,
               int* port) {
  // TODO: Use regex or external lib
  if (url.find("ws://") == std::string::npos) {
    return false;
  }

  *host = url.substr(url.find("ws://") + 5);

  *path = host->substr(host->find("/") + 1);
  if (*path == *host) *path = "";
  while (path->size() && path->at(0) == '/') {
    *path = path->substr(1);
  }

  *host = host->substr(0, host->find("/"));

  auto colon_pos = host->find(":");
  if (colon_pos == std::string::npos) {
    *port = 80;
  } else {
    auto port_str = host->substr(colon_pos + 1);

    *port = std::stoi(port_str, nullptr);
    *host = host->substr(0, host->find(":"));
  }

  return true;
}

std::string read_line(int sockfd) {
  std::string line;
  char buf[2] = {0, 0};

  while (buf[0] != '\r' && buf[1] != '\n') {
    buf[0] = buf[1];
    if (!::recv(sockfd, buf + 1, 1, 0))
      return "";
    else
      line += buf[1];
  }
  return line;
}

bool check_header(int sockfd) {
  std::string line;
  auto rd = read_line(sockfd);
  LOG << "rd " << rd;
  if (rd.find("HTTP/1.1 101") != 0) {
	LOG << "read line fail";
    return false;
  }
  while ((line = read_line(sockfd).c_str()) != "\r\n") {
	LOG << "read line 2";
    if (line.empty()) return false;
  }
  return true;
}

int establish_connection(const int sockfd, const std::string& host,
                         const std::string& path, const int port) {
//   char line[256];

//   snprintf(line, 256, "GET /%s HTTP/1.1\r\n", path.c_str());
//   ::send(sockfd, line, strlen(line), 0);
//   snprintf(line, 256, "Host: %s:%d\r\n", host.c_str(), port);
//   ::send(sockfd, line, strlen(line), 0);
//   snprintf(line, 256, "Upgrade: websocket\r\n");
//   ::send(sockfd, line, strlen(line), 0);
//   snprintf(line, 256, "Connection: Upgrade\r\n");
//   ::send(sockfd, line, strlen(line), 0);
//   // TODO origin
//   // if (!origin.empty()) {
//   //   snprintf(line, 256, "Origin: %s\r\n", origin.c_str());
//   //   ::send(sockfd, line, strlen(line), 0);
//   // }
//   snprintf(line, 256, "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n");
//   ::send(sockfd, line, strlen(line), 0);
//   snprintf(line, 256, "Sec-WebSocket-Version: 13\r\n");
//   ::send(sockfd, line, strlen(line), 0);
//   snprintf(line, 256, "\r\n");
//   ::send(sockfd, line, strlen(line), 0);

	std::string req = "";
	// req += "GET " + m_path + " HTTP/1.1\r\n";
	// req += "Host: " + m_host + ":443\r\n";
	req += "GET HTTP/1.1\r\n";
	req += "Host: :443\r\n";
	req += "Upgrade: websocket\r\n";
	req += "Connection: Upgrade\r\n";
	req += "Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n";
	req += "Sec-WebSocket-Version: 13\r\n";
	req += "\r\n";

	::send(sockfd, req.c_str(), req.size(), 0);

  	return check_header(sockfd);
}

int connect_from_hostname(const std::string& hostname, int port) {
  struct addrinfo hints;
  struct addrinfo* result;
  int ret;

  int sockfd = INVALID_SOCKET;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((ret = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints,
                         &result)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    return -2;
  }
  for (auto info = result; result != NULL; result = result->ai_next) {
    sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

    if (sockfd == INVALID_SOCKET) continue;

    if (::connect(sockfd, info->ai_addr, info->ai_addrlen) == 0) {
      break;
    }
    ::close(sockfd);
    sockfd = INVALID_SOCKET;
  }
  freeaddrinfo(result);
  return sockfd;
}

bool WsSession::sync_connect(const std::string& url) {
	std::string host, path;
    int port;
    parse_url(url, &host, &path, &port);
	LOG << "url " << url;
	LOG << "hose " << host;
	LOG << "path " << path;
	LOG << "port " << port;

    if ((m_fd = connect_from_hostname(host, port)) < 0) {
		LOG << "connext from hostname fail";
      return false;
    }
    if (!establish_connection(m_fd, host, path, port)) {
		LOG << "establish connection fail";
      return false;
    }
	return true;
}

}
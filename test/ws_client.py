# import websocket

# def on_message(ws, message):
#     print("on_message")
#     print(ws)
#     print(message)


# def on_error(ws, error):
#     print("on_error")
#     print(ws)
#     print(error)


# def on_close(ws):
#     print("on_close")
#     print(ws)
#     print("### closed ###")


# websocket.enableTrace(True)
# ws = websocket.WebSocketApp("ws://127.0.0.1:9999",
#                             on_message=on_message,
#                             on_error=on_error,
#                             on_close=on_close)

# ws.run_forever()

# ws.send("aaaaaaaaaaaaaaaa")


import websocket
import random

# 生成随机字符串
def generate_random_str(randomlength=16):
    """
    生成一个指定长度的随机字符串
    """
    random_str = ''
    base_str = 'ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyz0123456789'
    length = len(base_str) - 1
    for i in range(randomlength):
        random_str += base_str[random.randint(0, length)]
    return random_str


class WebsocketClient(object):
    """docstring for WebsocketClient"""

    def __init__(self, address, send_message):
        super(WebsocketClient, self).__init__()
        self.address = address
        self.send_message = send_message
        self.recv = None

    def on_message(self, ws, message):
        self.recv = message
        print("on_client_message:", self.recv)

    def on_error(self, ws, error):
        print("### error:", error)

    def on_close(self, ws):
        print("### closed ###")

    def on_open(self, ws):
        print("on open")

        for i in range(1, 2):
            len = random.randint(0, 4096)
            data = generate_random_str(len)
            ws.send(data)

    def get_message(self):
        return self.recv

    def run(self):
        websocket.enableTrace(False)
        self.ws = websocket.WebSocketApp(self.address,
                                         on_message=self.on_message,
                                         on_error=self.on_error,
                                         on_close=self.on_close)
        self.ws.on_open = self.on_open
        self.ws.run_forever()


if __name__ == '__main__':
    ws_client = WebsocketClient("ws://127.0.0.1:9777", 'hello')
    ws_client.run()
    print(ws_client.get_message())

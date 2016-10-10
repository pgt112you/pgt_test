#coding:utf-8
import socket
import tornado.ioloop
import tornado.web

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world")

application = tornado.web.Application([
    (r"/", MainHandler),
])

if __name__ == "__main__":
    application.listen(8888)
    tornado.ioloop.IOLoop.instance().start()


host = "127.0.0.1"
port = 6379
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print type(s)
print dir(s)
s.connect((host, port))
get_str = "*2\r\n$3\r\nGET\r\n$3\r\naaa\r\n"
s.sendall(get_str)
get_reply_str = s.recv(1024)
s.close()
print get_reply_str


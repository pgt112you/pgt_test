#coding:utf-8
import datetime, time
import re
import hashlib, hmac
import os, sys, signal, fcntl, socket, struct
import json
import logging


sys.path.insert(0, os.path.join(os.path.abspath(os.path.dirname(__file__)), './lib'))

# 引入tornado相关的库
import tornado.ioloop
import tornado.web
from tornado.options import OptionParser
from tornado.log import enable_pretty_logging
import tornado.autoreload


host = "127.0.0.1"
port = 6379


class MainHandler(tornado.web.RequestHandler):
    @tornado.web.asynchronous  
    def get(self):
        f = open("/home/guangtong/pgt_test/aaa.txt", "a")
        f.write("111 >>>===>>>%s\n"% str(self._finished))
        get_str = "*2\r\n$3\r\nGET\r\n$3\r\naaa\r\n"
        if not self.application.mark:
            self.application.il.add_handler(self.application.s, self.get_data, tornado.ioloop.IOLoop.READ, 'abcdefg')
            #self.application.il.add_handler(self.application.s, self.get_data, tornado.ioloop.IOLoop.ERROR)
            self.application.mark = True
            f.write("151515 >>>===>>>%s\n"% str(self._finished))
        self.application.s.sendall(get_str)
        self.write("Hello, world\n")
        f.write("222 >>>===>>>%s\n"% str(self._finished))
        f.close()

    def get_data(self, fd, events):
        print id(fd)
        print "fd is", fd.fileno()
        print "event is", events
        f = open("/home/guangtong/pgt_test/aaa.txt", "a")
        f.write("333 >>>===>>>%s %s\n"% (str(self._finished), str(self._auto_finish)))
        #get_reply_str = self.application.s.recv(1024)
        get_reply_str = fd.recv(1024)
        print "=====", get_reply_str
        f.write("444 >>>===>>>%s %s\n"% (str(self._finished), str(self._auto_finish)))
        self.write(get_reply_str)
        f.write("555 >>>===>>>%s %s\n"% (str(self._finished), str(self._auto_finish)))
        self.flush()
        self.finish()
        f.write("666 >>>===>>>%s\n"% str(self._finished))
        f.close()

    def get_error(self, fd, event):
        print id(fd)


class MicPic_Application(tornado.web.Application):
    def __init__(self):
        
        self.mark = False
        self.connect_to_redis()
        self.il = tornado.ioloop.IOLoop.current()
        settings = dict(
                   debug = True,
                   )
        handlers = [
            (r"^/test", MainHandler),
            #(r'.*', Custom_404_cls),  # 处理404的方法，这条匹配必须放在所有的uri匹配的最后面
        ]
        super(MicPic_Application, self).__init__(handlers,  **settings)

    def connect_to_redis(self):
        self.host = "127.0.0.1"
        self.port = 6379
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setblocking(0)
        try:
           self.s.connect((host, port))
        except:
           pass

    def close_redis(self):
        if self.s:
            self.s.close()



if __name__ == "__main__":
    
    io_loop = tornado.ioloop.IOLoop.instance()

    application = MicPic_Application()
    application.listen(8080, "127.0.0.1")
    tornado.autoreload.start(io_loop)
    io_loop.start()


import tornado.ioloop
import tornado.web

class MainHandler(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world") 
        print "hello world"

class MainHandler1(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world1")
        print "hello world1"

class MainHandler2(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world2")
        print "hello world2"

class MainHandler3(tornado.web.RequestHandler):
    def get(self):
        self.write("Hello, world3")
        print "hello world3"

def make_app():
    return tornado.web.Application([
        (r"/", MainHandler),
        (r"/prefix", MainHandler1),
        (r"/prefix/hello", MainHandler2),
        (r"/hello", MainHandler3),
    ])

if __name__ == "__main__":
    app = make_app()
    app.listen(8888)
    tornado.ioloop.IOLoop.current().start()

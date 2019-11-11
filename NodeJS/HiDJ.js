var http = require('http');

http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/html'});
  res.end('Hello DJ');
}).listen(8080);

//C:\Users\g00339428\OneDrive - GMIT\2019\Final Year Project\NodeJS
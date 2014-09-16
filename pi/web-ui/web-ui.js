
var http = require('http');
var express = require('express');
var bodyParser = require('body-parser')
var socketIo = require('socket.io');
var fs = require('fs');

// var deviceFileName = 'foo.txt';
// var deviceFileName = '/dev/tty.usbmodem14231';
var deviceFileName = '/dev/ttyACM0';
var deviceFileDescriptor = fs.openSync(deviceFileName, 'w');

var app = express();
app.use(bodyParser.urlencoded({ extended: false }));

app.get('/', function(req, res){
  res.sendfile('index.html');
});
app.get('/js/jquery-1.11.1.min.js', function(req, res) {
  res.sendfile('js/jquery-1.11.1.min.js');
});
app.get('/js/fastclick.js', function(req, res) {
  res.sendfile('js/fastclick.js');
});
app.get('/css/bootstrap.css', function(req, res) {
  res.sendfile('css/bootstrap.css');
});
app.get('/css/bootstrap.css.map', function(req, res) {
  res.sendfile('css/bootstrap.css.map');
});

var server = http.Server(app);
server.listen(3000, function(){
  console.log('listening on *:3000');
});

var controlCodes = [
  {
    'l': 'a',
    'r': 'd',
    'd': 's',
    '1': 'q',
    '2': 'e',
  },
  {
    'l': 'j',
    'r': 'l',
    'd': 'k',
    '1': 'u',
    '2': 'o',
  }
];

var playerSockets = [null, null];
var serverSocketIo = socketIo(server);
serverSocketIo.on('connection', function(socket) {
  var playerId = null;
  if (playerSockets[0] == null) {
    playerId = 0;
  } else if (playerSockets[1] == null) {
    playerId = 1;
  }
  socket.emit('playerId', playerId);
  if (playerId != null) {
    playerSockets[playerId] = socket;
    socket.on('button', function(key) {
      // socket.emit('chat message', msg);
      // console.log(playerId + ': ' + key);
      var controlCode = controlCodes[playerId][key];
      fs.write(deviceFileDescriptor, new Buffer(controlCode), 0, 1, null, null);
    });
    socket.on('disconnect', function() {
      playerSockets[playerId] = null;
    });
  }
});

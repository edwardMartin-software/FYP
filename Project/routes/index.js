var express = require('express');
var loggedIn = false;
var router = express.Router();
var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27017/";
var message = "";
var awsIot = require('aws-iot-device-sdk');
var device = awsIot.device({
  keyPath: 'secrets/ed6daf9cf4-private.pem.key',
  certPath: 'secrets/ed6daf9cf4-certificate.pem.crt',
  caPath: 'secrets/AmazonRootCA1.pem',
  clientId: 'MyConnect',
  host: 'a1s5wjum8ai2m6-ats.iot.eu-west-1.amazonaws.com'
});

var AdminLoggedIN = false;
var loggedIn = false;
var count = 0;
var attemptedUser = {
  name: "",
  password: "",
  setDetails(name, password) {
    this.name = name;
    this.password = password;
  }
};

router.post('/login', function(req, res, next) {
  var user = {
    name: req.body.uname,
    password: req.body.psw
  };

  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("userDb");
    var arr = dbo.collection("Users").find({
      User: req.body.uname
    }).toArray(function(err, result) {
      if (err) throw err;
      console.log("Got as far as here, array length is: " + result[0].User + "  " + result[0].Password);
      attemptedUser.setDetails(result[0].User, result[0].Password);
      if (user.name == attemptedUser.name && user.password == attemptedUser.password) {
        loggedIn = true;
        if (user.name == "Admin") {
          MongoClient.connect(url, function(err, db) {
            if (err) throw err;
            var dbo = db.db("userDb");
            var arr = dbo.collection("Users").find().toArray(function(err, result) {
              if (err) throw err;
              res.render('adminPage', {
                title: "Admin Page - Cloud",
                items: result
              });
            });
          });
        } else {
          res.render('UserHome', {
            title: "User Home - Edwards FYP",
            user: attemptedUser.name
          });
        }
      } else {
        res.render('error', {
          message: "Details incorrect",
          username: user.name,
          password: user.password
        });
      }
      db.close();
    });
  });
  console.log("User: " + user.name + " Attempted: " + attemptedUser.name)
});

router.post('/delete', function(req, res, next) {
  var item = {
    User: req.body.User1,
  };
  if (item.User == 'Admin') {
    res.redirect('/login');
  }
  console.log(item);
  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("userDb");
    dbo.collection("Users").deleteOne(item, function(err, result) {

      console.log('Item Deleted Successfully');
    });
    dbo.close();
  });
  AdminLoggedIN = true;
  res.redirect('/login');
});

router.post('/insert', function(req, res, next) {
  var item = {
    User: req.body.username,
    Password: req.body.psw
  };
  console.log(item);
  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("userDb");
    dbo.collection("Users").insertOne(item, function(err, result) {

      console.log('Item Inserted Successfully');
    });
    dbo.close();
  });
  AdminLoggedIN = true;
  res.redirect('/login');
});

router.get('/login', function(req, res, next) {
  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("userDb");
    var arr = dbo.collection("Users").find().toArray(function(err, result) {
      console.log("Admin Logged In: " + AdminLoggedIN);
      if (err) throw err;
        if (AdminLoggedIN) {
          res.render('adminPage', {
            title: "Admin Page - Edwards FYP",
            items: result
          });
        } else {
          res.render('UserHome', {
            title: "User Home - Edwards FYP",
            user: attemptedUser.name
          });
        }
      db.close();
    });
  });
});

router.get('/', function(req, res, next) {
  AdminLoggedIN = false;
  loggedIn = false;
  res.render('login');
});

router.get('/Drive?', function(req, res, next) {
  //if(loggedIn)
  res.render('MotorControl');
});

device
  .on('connect', function() {
    console.log('connect');
    device.subscribe('esp32/pub');
    device.publish('esp32/sub', JSON.stringify({
      test_data: 'NodeJS server connected...'
    }));
  });
  var countMessages = 0;
device
  .on('message', function(topic, payload) {

    message = payload.toString();
    var data = JSON.parse(message);
    if(Started){

        MongoClient.connect(url, function(err, db) {
          if (err) throw err;
      var dbo = db.db("rssiDB");

      data.Time = countMessages;
      console.log("Time: " + data.Time + " RSSI1: "+data.RSSI1 + " RSSI2: "+data.RSSI2 + " RSSI3: "+data.RSSI3);
      dbo.collection(""+routeName).insertOne(data, function(err, res) {
        if (err) throw err;
        console.log("1 document inserted");
        db.close();
      });
    });
    countMessages = countMessages + 1000;
  }

    console.log(message);
  });

var time = 10000;
router.get('/Data', function(req, res, next) {
  console.log(time);

  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("rssiDB");
    var arr = dbo.collection("rssiDatas").find({
      Time: time
    }).toArray(function(err, result) {
      if (err) {
        throw err;
      }
      time = time + 1000;
      if (result.length == 0) {
        time = 0;
        res.sendStatus(404);
      } else {
        res.json(result[0]);
        console.log(result[0]);
      }
    });
    // time = time + 1000;
    // if(arr[0] == undefined){
    //   time = 0;
    // }else{
    //   res.send(arr);
    //   console.log(arr);
    // }
  });
});

router.post('/SendControl', function(req, res, next) {
  var myObj = req.body;
  //if(loggedIn){
  device.publish('esp32/sub', JSON.stringify({
    "drive": myObj.driveSpeed,
    "turn": myObj.turnSpeed,

  }));
  console.log("esp32/sub " + myObj.driveSpeed);
  res.sendStatus(200);
  //}
});
var routeName;
var Started = false;
router.post('/Name', function(req,res,next){
  var obj = req.body;
  routeName = obj.name;
  MongoClient.connect(url, function(err, db) {
  if (err) throw err;
  var dbo = db.db("rssiDB");
  dbo.collection(""+routeName).drop();
  dbo.createCollection(""+routeName, function(err, res) {
    if (err) throw err;
    console.log("Collection created!");
    db.close();
  });
});

  res.redirect('/AddRoute');
});
router.get('/AddRoute', function(req,res,next){
  device.publish('esp32/sub', JSON.stringify({
    "Start": "true"
  }));
  Started = true;
  console.log("Route name:" + routeName);
  res.render('AddRoute');
});

router.get('/Stop', function(req, res, next){
  device.publish('esp32/sub', JSON.stringify({
    "Start": "false"
  }));
  loggedIn = true;
  Started = false;
  countMessages = 0;
  res.redirect('/login');
});
var routes = [];
router.get('/CurrentRoute', function(req, res, next) {
  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("rssiDB");
    dbo.listCollections().toArray(function(err, collections) {
      if (err) console.log(err);
       res.render('selectRoute', {title: "Edwards FYP" , items: collections});

    });


  });

});
var routeName1 = { name: ""};
router.post('/SelectedRoute', function(req, res, next) {
  routeName1.name = req.body.route;
  console.log(routeName1.name);
  res.redirect('/showRoute');
});

router.get('/showRoute', function(req,res,next){
  MongoClient.connect(url, function(err, db) {
    if (err) throw err;
    var dbo = db.db("rssiDB");
    var arr = dbo.collection(""+routeName1.name).find().toArray(function(err, result) {
      if (err) throw err;
      res.render('currentRoute', {
        title: "My Page",
        username: attemptedUser.name,
        items: result,
        size: result.lenght
      });
    });
  });
});


module.exports = router;

var express = require('express');
var router = express.Router();
var mongoose = require('mongoose');
mongoose.connect('mongodb://localhost:27017/rssiDb');
var Schema = mongoose.Schema;

var userDataSchema = new Schema({
  Time: String,
  RSSI3: String,
  RSSI2: String,
  RSSI1: String
});

var MongoClient = require('mongodb').MongoClient;
var url = "mongodb://localhost:27017/";

// MongoClient.connect(url, function(err, db){
//   if(err) throw err;
//   var dbo = db.db("rssiDb");
//   var arr = dbo.collection("rssiDatas").find().toArray(function(err, result){
//     if(err) throw err;
//     console.log(result);
//     console.log("Got as far as here, array length is: " + result.length);
//     res.render('index', {title: "My Page", items: result});
//     db.close();
//
//   });
// });


var UserData = mongoose.model('rssiData', userDataSchema);

router.get('/', function(req,res,next){
    res.render('login');
});


router.post('/login', function(req, res, next) {
  // UserData.find()
  //     .then(function(doc) {
  //       console.log("Got as far as here, array length is: " + returned.length);
  //       res.render('index', {title: "My Page", items: doc});
  //     });
  var user = { name: req.body.uname,
               password: req.body.psw};

  var Admin = {
      name: "Admin",
      password: "1234"
  };
  MongoClient.connect(url, function(err, db){
    if(err) throw err;
    var dbo = db.db("rssiDB");
    var arr = dbo.collection("rssiDatas").find().toArray(function(err, result){
      if(err) throw err;
      console.log(result);
      console.log("Got as far as here, array length is: " + result.length);
      if(user.name == Admin.name && user.password == Admin.password){
      res.render('index', {title: "My Page", items: result});
    }
      db.close();
      });
});
});

module.exports = router;

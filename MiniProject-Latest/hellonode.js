var MongoClient = require('mongodb').MongoClient;
var url = 'mongodb://localhost/test';

MongoClient.connect(url, function(err, db) {

    var cursor = db.collection('TestData').find();

    cursor.each(function(err, doc) {

        console.log(doc);

    });
});

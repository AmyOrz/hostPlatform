var express = require("express");
var http = require("http");
var path = require('path');
var app = express();
var port = 5555;

app.use(express.static("public"));

app.get('/', function (req, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/sign', function (req, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/homePage', function (req, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/targetProject', function (req, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/publicProject', function (req, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/userActive', function (req, res) {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.get('/runWithNoWorker', function (req, res) {
    res.sendFile(path.join(__dirname, 'runWithNoWorker.html'));
});

app.get('/runWithWorker', function (req, res) {
    res.sendFile(path.join(__dirname, 'runWithWorker.html'));
});

http.createServer(app).listen(port, function () {
    console.log('Static server listening on port %d', port);
});

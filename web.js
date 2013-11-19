var express = require('express');

var app = express();

app.use(express.logger());

app.get('/', function (req, res) {
    res.end("Hello world!");
});

app.listen(process.env.PORT || 80, function () {
    console.log("Listening on " + (process.env.PORT || 80) + " ...");
});

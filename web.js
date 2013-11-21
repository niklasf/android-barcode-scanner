var express = require('express'),
    qrcode = require('qrcode');

var BACKEND = require('./config.js').BACKEND;

var channels = require('./channels').create();

var app = express();
app.use(express.logger());

app.param('channel', function (req, res, next, token) {
    if (/^[0-9a-f]{1,64}$/.exec(token)) {
        req.channel = token;
        next();
    } else {
        next('route');
    }
});

app.get('/channel/:channel/', function (req, res) {
    setTimeout(function() {
        res.setHeader('Content-Type', 'text/plain');

        var respondNow = function() {
            res.end(channels.get(req.channel));
            channels.unsubscribe(req.channel, respondNow);
        };

        if (channels.get(req.channel)) {
            respondNow();
        } else {
            channels.subscribe(req.channel, respondNow);
            setTimeout(respondNow, 15000);
        }
    }, Math.floor(Math.random() * 20));
});

app.delete('/channel/:channel/', function (req, res) {
    setTimeout(function() {
        res.setHeader('Content-Type', 'text/plain');

        var respondNow = function() {
            res.end(channels.get(req.channel));
            channels.clear(req.channel);
            channels.unsubscribe(req.channel, respondNow);
        };

        if (channels.get(req.channel)) {
            respondNow();
        } else {
            channels.subscribe(req.channel, respondNow);
            setTimeout(respondNow, 15000);
        }
    }, Math.floor(Math.random() * 20));
});

app.put('/channel/:channel/', function (req, res) {
    var data = '';

    req.setEncoding('utf8');

    req.on('data', function(chunk) {
        data += chunk;
    });

    req.on('end', function() {
        setTimeout(function () {
            channels.append(req.channel, data);
            res.end(channels.get(req.channel));
        }, Math.floor(Math.random() * 20));
    });
});

app.get('/channel/:channel/qr.png', function (req, res) {
    res.setHeader('Content-Type', 'image/png');

    qrcode.draw(BACKEND + 'BarcodeScanner.apk#' + req.channel, function (err, canvas) {
        if (err) {
            throw err;
        }

        canvas.pngStream().pipe(res);
    });
});

app.listen(process.env.PORT || 80, function () {
    console.log("Listening on " + (process.env.PORT || 80) + " ...");
});

setInterval(function () {
    channels.flip();
}, 15000);

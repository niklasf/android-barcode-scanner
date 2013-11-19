var PLAY_STORE_URL = 'http://play.google.com/store/apps/details?id=de.tuc.barcodescanner';

var express = require('express'),
    qrcode = require('qrcode'),
    crypto = require('crypto');

var app = express();

var channels = require('./channels').create();

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
        res.end(channels.get(req.channel));
    }, Math.floor(Math.random() * 20));
});

app.get('/channel/:channel/qr.png', function (req, res) {
    res.setHeader('Content-Type', 'image/png');

    qrcode.draw(PLAY_STORE_URL + '#' + req.channel, function (err, canvas) {
        if (err) {
            throw err;
        }

        canvas.pngStream().pipe(res);
    });
});

app.listen(process.env.PORT || 80, function () {
    console.log("Listening on " + (process.env.PORT || 80) + " ...");
});

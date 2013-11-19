var express = require('express'),
    qrcode = require('qrcode'),
    crypto = require('crypto');

var app = express();

app.use(express.logger());

app.get('/', function (req, res) {
    res.setHeader('Content-Type', 'text/plain');

    crypto.randomBytes(20, function (err, buffer) {
        if (err) {
            throw err;
        }

        var token = buffer.toString('hex');
        console.log(token);

        qrcode.draw(token, function (err, canvas) {
            if (err) {
                throw err;
            }

            canvas.pngStream().pipe(res);
        });
    });
});

app.listen(process.env.PORT || 80, function () {
    console.log("Listening on " + (process.env.PORT || 80) + " ...");
});

exports.testChannelFlipping = function(test) {
    var channels = require('./channels').create();

    // Channels are empty initially.
    test.strictEqual(channels.get('abc'), '');
    test.strictEqual(channels.get('fff'), '');

    // Put something into the channels.
    channels.append('fff', 'alpha');
    test.equal(channels.get('fff'), 'alpha\n');
    channels.append('fff', 'beta');
    test.equal(channels.get('fff'), 'alpha\nbeta\n');

    // Nothing is changed after one flip.
    channels.flip();
    test.equal(channels.get('fff'), 'alpha\nbeta\n');

    // Things are cleared after two flips.
    channels.flip();
    test.equal(channels.get('fff'), '');

    test.done();
};

exports.testChannelFiltering = function(test) {
    var channels = require('./channels').create();

    // Ensure tabs and line breaks are filtered.
    channels.append('fff', 'a bc-_01äöüß\t\r\n@{&\n');
    test.equal(channels.get('fff'), 'a bc-_01äöüß@{&\n');

    // Ensure unicode symbols are not filtered.
    channels.append('aaa', '♕♗');
    test.equal(channels.get('aaa'), '♕♗\n');

    test.done();
};

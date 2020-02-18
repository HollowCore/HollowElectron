const native = require('./build/Release/HollowElectronNative.node');

function Path(svgPathData) {
    native.pathInit(svgPathData);
}

Path.prototype.elementCount = function() {
  return native.pathElementCount();
};

module.exports = {
    Path: Path
}

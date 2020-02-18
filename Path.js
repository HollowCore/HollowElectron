const native = require('./build/Release/HollowElectronNative.node');

function Path(svgPathData) {
    native.pathInit(svgPathData);
}

Path.prototype.elementCount = function() {
  return native.pathElementCount();
};

Path.prototype.currentPoint = function() {
  return native.pathCurrentPoint();
};

Path.prototype.bounds = function() {
  return native.pathBounds();
};

module.exports = {
    Path: Path
}

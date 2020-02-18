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

Path.prototype.move = function(x, y) {
  return native.pathMove(x, y);
};

Path.prototype.addLine = function(x, y) {
  return native.pathAddLine(x, y);
};

Path.prototype.addQuadraticCurve = function(cx, cy, x, y) {
  return native.pathAddQuadraticCurve(cx, cy, x, y);
};

Path.prototype.addCubicCurve = function(cx0, cy0, cx1, cy1, x, y) {
  return native.pathAddCubicCurve(cx0, cy0, cx1, cy1, x, y);
};

Path.prototype.close = function() {
  return native.pathClose();
};

module.exports = {
    Path: Path
}

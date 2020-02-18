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
    native.pathMove(x, y);
};

Path.prototype.addLine = function(x, y) {
    native.pathAddLine(x, y);
};

Path.prototype.addQuadraticCurve = function(cx, cy, x, y) {
    native.pathAddQuadraticCurve(cx, cy, x, y);
};

Path.prototype.addCubicCurve = function(cx0, cy0, cx1, cy1, x, y) {
    native.pathAddCubicCurve(cx0, cy0, cx1, cy1, x, y);
};

Path.prototype.close = function() {
    native.pathClose();
};

Path.prototype.containsPoint = function(point) {
    return native.pathContainsPoint(point);
};

Path.prototype.intersectsPath = function(other) {
    return native.pathIntersectsPath(other);
};

module.exports = {
    Path: Path
}

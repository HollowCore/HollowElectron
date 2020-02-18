const {app, ipcMain, BrowserWindow} = require('electron')
const filePath = require('path')
const native = require('./build/Release/HollowElectronNative.node');
const {Path} = require('./Path.js');

// App Events
app.on('ready', function () {
    // Create the browser window
    const mainWindow = new BrowserWindow({
    width: 1280,
    height: 720,
    webPreferences: {
            preload: filePath.join(app.getAppPath(), 'index_preload.js')
        }
    })

    // Load index.html as the window content
    mainWindow.loadFile('index.html')

    // Open developer tools
//    mainWindow.webContents.openDevTools()
})

app.on('window-all-closed', function () {
    app.quit()
})

// IPC Events
ipcMain.on('go', (event) => {
    const path = new Path("M 10 10 L 40 10 Z");
    path.move(10.0, 15.0);
    path.addLine(40.0, 15.0);
    path.addLine(40.0, 40.0);
    path.addLine(15.0, 40.0);
    path.close();
    path.addQuadraticCurve(25.0, -10.0, 40.0, 15.0)
    path.close();
    path.addCubicCurve(15.0, -10.0, 35.0, -10.0, 40.0, 15.0)
    path.close();
           
    const elementCount = path.elementCount();
    const currentPoint = path.currentPoint();
    const bounds = path.bounds();
    const containsPoint = path.containsPoint({x: 30.0, y: 20.0});
    const containsPoint2 = path.containsPoint({x: 100.0, y: 20.0});
//    const intersectsPath = path.intersectsPath(new Path("M 5 5 L 50 50"));

    const result = `Path:\n
        elementCount = ${JSON.stringify(elementCount)}\n
        currentPoint = ${JSON.stringify(currentPoint)}\n
        bounds = ${JSON.stringify(bounds)}\n
        containsPoint = ${JSON.stringify(containsPoint)}\n
        containsPoint2 = ${JSON.stringify(containsPoint2)}\n
    `;
    
    event.returnValue = result;
})

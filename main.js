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
    const path = new Path("M 10 15 L 40 15 40 40 15 40 Z");
    const elementCount = path.elementCount();
    const currentPoint = path.currentPoint();
    const bounds = path.bounds();
    const result = `Path:\n
        elementCount = ${JSON.stringify(elementCount)}\n
        currentPoint = ${JSON.stringify(currentPoint)}\n
        bounds = ${JSON.stringify(bounds)}\n
    `;
    event.returnValue = result;
})

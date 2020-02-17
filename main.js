const {app, ipcMain, BrowserWindow} = require('electron')
const path = require('path')
const native = require('./build/Release/HollowElectronNative.node');

// App Events
app.on('ready', function () {
  // Create the browser window
  const mainWindow = new BrowserWindow({
    width: 1280,
    height: 720,
    webPreferences: {
      preload: path.join(app.getAppPath(), 'index_preload.js')
    }
  })

  // Load index.html as the window content
  mainWindow.loadFile('index.html')

  // Open developer tools
//  mainWindow.webContents.openDevTools()
})

app.on('window-all-closed', function () {
    app.quit()
})

// IPC Events
ipcMain.on('go', (event) => {
  const contourPath = native.createPath("M 10 10 L 40 10 40 40 10 40 Z")
  const result = `Path element count is ${native.pathElementCount(contourPath)}`;
  event.returnValue = result;
})

const {ipcRenderer} = require('electron');

// Expose IPC to renderers
window.sendSync = function (message, payload) {
    return ipcRenderer.sendSync(message, payload)
}

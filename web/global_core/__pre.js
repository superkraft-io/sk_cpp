function __ipcSendMsg(data) {
    if (__sk_ipc_sendMsg) __sk_ipc_sendMsg(data)
    else if (IPlugSendMsg) IPlugSendMsg(data)
    else if (window.__JUCE__) window.__JUCE__.backend.emitEvent(data.cmd, data.payload)
}
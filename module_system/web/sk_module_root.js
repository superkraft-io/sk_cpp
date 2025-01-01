class SK_Module_Root {
    constructor(){
        this.path = ''
    }

    sync(operation, payload){
        return SK_Module.syncOperation(this.path, operation, payload)
    }

    async(operation, payload){
        return SK_Module.asyncOperation(this.path, operation, payload)
    }
}
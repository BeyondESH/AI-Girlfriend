function addChatMessage(listModel,text,role="User"){
    listModel.append({"context":text,"role":role})
}

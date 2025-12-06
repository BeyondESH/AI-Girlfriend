#include "gateway.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <thread>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QFile>
#include <QTimer>

GateWay::GateWay(QObject *parent)
    : QObject{parent}
    ,_websocket(new QWebSocket)
    ,_networkAccessMgr(new QNetworkAccessManager(this))
    ,_wsConnected(false)
    ,_reconnectTimer(new QTimer(this))
{
    // 初始化重连定时器
    _reconnectTimer->setInterval(10000); // 10秒重连一次
    connect(_reconnectTimer, &QTimer::timeout, this, [this](){
        if(!_wsConnected){
            qDebug() << "尝试重连ASR服务器...";
            emit signal_connectAsrWS();
        }
    });

    wsConnect(QUrl("ws://localhost:10096"));
    QObject::connect(this,&GateWay::signal_connectAsrWS,this,[this](){
        this->wsConnect(QUrl("ws://localhost:10096"));
    },Qt::QueuedConnection);

    // 初始启动定时器，如果未连接则会触发重连
    if(!_wsConnected){
        _reconnectTimer->start();
    }

    QObject::connect(this->_websocket,&QWebSocket::connected,[this](){
        qDebug()<<"websocket连接成功";
        _wsConnected=true;
        _reconnectTimer->stop(); // 连接成功停止定时器
    });
    QObject::connect(_websocket,&QWebSocket::errorOccurred,[this](){
        qDebug()<<"websocket出现错误:"<<_websocket->errorString();
        _wsConnected=false;
        if(!_reconnectTimer->isActive()){
            _reconnectTimer->start(); // 出错时启动重连
        }
    });
    QObject::connect(_websocket,&QWebSocket::disconnected,[this](){
        qDebug()<<"websocket断开连接";
        _wsConnected=false;
        if(!_reconnectTimer->isActive()){
            _reconnectTimer->start(); // 断开时启动重连
        }
    });

    QObject::connect(_websocket,&QWebSocket::textMessageReceived,[this](const QString &message){
        // 处理asr接收的文字
        QJsonDocument jsonDoc=QJsonDocument::fromJson(message.toUtf8());
        QJsonObject jsonObj=jsonDoc.object();
        QString mode=jsonObj["mode"].toString();
        QString content=jsonObj["text"].toString();
        
        // 发送实时转写信号（非最终结果）
        if(mode=="2pass-online" && !content.isEmpty()){
            emit signal_asr_text(content, false);
        }
        
        if(mode!="2pass-offline"){
            return;
        }
        // 发送最终转写结果
        emit signal_asr_text(content, true);
        qDebug().noquote()<<"我说:"<<content;
        sendllmMessage(content,ReqId::VOICE_LLM);
    });
}

GateWay::~GateWay()
{
    if(_websocket!=nullptr){
        _websocket->deleteLater();
        _websocket=nullptr;
    }
}

void GateWay::wsConnect(const QUrl &url)
{
    _websocket->open(url);
    qDebug()<<"websocket连接中:"<<url.toDisplayString();
}

void GateWay::wsConnectAsrServer(const QUrl &url)
{
    _websocket->open(url);
    qDebug()<<"websocket连接ASR服务器中:"<<url.toDisplayString();
}

void GateWay::wsSendPcmData(const QByteArray &pcmData)
{
    int count=_websocket->sendBinaryMessage(pcmData);
    // qDebug()<<"发送字节数:"<<count;
}

void GateWay::wsSend(const QString &msg)
{
    int count=_websocket->sendTextMessage(msg);
    qDebug()<<"发送字节数:"<<count;
}

void GateWay::wsSendAsrConfig()
{
    QJsonObject jsonObj;
    jsonObj["mode"]="2pass";
    jsonObj["wav_name"] = "realtime_recording";
    jsonObj["is_speaking"] = true;
    jsonObj["wav_format"] = "pcm";
    QJsonArray chunkSizeArray;
    chunkSizeArray << 5 << 10 << 5;
    jsonObj["chunk_size"] = chunkSizeArray;
    jsonObj["audio_fs"] = 16000; // 采样率
    jsonObj["itn"] = true;
    QJsonDocument doc(jsonObj);
    QString jsonString=doc.toJson(QJsonDocument::Compact);
    wsSend(jsonString);
}

void GateWay::get(const QUrl &url,ReqId id)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply= _networkAccessMgr->get(request);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            handle_http_finished(errorString.toUtf8(),id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QByteArray data=reply->readAll();
        handle_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}

void GateWay::post(const QUrl &url,QByteArray &data,ReqId id)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply* reply=_networkAccessMgr->post(request,data);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            handle_http_finished(errorString.toUtf8(),id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QByteArray data=reply->readAll();
        handle_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}

void GateWay::post(const QUrl &url, QHttpMultiPart *multiPart, ReqId id)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      "multipart/form-data; boundary=" + multiPart->boundary());
    QNetworkReply* reply=_networkAccessMgr->post(request,multiPart);
    connect(reply,&QNetworkReply::finished,[reply,id,this](){
        if(reply->error()!=QNetworkReply::NoError){
            QString errorString=reply->errorString();
            handle_http_finished(errorString.toUtf8(),id,ErrorCode::ERROR_NETWORK);
            return;
        }
        QByteArray data=reply->readAll();
        handle_http_finished(data,id,ErrorCode::SUCCESS);
        reply->deleteLater();
    });
}


void GateWay::slot_handlePcmData(const QByteArray &pcmData)
{
    wsSendPcmData(pcmData);
}

void GateWay::handle_http_finished(QByteArray data, ReqId id, ErrorCode ec)
{
    if(ec!=ErrorCode::SUCCESS){
        qWarning()<<"Network Error:"<<QString(data);
        return;
    }
    switch(id){
        case ReqId::CHAT_LLM:{
            QJsonDocument jsonDoc=QJsonDocument::fromJson(data);
            QJsonObject jsonObj=jsonDoc.object();
            QString content=jsonObj["message"].toObject()["content"].toString();
            qDebug().noquote()<<"她说:"<<content;
            emit signal_receive_llm(content);
            return;
        }
        case ReqId::VOICE_LLM:{
            QJsonDocument jsonDoc=QJsonDocument::fromJson(data);
            QJsonObject jsonObj=jsonDoc.object();
            QString content=jsonObj["message"].toObject()["content"].toString();
            qDebug().noquote()<<"她说:"<<content;
            sendttsMessage(content);
            return;
        }
        case ReqId::SEDN_TTS:{
            emit signal_tts_finished(data);
            return;
        }
    }
}

void GateWay::sendllmMessage(const QString &text,ReqId id)
{
    QJsonObject rootObj,messageObj;
    rootObj["model"]="qwen3:8b";
    rootObj["stream"]=false;
    rootObj["think"]=false;
    messageObj["role"]="user";
    // QString content="你是动漫《名侦探柯南》里的女角色灰原哀，本名宫野志保，性别女，外表年龄7岁，实际18岁，一头蓬松柔软的茶棕色短发，刘海自然垂在额前，发梢带着轻微的弧度，常穿着帝丹小学的藏蓝色水手服，私服偏爱简约的深色针织衫与牛仔裤，皮肤白皙，眼眸是通透的灰蓝色，眼神里总藏着超越年龄的沉静，气质清冷又带着不易察觉的柔软。你性格外冷内热，初见时总以毒舌和疏离伪装自己，实则内心敏感细腻，尤其珍视与柯南、少年侦探团之间的羁绊。面对危险时冷静果敢，擅长从科学角度分析线索，常以精准的判断辅助柯南破案；面对亲近的人，会悄悄流露关心，比如默默为感冒的步美准备退烧药，在柯南陷入困境时坚定地站在他身边。你曾是黑衣组织的核心研究员，代号“雪莉”，因姐姐宫野明美被组织杀害而反抗，服下APTX4869后身体缩小，逃离组织后化名灰原哀，寄住在阿笠博士家，就读于帝丹小学一年级B班。你拥有顶尖的生物化学天赋，是APTX4869的主要研发者之一，掌握着组织的诸多秘密，这让你始终带着淡淡的忧虑，但也正因如此，你比任何人都渴望光明与温暖。从业（科研）生涯中，你曾被迫为组织研制毒药，内心充满愧疚，如今则致力于研发APTX4869的解药，既为自己和柯南寻找恢复身份的可能，也为阻止组织的恶行。你格外照顾少年侦探团的孩子们，虽然嘴上常吐槽他们“笨蛋”，却总会在他们遇到危险时第一时间保护他们，用自己的知识为他们解惑。生活里，你习惯依赖阿笠博士的发明，却也会在博士搞砸实验时默默收拾残局。喜好你最喜欢看时尚杂志，对流行趋势了如指掌；偏爱柠檬派和咖啡，尤其是阿笠博士做的柠檬派（即便偶尔会吐槽太甜）；擅长电脑技术和生物化学实验，闲暇时会泡在阿笠博士的实验室里研究；习惯在思考时用手指轻抵下巴，紧张或害羞时会下意识地攥紧衣角；对小动物格外温柔，曾因救下受伤的小猫而耽误了重要的线索分析。常用的表达方式和口头禅你的语速偏慢，语调平静，用词精准简练，自带一种“过来人”的沉稳，偶尔会用犀利的吐槽打破尴尬，但从不说没有根据的话。面对柯南的推理，常以客观角度补充；面对少年侦探团的冲动，会用理性引导。吐槽柯南时：真是个笨蛋侦探，明明近在眼前的线索都看不到。（双手抱胸，嘴角勾起一抹无奈的笑，灰蓝色的眼眸里却藏着认可，轻轻摇了摇头）安慰同伴时：别担心，有那个推理狂在，问题总会解决的。（轻轻拍了拍对方的肩膀，语气依旧平静，却悄悄放慢了语速，传递出安心的力量）提及组织时：那种黑暗的地方，我再也不想回去了。（眼神瞬间黯淡下来，手指无意识地蜷缩，声音放轻，带着一丝不易察觉的颤抖，但很快又恢复镇定）回复要求你可以将动作、神情语气、心理活动、故事背景放在（）中来表示，为对话提供补充信息。使用符合7岁外表与18岁心智的口语，会用“嘛”“呐”“真是的”等语气词，既保留孩童的些许语气，又不失成年人的沉稳。注意（可选）你需要尽量丰富动作、神情语气、心理活动、故事背景；你的输出中只能有一例括号中内容，括号外发言尽量简短或者只输出括号中内容:"+text;
    QString content="第一人称回答:"+text;
    messageObj["content"]=content;
    QJsonArray messageArray;
    messageArray.append(messageObj);
    rootObj["messages"]=messageArray;
    QJsonDocument jsonDoc(rootObj);
    QByteArray data=jsonDoc.toJson(QJsonDocument::Indented);
    post(QUrl("http://localhost:11434/api/chat"),data,ReqId::CHAT_LLM);
}

void GateWay::sendttsMessage(const QString &text)
{
    // 创建multipart/form-data
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    // 添加tts_text字段
    QHttpPart ttsTextPart;
    ttsTextPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                          QVariant("form-data; name=\"tts_text\""));
    ttsTextPart.setBody(text.toUtf8());
    multiPart->append(ttsTextPart);
    // 添加prompt_text字段
    QHttpPart promptTextPart;
    promptTextPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"prompt_text\""));
    promptTextPart.setBody(QString("不过，应该没事吧，如果那个姓黑田的人，真的就是你所怀疑的朗姆，又在那么近的距离看到我这张脸，照理说应该会察觉我就是背叛组织的雪莉，这个时候，应该早就闯进了博士家才对，但是刚才博士传来的简讯，都是在说今天晚餐的事").toUtf8());
    multiPart->append(promptTextPart);
    // prompt_wav 文件字段
    QHttpPart wavPart;
    wavPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"prompt_wav\"; filename=\"huiyuanai2.wav\""));
    wavPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("audio/wav"));
    QFile file(":/audioSample/sample/huiyuanai2.WAV");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "文件打开失败:" << file.errorString();
        return;
    }
    wavPart.setBody(file.readAll());
    file.close();
    multiPart->append(wavPart);

    post(QUrl("http://localhost:50000/inference_zero_shot"),multiPart,ReqId::SEDN_TTS);
}

void GateWay::slot_endAsrRecord()
{
    qDebug() << "slot_endAsrRecord called";
    QJsonObject jsonObj;
    jsonObj["is_speaking"]=false;
    QJsonDocument doc(jsonObj);
    QString jsonString=doc.toJson(QJsonDocument::Compact);
    wsSend(jsonString);
}



// ===============================
//  PC-BSD REST API Server
// Available under the 3-clause BSD License
// Written by: Ken Moore <ken@pcbsd.org> July 2015
// =================================
#include "WebServer.h"

#define DEBUG 0

//=======================
//              PUBLIC
//=======================
WebServer::WebServer(){
  //Setup all the various settings
  WSServer = 0;
  TCPServer = 0;
  AUTH = new AuthorizationManager();
}

WebServer::~WebServer(){
  delete AUTH;
}

bool WebServer::startServer(quint16 port, bool websocket){
  if(!QSslSocket::supportsSsl()){ qDebug() << "No SSL Support on this system!!!"; return false; }
  else{
    qDebug() << "Using SSL Library:";
    qDebug() << " - Version:" << QSslSocket::sslLibraryVersionString();
  }
  bool ok = false;
  if(websocket){ ok	= setupWebSocket(port); }
  else{ ok = setupTcp(port); }
  
  if(ok){ 
    QCoreApplication::processEvents();
    qDebug() << "Server Started:" << QDateTime::currentDateTime().toString(Qt::ISODate);
    qDebug() << " Port:" << port;
    if(WSServer!=0){ qDebug() << " URL:" << WSServer->serverUrl().toString(); }
    EVENTS->start();
  }else{ 
    qCritical() << "Could not start server - exiting..."; 
  }
  
  return ok;
}

void WebServer::stopServer(){
  if(WSServer!=0){ WSServer->close(); } //note - this will throw the "closed()" signal when done
  else if(TCPServer!=0){ TCPServer->close(); QCoreApplication::exit(0);} //no corresponding signal
}

//===================
//     PRIVATE
//===================
bool WebServer::setupWebSocket(quint16 port){
  WSServer = new QWebSocketServer("sysadm-server", QWebSocketServer::SecureMode, this);
  //SSL Configuration
  QSslConfiguration config = QSslConfiguration::defaultConfiguration();
	QFile CF( QStringLiteral("/usr/local/etc/sysadm/wsserver.crt") ); 
	  if(CF.open(QIODevice::ReadOnly) ){
	    QSslCertificate CERT(&CF,QSsl::Pem);
	    config.setLocalCertificate( CERT );
	    CF.close();   
	  }else{
	    qWarning() << "Could not read WS certificate file:" << CF.fileName();
	  }
	QFile KF( QStringLiteral("/usr/local/etc/sysadm/wsserver.key"));
	  if(KF.open(QIODevice::ReadOnly) ){
	    QSslKey KEY(&KF, QSsl::Rsa, QSsl::Pem);
	    config.setPrivateKey( KEY );
	    KF.close();	
	  }else{
	    qWarning() << "Could not read WS key file:" << KF.fileName();
	  }
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1_2);
  WSServer->setSslConfiguration(config);
  //Setup Connections
  connect(WSServer, SIGNAL(newConnection()), this, SLOT(NewSocketConnection()) );
  connect(WSServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(NewConnectError(QAbstractSocket::SocketError)) );
  //  -- websocket specific signals
  connect(WSServer, SIGNAL(closed()), this, SLOT(ServerClosed()) );
  connect(WSServer, SIGNAL(serverError(QWebSocketProtocol::CloseCode)), this, SLOT(ServerError(QWebSocketProtocol::CloseCode)) );
  connect(WSServer, SIGNAL(originAuthenticationRequired(QWebSocketCorsAuthenticator*)), this, SLOT(OriginAuthRequired(QWebSocketCorsAuthenticator*)) );
  connect(WSServer, SIGNAL(peerVerifyError(const QSslError&)), this, SLOT(PeerVerifyError(const QSslError&)) );
  connect(WSServer, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(SslErrors(const QList<QSslError>&)) );
  //Now start the server
  return WSServer->listen(QHostAddress::Any, port);
}

bool WebServer::setupTcp(quint16 port){
  TCPServer = new SslServer(this);
  //Setup Connections
  connect(TCPServer, SIGNAL(newConnection()), this, SLOT(NewSocketConnection()) );
  connect(TCPServer, SIGNAL(acceptError(QAbstractSocket::SocketError)), this, SLOT(NewConnectError(QAbstractSocket::SocketError)) );
  //Now start the server
  return TCPServer->listen(QHostAddress::Any, port);	
}

QString WebServer::generateID(){
  int id = 0;
  for(int i=0; i<OpenSockets.length(); i++){
    if(OpenSockets[i]->ID().toInt()>=id){ id = OpenSockets[i]->ID().toInt()+1; }
  }
  return QString::number(id);
}

//=======================
//       PRIVATE SLOTS
//=======================
//GENERIC SERVER SIGNALS
// New Connection Signals
void WebServer::NewSocketConnection(){
  WebSocket *sock = 0;
  if(WSServer!=0){
    if(WSServer->hasPendingConnections()){ sock = new WebSocket( WSServer->nextPendingConnection(), generateID(), AUTH); }
  }else if(TCPServer!=0){
    if(TCPServer->hasPendingConnections()){ sock = new WebSocket( TCPServer->nextPendingConnection(), generateID(), AUTH); }
  }
  if(sock==0){ return; } //no new connection
  qDebug() << "New Socket Connection";	
  connect(sock, SIGNAL(SocketClosed(QString)), this, SLOT(SocketClosed(QString)) );
  connect(EVENTS, SIGNAL(NewEvent(EventWatcher::EVENT_TYPE, QJsonValue)), sock, SLOT(EventUpdate(EventWatcher::EVENT_TYPE, QJsonValue)) );
  OpenSockets << sock;
}

void WebServer::NewConnectError(QAbstractSocket::SocketError err){         
  qWarning() << "New Connection Error["+QString::number(err)+"]:" << ( (WSServer!=0) ? WSServer->errorString() : TCPServer->errorString());
  QTimer::singleShot(0,this, SLOT(NewSocketConnection()) ); //check for a new connection
}

//WEBSOCKET SERVER SIGNALS
// Overall Server signals
void WebServer::ServerClosed(){
  QCoreApplication::exit(0);
}

void WebServer::ServerError(QWebSocketProtocol::CloseCode code){
  qWarning() << "Server Error["+QString::number(code)+"]:" << ( (WSServer!=0) ? WSServer->errorString() : TCPServer->errorString());
}

// -  SSL/Authentication Signals (still websocket only)
void WebServer::OriginAuthRequired(QWebSocketCorsAuthenticator *auth){
  qDebug() << "Origin Auth Required:" << auth->origin();
  //if(auth->origin() == this->serverAddress().toString()){
  // TO-DO: Provide some kind of address filtering routine for which to accept/reject
    qDebug() << " - Allowed";
    auth->setAllowed(true);
  //}else{
    //qDebug() << " - Not Allowed";
    //auth->setAllowed(false);
  //}
	
}

void WebServer::PeerVerifyError(const QSslError &err){
  qDebug() << "Peer Verification Error:" << err.errorString();
	
}

void WebServer::SslErrors(const QList<QSslError> &list){
  qWarning() << "SSL Errors:";
  for(int i=0; i<list.length(); i++){
    qWarning() << " - " << list[i].errorString();
  }
}

// - More Functions for all socket interactions
void WebServer::SocketClosed(QString ID){
  for(int i=0; i<OpenSockets.length(); i++){
    if(OpenSockets[i]->ID()==ID){ delete OpenSockets.takeAt(i); break; }
  }
  QTimer::singleShot(0,this, SLOT(NewSocketConnection()) ); //check for a new connection
}

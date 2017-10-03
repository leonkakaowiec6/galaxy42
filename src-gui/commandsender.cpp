#include <QDebug>

#include "commandsender.h"
#include "sendedcommand.h"

CommandSender::CommandSender(commandExecutor* executor,QObject *parent)
                : QObject(parent),m_executor(executor),m_counter(QString("fla"))
{
    m_sended = new SendedCommands(this);
    startCommunication();
}

void CommandSender::startCommunication()
{
    auto get_rpc_name = prepareCommand(orderType::GETNAME);
    m_executor->sendNetRequest(*get_rpc_name);
    m_wait_name_flag = true;
}

void CommandSender::onGetName(const QString& client_name)
{
    m_client_name = client_name;
    m_counter.setRpcName(m_client_name);
    m_wait_name_flag = false;
}

std::shared_ptr<order> CommandSender::getOrder(const QString &rpc_num)
{
    return m_sended->getOrder(rpc_num);
}

void CommandSender::sendCommand(orderType type, const MeshPeer &peer)
{
    if(m_wait_name_flag) {
        throw std::runtime_error ("can't send msg while witing for rpc name of client");
    }

    try{
            auto ord = prepareCommand(type,peer);
            m_executor->sendNetRequest(*ord);
            m_sended->addOrder(ord,QString::fromStdString(ord->getId()));

    } catch(std::runtime_error &e) {
        qDebug()<< "exception while sending :" << e.what();
    }

}

void CommandSender::sendCommand(CommandSender::orderType type)
{
    if(m_wait_name_flag && type != orderType::GETNAME) {
        throw std::runtime_error ("can't send msg while witing for rpc name of client");
    }

    try{
            auto ord = prepareCommand(type);
            m_executor->sendNetRequest(*ord);
            m_sended->addOrder(ord,QString::fromStdString(ord->getId()));
    } catch(std::runtime_error &e) {
        qDebug()<< "exception while sending :" << e.what();
    }
}

std::shared_ptr<order> CommandSender::prepareCommand(CommandSender::orderType type)
{
    std::shared_ptr<order> ord;

    switch (type) {
    case orderType::PING:
        if(m_client_name.size() == 0)
            throw std::runtime_error ("no client name provided - can't send command");
            ord = std::make_shared<pingOrder>( new pingOrder(m_counter.getRpcId()));		//! @todo przerobic na zwaierajaca rpc_num
        break;
    case orderType::DELETEALL:
        if(m_client_name.size() == 0)
            throw std::runtime_error ("no client name provided - can't send command");
            ord = std::make_shared<deleteAllPeersOrder>(new deleteAllPeersOrder(m_counter.getRpcId()));
        break;
    case orderType::BANALL:
        if(m_client_name.size() == 0)
            throw std::runtime_error ("no client name provided - can't send command");
            ord = std::make_shared<banAllOrder> (new banAllOrder(m_counter.getRpcId()));
        break;
    case orderType::GETNAME:
            ord = std::make_shared<getClientName> (new getClientName());
        break;
    default:
        throw std::runtime_error ("unproper number of parameters for this function");
        break;
    }
    return ord;
}

std::shared_ptr<order> CommandSender::prepareCommand(CommandSender::orderType type, const MeshPeer &peer)
{
    std::shared_ptr<order> ord;

    switch (type) {
        case orderType::ADDPEER:
        if(m_client_name.size() == 0)
            throw std::runtime_error ("no client name provided - can't send command");
              ord = std::make_shared<addPeerOrder> (new addPeerOrder(m_counter.getRpcId(),peer));
        break;

        case orderType::DELETEPEER:
        if(m_client_name.size() == 0)
            throw std::runtime_error ("no client name provided - can't send command");
            ord = std::make_shared<deletePeerOrder> (new deletePeerOrder(m_counter.getRpcId(),peer));
        break;

        case orderType::BANPEER:
            if(m_client_name.size() == 0)
            throw std::runtime_error ("no client name provided - can't send command");
            ord = std::make_shared<banPeerOrder> (new banPeerOrder(m_counter.getRpcId(),peer));
        break;
        default:
            throw std::runtime_error ("can't create order with this params list");
        }

    return ord;
}
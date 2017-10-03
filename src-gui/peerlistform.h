#ifndef PEERLISTFORM_H
#define PEERLISTFORM_H

#include <QWidget>

#include "meshpeer.h"
#include "peersmodel.h"

namespace Ui {
class PeerListForm;
}

class PeerListForm : public QWidget
{
    Q_OBJECT

   peersModel *m_model;

   QModelIndex m_index; 	//last selected index

public:
    explicit PeerListForm(QWidget *parent = 0);
    ~PeerListForm();

    peersModel* getModel();

    void onPeerBanned(const QString &vip);
    void onPeerAdded(const QString &vip);
public slots:

    void addActionSlot(bool);
    void banActionSlot(bool);
    void removeActionSlot(bool);
    void pingActionSlot(bool);
    void sendMessageActionSlot(bool);
    void findActionSlot(bool);
    void onPeerFounded(QString, QString);

    void onPeerRemoved(const QString &peer);

    void addPeer(const MeshPeer & peer);
signals:

    void pingPeer(QString);
    void banPeer(QString);
    void removePeer(QString);
    void addPeer(QString);
    void sendMessage(QString,QString);
    void findPeer(QString);

protected slots:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    Ui::PeerListForm *ui;
};

#endif // PEERLISTFORM_H
#include "basic/ItemCart.h"
#include "basic/ItemOffer.h"
#include "util/Parser.h"
#include "module/ReceiptPrinter.h"

#include <QString>
#include <QTextStream>

#include <QList>
#include <QHash>
#include <QMultiMap>

QT_USE_NAMESPACE

/******************** Receipt ********************/
Receipt::Receipt(const QString &strStore) {
    m_strStore = strStore;
}

QString Receipt::GetStoreName() const {
    return m_strStore;
}

void Receipt::InsertItem(const QString &strInfo) {
    m_strItemInfoList.append(strInfo);
}

void Receipt::InsertItem(const QList<QString> &strInfoList) {
    m_strItemInfoList.append(strInfoList);
}

void Receipt::InsertOffer(const QString &strInfo) {
    m_strOfferInfoList.append(strInfo);
}

void Receipt::InsertOffer(const QList<QString> &strInfoList) {
    m_strOfferInfoList.append(strInfoList);
}

void Receipt::SetCost(const QString &strInfo) {
    m_strCostInfo = strInfo;
}

QString Receipt::GetReceipt() const {
    QString strReceipt;

    // head
    strReceipt += QString("***<%1>购物清单***\n")
            .arg(this->GetStoreName());

    // items
    for (int i = 0; i < m_strItemInfoList.size(); ++i) {
        strReceipt += m_strItemInfoList.at(i);
    }
    strReceipt += QString("----------------------\n");

    // offer
    for (int i = 0; i < m_strOfferInfoList.size(); ++i) {
        strReceipt += m_strOfferInfoList.at(i);
        strReceipt += QString("----------------------\n");
    }

    // total
    strReceipt += m_strCostInfo;

    // tail
    strReceipt += QString("**********************\n");

    return strReceipt;
}

QString Receipt::GetReceipt(const QString &strStore, const Cart &cart) {
    Receipt rept(strStore);

    // used for offer info
    QHash<QString, ItemOffer *> pOfferMap;
    QMultiMap<QString, Item *> pOfferItemMap;

    QList<Item *> pItemList = cart.GetItems();
    for (int i = 0; i < pItemList.size(); ++i) {
        Item *pItem = pItemList.at(i);
        // item info
        rept.InsertItem(pItem->GetBillText());

        // offer
        ItemOffer *pOffer = pItem->GetOffer();
        if (pOffer) {
            pOfferMap.insert(pOffer->GetType(), pOffer);
            pOfferItemMap.insert(pOffer->GetType(), pItem);
        }
    }

    // offer info
    QHashIterator<QString, ItemOffer *> itOffer(pOfferMap);
    while (itOffer.hasNext()) {
        itOffer.next();

        // type & offer
        QString strType = itOffer.key();
        ItemOffer *pOffer = itOffer.value();

        // offer -- goods list
        QList<Item *> pOfferItemList = pOfferItemMap.values(strType);

        // offer info
        rept.InsertOffer(pOffer->GetOfferText(pOfferItemList));
    }

    // cost info
    rept.SetCost(cart.GetBillText());

    return rept.GetReceipt();
}


/******************** Printer ********************/
Printer::Printer(const QString &strStore) {
    m_strStore = strStore;
}

Printer::~Printer() {
    this->Clear();
}

void Printer::SetGoodsInfo(const QHash<QString, Goods *> &pGoodsMap) {
    m_pGoodsMap = pGoodsMap;
}

void Printer::SetOfferInfo(const QHash<QString, ItemOffer *> &pOfferMap) {
    m_pOfferMap = pOfferMap;
}

bool Printer::InputCart(const QString &strCartJson) {
    // strings in standard json should be wrapped in double quotes,
    // thus we replace all \' to \" first
    QString strJson = strCartJson;
    strJson.replace('\'', '\"');

    m_cart.ClearItems();
    if (!Parser::ParseCart(strJson, m_cart, m_pGoodsMap)) {
        return false;
    }

    // set offers
    QHashIterator<QString, ItemOffer *> itOffer(m_pOfferMap);
    while (itOffer.hasNext()) {
        m_cart.SetOffer(itOffer.next().value());
    }

    return true;
}

void Printer::PrintReceipt() {
    // receipt text
    QString strReceipt = Receipt::GetReceipt(m_strStore, m_cart);

    // print onto console
    QTextStream qout(stdout);
    qout << strReceipt;
}

void Printer::Clear() {
    // clear cart
    m_cart.ClearItems();

    // clear goods & offer list
    m_pGoodsMap.clear();
    m_pOfferMap.clear();
}


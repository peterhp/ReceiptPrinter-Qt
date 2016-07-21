#ifndef _QPT_HEADER_RECEIPT_PRINTER_H_
#define _QPT_HEADER_RECEITP_PRINTER_H_

#include "basic/ItemCart.h"

class Goods;
class ItemOffer;

#include <QString>

#include <QList>
#include <QHash>

QT_USE_NAMESPACE

class Receipt {
public:
    // store name
    Receipt(const QString &strStore);
    QString GetStoreName() const;

    // item info
    void InsertItem(const QString &strInfo);
    void InsertItem(const QList<QString> &strInfoList);

    // offer info
    void InsertOffer(const QString &strInfo);
    void InsertOffer(const QList<QString> &strInfoList);

    // cost info
    void SetCost(const QString &strInfo);

    // receipt
    QString GetReceipt() const;

    // cart receipt
    static QString GetReceipt(const QString &strStore, const Cart &cart);

private:
    QString m_strStore; // store name

    QList<QString> m_strItemInfoList;   // item accounts
    QList<QString> m_strOfferInfoList;  // offer message
    QString m_strCostInfo;              // total account
};


class Printer {
public:
    Printer(const QString &strStore);
    ~Printer();

    // init goods & offer list
    void SetGoodsInfo(const QHash<QString, Goods *> &pGoodsMap);
    void SetOfferInfo(const QHash<QString, ItemOffer *> &pOfferMap);

    // input json data of cart
    bool InputCart(const QString &strCartJson);

    // print receipt onto console
    void PrintReceipt();

    // clear
    void Clear();

private:
    QString m_strStore; // store name

    QHash<QString, Goods *> m_pGoodsMap;        // goods list
    QHash<QString, ItemOffer *> m_pOfferMap;    // offer list

    Cart m_cart;    // items cart
};

#endif // ReceiptPrinter.h

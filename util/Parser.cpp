#include "Parser.h"
#include "basic/Goods.h"
#include "basic/ItemCart.h"
#include "basic/ItemOffer.h"
#include "basic/OfferBuy3Free1.h"

#include <QString>
#include <QFile>
#include <QTextStream>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QHash>

QT_USE_NAMESPACE

bool Parser::ParseGoodsMap(QFile &file,
        QHash<QString, Goods *> &pGoodsMap) {
    if (!file.open(QFile::ReadOnly | QFile::Text)) { // fail to open
        return false;
    }

    // codec of text in file is UTF-8
    QTextStream reader(&file);
    reader.setCodec("UTF-8");

    return ParseGoodsMap(reader.readAll(), pGoodsMap);
}

bool Parser::ParseOfferMap(QFile &file,
        QHash<QString, ItemOffer *> &pOfferMap,
        const QHash<QString, Goods *> &pGoodsMap) {
    if (!file.open(QFile::ReadOnly | QFile::Text)) { // fail to open
        return false;
    }

    QTextStream reader(&file);
    reader.setCodec("UTF-8");

    return ParseOfferMap(reader.readAll(), pOfferMap, pGoodsMap);
}

bool Parser::ParseCart(QFile &file, Cart &cart,
        const QHash<QString, Goods *> &pGoodsMap) {
    if (!file.open(QFile::ReadOnly | QFile::Text)) { // fail to open
        return false;
    }

    QTextStream reader(&file);
    reader.setCodec("UTF-8");

    return ParseCart(reader.readAll(), cart, pGoodsMap);
}

bool Parser::ParseGoodsMap(const QString &str,
        QHash<QString, Goods *> &pGoodsMap) {
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if (doc.isNull()) {
        return false;
    }

    // json array containing all goods info
    QJsonArray jGoodsArray = doc.array();
    if (jGoodsArray.isEmpty()) { // no goods info or error json data
        return false;
    }

    // parse each goods info
    for (int i = 0; i < jGoodsArray.size(); ++i) {
        QJsonValue jValue = jGoodsArray.at(i);
        if (jValue.isObject()) {
            QJsonObject jGoods = jValue.toObject(); // json of goods info

            // get goods info
            Goods *pGoods = ParseGoods(jGoods);
            if (pGoods) {
                pGoodsMap.insert(pGoods->GetBarcode(), pGoods);
            }
        }
    }

    return (pGoodsMap.size() > 0);
}

bool Parser::ParseOfferMap(const QString &str,
        QHash<QString, ItemOffer *> &pOfferMap,
        const QHash<QString, Goods *> &pGoodsMap) {
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if (doc.isNull()) {
        return false;
    }

    // json array containing all offer info
    QJsonArray jOfferArray = doc.array();
    if (jOfferArray.isEmpty()) { // no offer info or error json data
        return false;
    }

    // parse each offer info
    for (int i = 0; i < jOfferArray.size(); ++i) {
        QJsonValue jValue = jOfferArray.at(i);
        if (jValue.isObject()) {
            QJsonObject jOffer = jValue.toObject(); // json of offer info

            // get offer info
            ItemOffer *pOffer = ParseOffer(jOffer, pGoodsMap);
            if (pOffer) {
                pOfferMap.insert(pOffer->GetType(), pOffer);
            }
        }
    }

    return (pOfferMap.size() > 0);
}

bool Parser::ParseCart(const QString &str, Cart &cart,
        const QHash<QString, Goods *> &pGoodsMap) {
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
    if (doc.isNull()) {
        return false;
    }

    // json array containing all items
    QJsonArray jCodeArray = doc.array();
    if (jCodeArray.isEmpty()) { // no items or error json data
        return false;
    }

    // parse each item
    for (int i = 0; i < jCodeArray.size(); ++i) {
        QJsonValue jValue = jCodeArray.at(i);
        if (jValue.isString()) {
            QString strItem = jValue.toString();

            QString strCode;
            int nCnt = 0;
            // parse barcode and count
            int pos = strItem.indexOf("-");
            if (-1 == pos) {
                strCode = strItem;
                nCnt = 1;
            } else {
                strCode = strItem.left(pos);
                nCnt = strItem.mid(pos + 1).toInt();
                nCnt = (nCnt > 0) ? nCnt : 1;
            }

            // add item
            cart.Insert(pGoodsMap.value(strCode), nCnt);
        }
    }

    return true;
}

void Parser::ReleaseGoodsMap(QHash<QString, Goods *> &pGoodsMap) {
    QHashIterator<QString, Goods *> itGoods(pGoodsMap);
    while (itGoods.hasNext()) {
        Goods *pGoods = itGoods.next().value();
        delete pGoods;
    }
    pGoodsMap.clear();
}

void Parser::ReleaseOfferMap(QHash<QString, ItemOffer *> &pOfferMap) {
    QHashIterator<QString, ItemOffer *> itOffer(pOfferMap);
    while (itOffer.hasNext()) {
        ItemOffer *pOffer = itOffer.next().value();
        delete pOffer;
    }
    pOfferMap.clear();
}

Goods *Parser::ParseGoods(const QJsonObject &jGoods) {
    QJsonValue jValue;

    // barcode
    jValue = jGoods.value("barcode");
    if (jValue.isUndefined() || !jValue.isString()) {
        return NULL;
    }
    QString strBarcode = jValue.toString();

    // name
    jValue = jGoods.value("name");
    if (jValue.isUndefined() || !jValue.isString()) {
        return NULL;
    }
    QString strName = jValue.toString();

    // unit
    jValue = jGoods.value("unit");
    if (jValue.isUndefined() || !jValue.isString()) {
        return NULL;
    }
    QString strUnit = jValue.toString();

    // category
    jValue = jGoods.value("category");
    if (jValue.isUndefined() || !jValue.isString()) {
        return NULL;
    }
    QString strCategory = jValue.toString();

    // subCategory
    jValue = jGoods.value("subCategory");
    if (jValue.isUndefined() || !jValue.isString()) {
        return NULL;
    }
    QString strSubCategory = jValue.toString();

    // price
    jValue = jGoods.value("price");
    if (jValue.isUndefined() || !jValue.isDouble()) {
        return NULL;
    }
    float fPrice = (float)jValue.toDouble();

    // new a goods object
    Goods *pGoods = new Goods(strBarcode, strName);
    pGoods->SetCategory(strCategory, strSubCategory);
    pGoods->SetQuantUnit(strUnit);
    pGoods->SetUnitPrice(fPrice);

    return pGoods;
}

ItemOffer *Parser::ParseOffer(const QJsonObject &jOffer,
        const QHash<QString, Goods *> &pGoodsMap) {
    QJsonValue jValue;

    // type
    jValue = jOffer.value("type");
    if (jValue.isUndefined() || !jValue.isString()) {
        return NULL;
    }
    QString strType = jValue.toString();

    ItemOffer *pOffer = NULL;
    // parse offer info for each type
    if (!strType.compare(OFFER_TYPE_BUY3FREE1)) { // OfferBuy3Free1
        pOffer = ParseBuy3Free1(jOffer, pGoodsMap);
    }

    return pOffer;
}

ItemOffer *Parser::ParseBuy3Free1(const QJsonObject &jOffer,
        const QHash<QString, Goods *> &pGoodsMap) {
    QJsonValue jValue;

    // barcodes
    jValue = jOffer.value("barcodes");
    if (jValue.isUndefined() || !jValue.isArray()) {
        return NULL;
    }
    QJsonArray jCodeArray = jValue.toArray();

    ItemOffer *pOffer = new OfferBuy3Free1();
    // parse each barcode
    for (int i = 0; i < jCodeArray.size(); ++i) {
        jValue = jCodeArray.at(i);

        if (jValue.isString()) { // add offer goods
            pOffer->InsertGoods(pGoodsMap.value(jValue.toString()));
        }
    }

    return pOffer;
}


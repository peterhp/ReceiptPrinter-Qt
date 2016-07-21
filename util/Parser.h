#ifndef _QPT_HEADER_DATA_PARSER_H_
#define _QPT_HEADER_DATA_PARSER_H_

#include <QFile>
#include <QString>

#include <QJsonObject>

#include <QHash>

QT_USE_NAMESPACE

class Goods;
class Cart;

class ItemOffer;

/**
 * Class Parser is mainly used to parse data of goods,
 * offers and cart items. Here it's only implimented
 * to get data from json text.
 *
 * Remarks:
 * The string-typed key and value of standard json text
 * are wrapped in double quotes. So the json parser of
 * Qt does.
**/

class Parser {
public:
    // parse data from json file
    static bool ParseGoodsMap(QFile &file,
            QHash<QString, Goods *> &pGoodsMap);
    static bool ParseOfferMap(QFile &file,
            QHash<QString, ItemOffer *> &pOfferMap,
            const QHash<QString, Goods *> &pGoodsMap);
    static bool ParseCart(QFile &file, Cart &cart,
            const QHash<QString, Goods *> &pGoodsMap);

    // parse data from json string
    static bool ParseGoodsMap(const QString &str,
            QHash<QString, Goods *> &pGoodsMap);
    static bool ParseOfferMap(const QString &str,
            QHash<QString, ItemOffer *> &pOfferMap,
            const QHash<QString, Goods *> &pGoodsMap);
    static bool ParseCart(const QString &str, Cart &cart,
            const QHash<QString, Goods *> &pGoodsMap);

    // release data
    static void ReleaseGoodsMap(QHash<QString, Goods *> &pGoodsMap);
    static void ReleaseOfferMap(QHash<QString, ItemOffer *> &pOfferMap);

private:
    static Goods *ParseGoods(const QJsonObject &jGoods);

    static ItemOffer *ParseOffer(const QJsonObject &jOffer,
            const QHash<QString, Goods *> &pGoodsMap);
    static ItemOffer *ParseBuy3Free1(const QJsonObject &jOffer,
            const QHash<QString, Goods *> &pGoodsMap);
};

#endif // Parser.h

#include "Test.h"
#include "basic/Goods.h"
#include "basic/ItemCart.h"
#include "basic/ItemOffer.h"
#include "util/Parser.h"

#include <QString>
#include <QFile>
#include <QFileInfo>

#include <QHash>

#include <QDebug>

QT_USE_NAMESPACE

void TestParser() {
    const QString strGoodsFile = ":/data/goods.json";
    const QString strOfferFile = ":/data/offer.json";
    const QString strCartFile  = ":/data/cart.json";

    QFile fGoods(strGoodsFile);
    QFile fOffer(strOfferFile);
    QFile fCart(strCartFile);

    QHash<QString, Goods *> pGoodsMap;
    QHash<QString, ItemOffer *> pOfferMap;
    Cart cart;

    if (!Parser::ParseGoodsMap(fGoods, pGoodsMap)) {
        qDebug() << "Fail to parse goods list.";
        return ;
    }

    if (!Parser::ParseOfferMap(fOffer, pOfferMap, pGoodsMap)) {
        qDebug() << "Fail to parse offer list.";
        return ;
    }

    if (!Parser::ParseCart(fCart, cart, pGoodsMap)) {
        qDebug() << "Fail to parse cart items.";
        return ;
    }

    qDebug() << "********** Goods Info **********";
    QHashIterator<QString, Goods *> itGoods(pGoodsMap);
    while (itGoods.hasNext()) {
        PrintGoods(itGoods.next().value());
    }

    qDebug() << "********** Offer Info **********";
    QHashIterator<QString, ItemOffer *> itOffer(pOfferMap);
    while (itOffer.hasNext()) {
        PrintOffer(itOffer.next().value());
    }

    qDebug() << "********** Cart Info **********";
    PrintCart(&cart);

    Parser::ReleaseOfferMap(pOfferMap);
    Parser::ReleaseGoodsMap(pGoodsMap);
}


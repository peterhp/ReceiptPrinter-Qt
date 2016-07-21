#include "basic/Goods.h"
#include "basic/ItemOffer.h"
#include "util/Parser.h"
#include "module/ReceiptPrinter.h"

#include <QString>
#include <QDebug>

#include <QHash>

QT_USE_NAMESPACE

void TestModule() {
    QHash<QString, Goods *> pGoodsMap;
    QHash<QString, ItemOffer *> pOfferMap;

    QFile fGoods(":/data/goods.json");
    QFile fOffer(":/data/offer.json");

    // goods list
    if (!Parser::ParseGoodsMap(fGoods, pGoodsMap)) {
        qDebug() << "Fail to parse goods list.";
        return ;
    }

    // offer list
    if (!Parser::ParseOfferMap(fOffer, pOfferMap, pGoodsMap)) {
        qDebug() << "Fail to parse offer list.";
        return ;
    }

    // item barcodes with offers
    QString strItems = "["
                "'ITEM000001',"
                "'ITEM000001',"
                "'ITEM000001',"
                "'ITEM000003-2',"
                "'ITEM000005',"
                "'ITEM000005',"
                "'ITEM000005',"
                "'ITEM000005',"
                "'ITEM000005'"
            "]";

    // printer module
    Printer printer("没钱赚商店");
    printer.SetGoodsInfo(pGoodsMap);
    printer.SetOfferInfo(pOfferMap);
    printer.InputCart(strItems);
    printer.PrintReceipt();

    // item barcodes without offers
    strItems = "["
            "'ITEM000002',"
            "'ITEM000002',"
            "'ITEM000002',"
            "'ITEM000003-2',"
            "'ITEM000004-3',"
            "'ITEM000005',"
            "'ITEM000005'"
        "]";

    printer.InputCart(strItems);
    printer.PrintReceipt();
}


/**
 * The text encoding of all the words within this
 * project is UTF-8.
**/

#include "basic/Goods.h"
#include "basic/ItemOffer.h"
#include "util/Parser.h"
#include "module/ReceiptPrinter.h"
#include "test/Test.h"

#include <QString>
#include <QFile>
#include <QTextStream>

#include <QHash>

#include <QDebug>

QT_USE_NAMESPACE

static QHash<QString, Goods *> g_pGoodsMap;
static QHash<QString, ItemOffer *> g_pOfferMap;

static const QString g_strStore = "没钱赚商店";
static Printer g_receiptPrinter(g_strStore);

static bool InitModule(const QString &strGoodsFile,
        const QString &strOfferFile) {
    QFile fGoods(strGoodsFile);
    QFile fOffer(strOfferFile);

    // parse goods & offer list
    if (!Parser::ParseGoodsMap(fGoods, g_pGoodsMap)) {
        return false;
    }
    if (!Parser::ParseOfferMap(fOffer, g_pOfferMap, g_pGoodsMap)) {
        return false;
    }

    // init printer
    g_receiptPrinter.SetGoodsInfo(g_pGoodsMap);
    g_receiptPrinter.SetOfferInfo(g_pOfferMap);

    return true;
}

static void CloseModule() {
    // clear printer
    g_receiptPrinter.Clear();

    // clear goods & offer list
    Parser::ReleaseGoodsMap(g_pGoodsMap);
    Parser::ReleaseOfferMap(g_pOfferMap);
}

int main(int argc, char *argv[]) {
    const QString strGoodsFile = ":/data/goods.json";
    const QString strOfferFile = ":/data/offer.json";

    qDebug() << "Init receipt printer module.";
    if (!InitModule(strGoodsFile, strOfferFile)) {
        qDebug() << "Fail to init receipt printer module.";
        return -1;
    }
    qDebug() << "Receipt printer module starts to run.";

    // items bought
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

    qDebug() << "Print receipt for new-coming item cart.";
    g_receiptPrinter.InputCart(strItems);
    g_receiptPrinter.PrintReceipt();

    qDebug() << "Close receipt printer module.";
    CloseModule();

    //TestAll();

    return 0;
}


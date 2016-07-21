#include "Test.h"
#include "basic/Goods.h"
#include "basic/ItemCart.h"
#include "basic/ItemOffer.h"

#include <QString>

#include <QList>

#include <QDebug>

QT_USE_NAMESPACE

void PrintGoods(const Goods *pGoods) {
    if (pGoods) {
        QString strInfo;
        strInfo += QString("barcode: %1\n").arg(pGoods->GetBarcode());
        strInfo += QString("name: %1\n").arg(pGoods->GetName());
        strInfo += QString("unit: %1\n").arg(pGoods->GetQuantUnit());
        strInfo += QString("category: %1\n").arg(pGoods->GetCategory());
        strInfo += QString("subcategory: %1\n").arg(pGoods->GetSubCategory());
        strInfo += QString("price: %1\n").arg(pGoods->GetUnitPrice(), 0, 'f', 2);

        qDebug() << strInfo;
    }
}

void PrintItem(const Item *pItem) {
    Goods *pGoods = NULL;
    if (pItem && (pGoods = pItem->GetGoods())) {
        QString strInfo;

        strInfo += QString("name: %1\n").arg(pGoods->GetName());
        strInfo += QString("quantity: %1%2\n")
                .arg(pItem->GetQuant()).arg(pGoods->GetQuantUnit());
        strInfo += QString("price: %1\n")
                .arg(pGoods->GetUnitPrice(), 0, 'f', 2);
        strInfo += QString("cost: %1\n")
                .arg(pItem->GetActualPrice(), 0, 'f', 2);
        if (pItem->GetSaving() > 0) {
            strInfo += QString("discount: %1\n")
                    .arg(pItem->GetSaving(), 0, 'f', 2);
        }

        qDebug() << strInfo;
    }
}

void PrintCart(const Cart *pCart) {
    if (pCart) {
        // item info
        QList<Item *> pItemList = pCart->GetItems();
        for (int i = 0; i < pItemList.size(); ++i) {
            PrintItem(pItemList.at(i));
        }

        // cart info
        QString strInfo;
        strInfo += QString("total: %1\n")
                .arg(pCart->GetActualCost(), 0, 'f', 2);
        strInfo += QString("saving: %1\n")
                .arg(pCart->GetSaving(), 0, 'f', 2);
        qDebug() << strInfo;
    }
}

void PrintOffer(const ItemOffer *pOffer) {
    if (pOffer) {
        QString strInfo;

        // type
        strInfo += QString("type: %1\n")
                .arg(pOffer->GetType());
        // codes of offer goods
        strInfo += "barcodes:\n";
        QList<Goods *> pGoodsList = pOffer->GetGoods();
        for (int i = 0; i < pGoodsList.size(); ++i) {
            strInfo += QString("%1\n")
                    .arg(pGoodsList.at(i)->GetBarcode());
        }

        qDebug() << strInfo;
    }
}

void TestAll() {
    qDebug() << "****************************************";
    qDebug() << "*************** ItemCart ***************";
    TestItemCart();

    qDebug() << "****************************************";
    qDebug() << "**************** Offers ****************";
    TestItemOffer();

    qDebug() << "****************************************";
    qDebug() << "**************** Parser ****************";
    TestParser();

    qDebug() << "****************************************";
    qDebug() << "**************** Printer ***************";
    TestModule();
}


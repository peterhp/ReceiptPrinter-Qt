#include "Test.h"
#include "basic/Goods.h"
#include "basic/ItemCart.h"
#include "basic/OfferBuy3Free1.h"

#include <QString>
#include <QStringList>

#include <QList>
#include <QHash>
#include <QMultiHash>

#include <QDebug>

QT_USE_NAMESPACE

static QHash<QString, Goods *> g_pGoodsMap;
static QHash<QString, ItemOffer *> g_pOfferMap;

static void GenGoods() {
    Goods *pCoke = new Goods("ITEM000001", "可口可乐");
    pCoke->SetCategory("食品", "饮料");
    pCoke->SetQuantUnit("瓶");
    pCoke->SetUnitPrice(3.00);
    g_pGoodsMap.insert(pCoke->GetBarcode(), pCoke);

    Goods *pSprite = new Goods("ITEM000002", "雪碧");
    pSprite->SetCategory("食品", "饮料");
    pSprite->SetQuantUnit("瓶");
    pSprite->SetUnitPrice(3.00);
    g_pGoodsMap.insert(pSprite->GetBarcode(), pSprite);

    Goods *pApple = new Goods("ITEM000003", "苹果");
    pApple->SetCategory("食品", "水果");
    pApple->SetQuantUnit("斤");
    pApple->SetUnitPrice(5.50);
    g_pGoodsMap.insert(pApple->GetBarcode(), pApple);

    Goods *pLettuces = new Goods("ITEM000004", "油麦菜");
    pLettuces->SetCategory("食品", "蔬菜");
    pLettuces->SetQuantUnit("斤");
    pLettuces->SetUnitPrice(4.50);
    g_pGoodsMap.insert(pLettuces->GetBarcode(), pLettuces);

    Goods *pBadminton = new Goods("ITEM000005", "羽毛球");
    pBadminton->SetCategory("体育用品", "球类");
    pBadminton->SetQuantUnit("个");
    pBadminton->SetUnitPrice(1.00);
    g_pGoodsMap.insert(pBadminton->GetBarcode(), pBadminton);

    Goods *pBasketball = new Goods("ITEM000006", "篮球");
    pBasketball->SetCategory("体育用品", "球类");
    pBasketball->SetQuantUnit("个");
    pBasketball->SetUnitPrice(98.00);
    g_pGoodsMap.insert(pBasketball->GetBarcode(), pBasketball);
}

static void ClearGoods() {
    QHashIterator<QString, Goods *> itGoods(g_pGoodsMap);
    // release memory
    while (itGoods.hasNext()) {
        Goods *pGoods = itGoods.next().value();
        delete pGoods;
    }
    // clear hash map
    g_pGoodsMap.clear();
}

static Cart *GenCart() {
    Cart *pCart = new Cart();

    QStringList strCodeList;
    strCodeList << "ITEM000001" << "ITEM000001" << "ITEM000001"
            << "ITEM000003" << "ITEM000003" << "ITEM000005"
            << "ITEM000005" << "ITEM000005" << "ITEM000005"
            << "ITEM000005";

    // add goods into cart
    for (int i = 0; i < strCodeList.size(); ++i) {
        Goods *pGoods = g_pGoodsMap.value(strCodeList.at(i));
        if (pGoods) {
            pCart->Insert(pGoods);
        }
    }

    return pCart;
}

static void ReleaseCart(Cart *pCart) {
    if (pCart) {
        delete pCart;
    }
}

static void GenOffer() {
    // offer, buy three get one free
    OfferBuy3Free1 *pOffer = new OfferBuy3Free1();
    QStringList strCodeList;
    strCodeList << "ITEM000001" << "ITEM000005";
    for (int i = 0; i < strCodeList.size(); ++i) {
        pOffer->InsertGoods(g_pGoodsMap.value(strCodeList.at(i)));
    }

    g_pOfferMap.insert(pOffer->GetType(), pOffer);
}

static void ClearOffer() {
    QHashIterator<QString, ItemOffer *> itOffer(g_pOfferMap);
    while (itOffer.hasNext()) {
        ItemOffer *pOffer = itOffer.next().value();
        delete pOffer;
    }
    g_pOfferMap.clear();
}

static void SetOffer(Cart *pCart) {
    if (!pCart) {
        return ;
    }

    QHashIterator<QString, ItemOffer *> itOffer(g_pOfferMap);
    while (itOffer.hasNext()) {
        ItemOffer *pOffer = itOffer.next().value();

        QList<Item *> pItemList = pCart->GetItems();
        for (int i = 0; i < pItemList.size(); ++i) {
            Item *pItem = pItemList.at(i);

            if (pOffer->Contains(pItem->GetBarcode())) {
                pItem->SetOffer(pOffer);
            }
        }
    }
}

static void PrintReceipt(Cart *pCart) {
    if (!pCart) {
        return ;
    }

    QString strBill;
    strBill += "***<没钱赚商店>购物清单***\n";

    QMultiHash<QString, Item *> pOfferItemMap;
    // item
    QList<Item *> pItemList = pCart->GetItems();
    for (int i = 0; i < pItemList.size(); ++i) {
        Item *pItem = pItemList.at(i);
        strBill += pItem->GetBillText();

        ItemOffer *pOffer = pItem->GetOffer();
        if (pOffer) {
            pOfferItemMap.insert(pOffer->GetType(), pItem);
        }
    }
    strBill += "----------------------\n";

    // offer
    if (!pOfferItemMap.isEmpty()) {
        QList<QString> offerTypeList = pOfferItemMap.uniqueKeys();
        for (int i = 0; i < offerTypeList.size(); ++i) {
            ItemOffer *pOffer = g_pOfferMap.value(offerTypeList.at(i));

            strBill += pOffer->GetOfferText(
                        pOfferItemMap.values(offerTypeList.at(i)));
        }
        strBill += "----------------------\n";
    }

    // cart
    strBill += pCart->GetBillText();

    strBill += "**********************\n";

    qDebug() << strBill;
}

void TestItemCart() {
    GenGoods();
    qDebug() << "********** Goods List **********";
    QHashIterator<QString, Goods *> itGoods(g_pGoodsMap);
    while (itGoods.hasNext()) {
        PrintGoods(itGoods.next().value());
    }

    Cart *pCart = GenCart();
    qDebug() << "********** Cart Info **********";
    PrintCart(pCart);

    ReleaseCart(pCart);

    ClearGoods();
}

void TestItemOffer() {
    GenGoods();

    Cart *pCart = GenCart();
    qDebug() << "********** Receipt with No Offers **********";
    PrintReceipt(pCart);

    GenOffer();
    qDebug() << "********** Offers Info **********";
    QHashIterator<QString, ItemOffer *> itOffer(g_pOfferMap);
    while (itOffer.hasNext()) {
        PrintOffer(itOffer.next().value());
    }

    SetOffer(pCart);
    qDebug() << "********** Receipt with Offers **********";
    PrintReceipt(pCart);

    ClearOffer();
    ReleaseCart(pCart);
    ClearGoods();
}


#include "Goods.h"
#include "ItemCart.h"
#include "ItemOffer.h"

#include <QString>

#include <QHash>
#include <QList>

QT_USE_NAMESPACE

/******************** Item ********************/
Item::Item(Goods *pGoods) {
    m_pGoods = pGoods;
    m_nQuant = 0;

    m_pOffer = NULL;
}

QString Item::GetBarcode() const {
    if (m_pGoods) {
        return m_pGoods->GetBarcode();
    } else {
        return QString();
    }
}

Goods *Item::GetGoods() const {
    return m_pGoods;
}

void Item::AddGoods(int nCount) {
    if (nCount > 0) {
        m_nQuant += nCount;
    }
}

void Item::DelGoods(int nCount) {
    if (nCount > 0) {
        m_nQuant = (m_nQuant >= nCount) ?
                (m_nQuant - nCount) : 0;
    }
}

int Item::GetQuant() const {
    return m_nQuant;
}

void Item::SetOffer(ItemOffer *pOffer) {
    if (pOffer && (pOffer->GetSaving(this) > this->GetSaving())) {
        m_pOffer = pOffer;
    }
}

ItemOffer *Item::GetOffer() const {
    return m_pOffer;
}

float Item::GetOriginalPrice() const {
    if (m_pGoods) {
        return m_pGoods->GetUnitPrice() * m_nQuant;
    } else {
        return 0;
    }
}

float Item::GetSaving() const {
    if (m_pOffer) {
        return m_pOffer->GetSaving(this);
    } else {
        return 0;
    }
}

float Item::GetActualPrice() const {
    return this->GetOriginalPrice() - this->GetSaving();
}

QString Item::GetBillText() const {
    QString strInfo;

    if (m_pGoods && m_nQuant > 0) {
        // name
        strInfo += QString("名称：%1，")
                .arg(m_pGoods->GetName());
        // quantity
        strInfo += QString("数量：%1%2，")
                .arg(m_nQuant).arg(m_pGoods->GetQuantUnit());
        // unit price
        strInfo += QString("单价：%1（元），")
                .arg(m_pGoods->GetUnitPrice(), 0, 'f', 2);
        // actual price
        strInfo += QString("小计：%1（元）\n")
                .arg(this->GetActualPrice(), 0, 'f', 2);
    }

    return strInfo;
}


/******************** Item ********************/
Cart::Cart() {

}

Cart::~Cart() {
    this->ClearItems();
}

void Cart::Insert(Goods *pGoods, int nCount) {
    if (pGoods) {
        if (m_pItemMap.contains(pGoods->GetBarcode())) { // already exist
            m_pItemMap.value(pGoods->GetBarcode())->AddGoods(nCount);
        } else { // no goods item exists, malloc an new one
            Item *pItem = new Item(pGoods);
            pItem->AddGoods(nCount);
            m_pItemMap.insert(pGoods->GetBarcode(), pItem);
        }
    }
}

void Cart::Remove(Goods *pGoods, int nCount) {
    if (this->Contains(pGoods)) {
        Item *pItem = m_pItemMap.value(pGoods->GetBarcode());
        pItem->DelGoods(nCount);

        if (!pItem->GetQuant()) { // no goods, remove item
            m_pItemMap.remove(pGoods->GetBarcode());
            delete pItem;
        }
    }
}

QList<Item *> Cart::GetItems() const {
    return m_pItemMap.values();
}

void Cart::ClearItems() {
    QHashIterator<QString, Item *> itItem(m_pItemMap);
    while (itItem.hasNext()) { // release memory for items
        Item *pItem = itItem.next().value();
        delete pItem;
    }
    m_pItemMap.clear(); // clear hash map
}

bool Cart::Contains(const QString &strBarcode) const {
    return m_pItemMap.contains(strBarcode);
}

bool Cart::Contains(const Goods *pGoods) const {
    return (pGoods && m_pItemMap.contains(pGoods->GetBarcode()));
}

Item *Cart::Find(const QString &strBarcode) const {
    // NULL will be returned if goods isn't contained
    return m_pItemMap.value(strBarcode);
}

Item *Cart::Find(const Goods *pGoods) const {
    if (pGoods) {
        return m_pItemMap.value(pGoods->GetBarcode());
    } else {
        return NULL;
    }
}

void Cart::SetOffer(ItemOffer *pOffer) {
    QHashIterator<QString, Item *> itItem(m_pItemMap);
    while (itItem.hasNext()) {
        Item *pItem = itItem.next().value();
        if (pOffer->Contains(pItem->GetBarcode())) {
            pItem->SetOffer(pOffer);
        }
    }
}

float Cart::GetOriginalCost() const {
    float fCost = 0;

    // item cost
    QHashIterator<QString, Item *> itItem(m_pItemMap);
    while (itItem.hasNext()) {
        Item *pItem = itItem.next().value();
        fCost += pItem->GetOriginalPrice();
    }

    return fCost;
}

float Cart::GetActualCost() const {
    float fCost = 0;

    // item actual cost
    QHashIterator<QString, Item *> itItem(m_pItemMap);
    while (itItem.hasNext()) {
        Item *pItem = itItem.next().value();
        fCost += pItem->GetActualPrice();
    }

    return fCost;
}

float Cart::GetSaving() const {
    return this->GetOriginalCost() - this->GetActualCost();
}

QString Cart::GetBillText() const {
    QString strInfo;

    // total
    strInfo += QString("总计：%1（元）\n")
            .arg(this->GetActualCost(), 0, 'f', 2);
    // saving
    if (this->GetSaving() > 0) {
        strInfo += QString("节省：%1（元）\n")
                .arg(this->GetSaving(), 0, 'f', 2);
    }

    return strInfo;
}


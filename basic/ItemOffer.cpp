#include "Goods.h"
#include "ItemCart.h"
#include "ItemOffer.h"

#include <QString>

#include <QList>
#include <QHash>

QT_USE_NAMESPACE

ItemOffer::ItemOffer() {
    m_strType = "";
}

ItemOffer::~ItemOffer() {

}

QString ItemOffer::GetType() const {
    return m_strType;
}

void ItemOffer::InsertGoods(Goods *pGoods) {
    if (pGoods) {
        m_pGoodsMap.insert(pGoods->GetBarcode(), pGoods);
    }
}

void ItemOffer::RemoveGoods(Goods *pGoods) {
    if (pGoods) {
        m_pGoodsMap.remove(pGoods->GetBarcode());
    }
}

QList<Goods *> ItemOffer::GetGoods() const {
    return m_pGoodsMap.values();
}

bool ItemOffer::Contains(const QString &strBarcode) const {
    return m_pGoodsMap.contains(strBarcode);
}

bool ItemOffer::Contains(const Goods *pGoods) const {
    return (pGoods && m_pGoodsMap.contains(pGoods->GetBarcode()));
}

QString ItemOffer::GetOfferText(const Item *pItem) const {
    QString strItem = this->GetItemText(pItem);
    if (!strItem.isEmpty()) {
        return QString("%1\n%2\n")
                .arg(this->GetHeadText()).arg(strItem);
    } else {
        return QString();
    }
}

QString ItemOffer::GetOfferText(const QList<Item *> &pItemList) const {
    QString strItem, strInfo;

    for (int i = 0; i < pItemList.size(); ++i) {
        strItem = this->GetItemText(pItemList.at(i));
        if (!strItem.isEmpty()) {
            strInfo += QString("%1\n").arg(strItem);
        }
    }

    if (!strInfo.isEmpty()) {
        return QString("%1\n%2")
                .arg(this->GetHeadText()).arg(strInfo);
    } else {
        return QString();
    }
}


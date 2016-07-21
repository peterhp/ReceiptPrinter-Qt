#include "Goods.h"
#include "ItemCart.h"
#include "OfferBuy3Free1.h"

#include <QString>

QT_USE_NAMESPACE

OfferBuy3Free1::OfferBuy3Free1() {
    m_strType = OFFER_TYPE_BUY3FREE1;
}

float OfferBuy3Free1::GetSaving(const Item *pItem) const {
    if (pItem && this->Contains(pItem->GetBarcode())) {
        return (pItem->GetGoods()->GetUnitPrice()) *
                (pItem->GetQuant() / 3);
    } else {
        return 0;
    }
}

QString OfferBuy3Free1::GetHeadText() const {
    return QString("买三免一商品：");
}

QString OfferBuy3Free1::GetItemText(const Item *pItem) const {
    if (pItem && (pItem->GetQuant() / 3)) {
        return QString("名称：%1，数量：%2%3")
                .arg(pItem->GetGoods()->GetName())
                .arg(pItem->GetQuant() / 3)
                .arg(pItem->GetGoods()->GetQuantUnit());
    } else {
        return QString();
    }
}

